#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <Windows.h>
using namespace std;

//进程状态
enum ProcessState { READY, RUNNING, BLOCKED, FINISH };//进程状态，有就绪态、运行态、等待态（阻塞态）和完成态
//等待原因
enum BlockReason { S1, S2 };//等待原因（在模拟实验中进程等待原因是为等待信号量s1或s2）
//PCB的结构体
struct PCB
{
	string name;//进程名
	ProcessState state;//状态
	BlockReason reason;//等待原因
	int BreakPoint;//断点
};
typedef int semaphore;//信号量
typedef int product;//产品
constexpr auto BufferLength = 10;//缓冲区长度，本题设为10
//缓冲区
product buffer[BufferLength];
int in, out;//in和out的初始值为0
int x = 0;
//进程的指令序列
int ProducerArray[5] = { 4,0,2,1,6 };//PA: produce, p, put, v, goto 0
int ConsumerArray[5] = { 0,3,1,5,6 };//SA: p, get, v, consume, goto 0
int PC;//模拟调度中的PC
PCB Producer, Consumer;//Producer和Consumer进程
PCB* CurrentProcess;
product production;//production初始为0
semaphore s1, s2;//s1和s2为信号量，初始时s1为10，s2为0
int max;//最大生产数

void p()
{
	if (CurrentProcess == &Producer)
	{
		cout << "生产者P操作。" << endl;
		if (--s1 < 0)//将信号量s减去1，若结果小于0，则执行原语的进程被置成等待信号量s的状态
		{
			cout << "生产者等待中..." << endl;
			CurrentProcess->state = BLOCKED;
			CurrentProcess->reason = S1;
			CurrentProcess->BreakPoint = 0;
		}
		else
			cout << "继续。" << endl;
	}
	else
	{
		cout << "消费者P操作。" << endl;
		if (--s2 < 0)//将信号量s减去1，若结果小于0，则执行原语的进程被置成等待信号量s的状态
		{
			cout << "消费者等待中..." << endl;
			CurrentProcess->state = BLOCKED;
			CurrentProcess->reason = S2;
			CurrentProcess->BreakPoint = 0;
		}
		else
			cout << "继续。" << endl;
	}
}

void v()
{
	if (CurrentProcess == &Producer)
	{
		cout << "生产者V操作。" << endl;
		if (++s2 <= 0 && Consumer.state == BLOCKED)//将信号量s加1，若结果不大于0，则释放一个等待信号量s的进程
			Consumer.state = READY;
	}
	else
	{
		cout << "消费者V操作。" << endl;
		if (++s1 <= 0 && Producer.state == BLOCKED)//将信号量s加1，若结果不大于0，则释放一个等待信号量s的进程
			Producer.state = READY;
	}
}

void put()
{
	buffer[in] = production;
	cout << "生产者在" << in << "存入了" << production << "。\n";
	in = (in + 1) % BufferLength;
}

void get()
{
	x = buffer[out];
	cout << "消费者在" << out << "取出了" << x << "。\n";
	out = (out + 1) % BufferLength;
}

void produce()
{
	production++;
	cout << "产品" << production << "已被生产。\n";
}

void consume()
{
	cout << "产品" << x << "已被取出。\n";
}

void goto0()
{
	if (CurrentProcess == &Producer)
	{
		cout << "生产者执行goto操作。" << endl;
		if (production >= max)
		{
			cout << "生产者可生产的商品数量达到最大值，生产者运行结束。" << endl;
			CurrentProcess->state = FINISH;
		}
	}
	else
		cout << "消费者执行goto操作。" << endl;
	PC = 0;
}

void Initiate()//初始化
{
	s1 = BufferLength;
	s2 = 0;
	Producer.name = "producer";
	Consumer.name = "consumer";
	Producer.state = Consumer.state = READY;
	Producer.BreakPoint = Consumer.BreakPoint = 0;
	CurrentProcess = &Producer;
	PC = 0;
	in = out = 0;
	production = 0;
	srand((unsigned int)time(NULL));
}

void Execute()
{
	int i = PC++;
	int j = (CurrentProcess == &Producer) ? ProducerArray[i] : ConsumerArray[i];
	//Sleep(100);//不让结果一次性出完
	//根据 j 的大小，选择下一个指令
	switch (j)
	{
	case 0: p(); break;
	case 1: v(); break;
	case 2: put(); break;
	case 3: get(); break;
	case 4: produce(); break;
	case 5: consume(); break;
	case 6: goto0(); break;
	default: break;//空操作，即NOP
	}
}

void Dispatch()
{
	while (true)
	{
		//将当前状态设为就绪，保护现场
		CurrentProcess->BreakPoint = PC;
		if (CurrentProcess->state != BLOCKED && CurrentProcess->state != FINISH)
			CurrentProcess->state = READY;
		//从就绪队列中按随机数选一个出来
		int choice = rand() % 2;
		if (Producer.state == READY && Consumer.state == READY)
			CurrentProcess = (choice == 0) ? &Producer : &Consumer;
		else if (Producer.state == READY)//此时Consumer.state!=READY
			CurrentProcess = &Producer;
		else if (Consumer.state == READY)//此时Producer.state!=READY
			CurrentProcess = &Consumer;
		else//无就绪进程
		{
			cout << "就绪队列为空，调度结束。" << endl;
			exit(0);
		}
		//将现行进程状态改为运行态
		CurrentProcess->state = RUNNING;
		PC = CurrentProcess->BreakPoint;
		Execute();
	}
}
int main()
{
	cout << "请输入生产者可生产的最大商品数量：" << endl;//当到达生产者可生产的最大商品数量时，生产者运行结束（为人工选择的方法）
	cin >> max;
	Initiate();//初始化
	Dispatch();//运行调度
	return 0;
}