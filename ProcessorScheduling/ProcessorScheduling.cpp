#include <iostream>
#include <queue>
using namespace std;

enum State { R, E }; //R（0）表示就绪，E（1）表示结束
class PCB
{
private:
	string threadName;//进程名
	int requiredRuntime;//要求运行时间
	signed int priority; //优先级，优先数越大，优先级越高
	State state;//状态，R（0）表示就绪，E（1）表示结束
public:
	PCB()//不带参数的构造函数
	{
		threadName = " ";
		requiredRuntime = 0;
		priority = 0;
		state = R;
	}
	PCB(string threadName, int requiredRuntime, int priority)//带参数的构造函数
	{
		this->threadName = threadName;
		this->requiredRuntime = requiredRuntime;
		this->priority = priority;
		state = R;
	}
	
	//获取进程名
	string getThreadName()
	{
		return threadName;
	}

	//获取、设置要求运行时间
	int getRequiredRuntime()
	{
		return requiredRuntime;
	}
	void setRequiredRuntime()
	{
		requiredRuntime--;
	}
	
	//获取、设置优先数
	int getPriority()
	{
		return priority;
	}
	void setPriority()
	{
		priority--;
	}

	//获取、设置状态
	State getState()
	{
		return state;
	}
	void setState()
	{
		state = E;
	}
	
	//重载“=”运算符
	PCB*& operator= (PCB*& c2)
	{
		this->threadName=c2->getThreadName();//进程名
		this->requiredRuntime=c2->getRequiredRuntime();//要求运行时间
		this->priority=c2->getPriority(); //优先级，优先数越大，优先级越高
		this->state=c2->getState();
		return c2;
	}
};
int main()
{
	queue <PCB> pcb;//pcb为就绪队列
	
	//获取优先数和要求运行时间
	int Priority[5] = { 0,0,0,0,0 };
	int RequiredRuntime[5] = { 0,0,0,0,0 };
	cout << "请输入各进程的优先数：\n";
	for (int i = 0; i <= 4; i++)
		cin >> Priority[i]; //接受优先数输入
	cout << "请输入各进程的要求运行时间：\n";
	for (int i = 0; i <= 4; i++)
		cin >> RequiredRuntime[i]; //接受要求运行时间输入
	cout << endl;

	//初始化Pcb数组
	PCB Pcb[5] = {
		PCB("P1",RequiredRuntime[0],Priority[0]),
		PCB("P2",RequiredRuntime[1],Priority[1]),
		PCB("P3",RequiredRuntime[2],Priority[2]),
		PCB("P4",RequiredRuntime[3],Priority[3]),
		PCB("P5",RequiredRuntime[4],Priority[4])
	};
	
	//利用冒泡排序，按照优先数的大小，从大到小排列
	PCB temp(" ", 0, 0);
	for(int i = 0 ; i < 4 ; i++)
		for (int j = 0; j < 4 - i; j++)
		{
			if (Pcb[j].getPriority() < Pcb[j + 1].getPriority())
			{
				temp = Pcb[j];
				Pcb[j] = Pcb[j + 1];
				Pcb[j + 1] = temp;
			}
		}

	//让Pcb[i]进入队列
	for (int i = 0; i <= 4; i++)
		pcb.push(Pcb[i]);

	bool IsRequiredTime0 = true;

	while (!pcb.empty()) //不为空时循环
	{

		//每次运行，优先数减1，要求运行时间减1
		pcb.front().setPriority();
		pcb.front().setRequiredRuntime();

		//设置IsRequiredTime0
		if (pcb.front().getRequiredRuntime() == 0)
			IsRequiredTime0 = true;
		else
			IsRequiredTime0 = false;

		//队首元素出队列
		PCB top = pcb.front();
		pcb.pop();
		
		//输出当前进程信息
		cout << "本次调度中，正在运行的进程是" << top.getThreadName() << endl;

		//若要求运行时间不等于0，则将它重新加入就绪队列；否则，将状态改为“结束”，不再入队
		if (!IsRequiredTime0)
			pcb.push(top);
		else
			top.setState();

		//重新对就绪队列进行按优先数从大到小排列
		int count = pcb.size();//当前就绪队列中的进程数

		PCB* temp_list = new PCB[count];//temp_list保存pcb队列中的数据，便于后面按优先数从大到小进行重排
		for (int i = 0; i <= count - 1; i++)
		{
			temp_list[i] = pcb.front();
			pcb.pop();
			pcb.push(temp_list[i]);
		}
		
		//对temp_list按优先数从大到小进行重排，便于后面重新进入pcb队列
		for (int i = 0; i < count - 1; i++)
			for (int j = 0; j < count - 1 - i; j++)
			{
				if (temp_list[j].getPriority() < temp_list[j + 1].getPriority())	//使用 < 可以保证，当已调度的进程
																											//的优先数和就绪队列中其他进程的
																											//优先数的最大值相同时，该进程放
																											//到后面，而不是队首
				{
					temp = temp_list[j];
					temp_list[j] = temp_list[j + 1];
					temp_list[j + 1] = temp;
				}
			}
		
		//temp_list重新进入pcb队列

		//先清空pcb队列
		while (!pcb.empty())
		{
			pcb.pop();
		}
		//再将已排序的temp_list入队
		for (int i = 0; i <= count - 1; i++)
		{
			pcb.push(temp_list[i]);
		}

		//输出就绪队列信息
		if (!pcb.empty())
		{
			cout << "调度后，就绪队列为：" << endl;
			for (int i = 0; i <= pcb.size() - 1; i++)
			{
				cout << temp_list[i].getThreadName() << " ";
			}
			cout << endl << "———————————————" << endl;
		}
		else
			cout << "调度后，就绪队列为空" << endl;

		//释放temp_list
		delete[] temp_list;
	}
	//退出循环即意味着调度结束
	cout << "———————————————" << endl << "调度结束" << endl;
	return 0;
}