//可变分区管理方式下，采用首次适应算法实现主存分配和回收
//使用空闲分区链

#include <iostream>
using namespace std;

constexpr auto MaxLength = 128;//最大内存空间为128 KB
int function = 0;//function选择功能，0退出，1申请空间，2释放空间

struct FreeStorage
{
	int index;//分区号——表示是第几个空闲分区
	int address;//起始地址——指出空闲区的起始地址
	int length;//长度——一个连续空闲区的长度
	bool flag;//使用状态，true已占用，false未占用
};
typedef struct DNode//双向链表结点
{
	FreeStorage data;//数据域
	DNode* prior;//指针域，前向
	DNode* next;//指针域，后向
}*DLinkList;

DLinkList front = new DNode, rear = new DNode;//双向链表的首尾指针，其中front为首结点，rear为尾部

bool first_fit(int id, int requiredLength);
void allocation();
void free();
void display();

int main()
{
	//初始化
	front->prior = NULL;
	front->next = rear;

	rear->prior = front;
	rear->next = NULL;

	front->data.length = 0;

	rear->data.index = 0;
	rear->data.address = 0;
	rear->data.length = MaxLength;
	rear->data.flag = 0;

	while (true)
	{
		cout << "请选择功能，输入1申请空间，输入2释放空间，输入0退出" << endl;
		cin >> function;
		while (function != 0 && function != 1 && function != 2)
		{
			cout << "输入无效，请重试！" << endl;
			cin >> function;
		}
		switch (function)
		{
		case 1:
			allocation();
			display();
			break;
		case 2:
			free();
			display();
			break;
		case 0:
			exit(0);
			break;
		}
	}
}
bool first_fit(int id, int requiredLength)
{
	DNode* p = front->next;
	DLinkList newSpace = new DNode;//新建一个空闲区，供当空闲区大于所需空间时使用
	//赋值
	newSpace->data.index = id;
	newSpace->data.length = requiredLength;
	newSpace->data.flag = true;//表示已经占用
	while (p != NULL)//在空闲区列表中从低地址向高地址查找
	{
		if (p->data.flag == false && p->data.length == requiredLength)
		{
			p->data.index = id;
			p->data.flag = true;
			delete newSpace;//清空新创建的空闲区
			return true;
			break;
		}
		else if (p->data.flag == false && p->data.length > requiredLength)
		{
			newSpace->next = p;
			newSpace->prior = p->prior;
			newSpace->data.address = p->data.address;
			p->prior->next = newSpace;
			p->prior = newSpace;
			p->data.address = newSpace->data.address + newSpace->data.length;
			p->data.length -= requiredLength;
			return true;
			break;
		}
		p = p->next;
	}
	return false;
}
void allocation()
{
	int id = 0, requiredLength = 0;
	cout << "请输入要申请的分区号和申请的内存大小（单位：KB）：\n";
	cin >> id >> requiredLength;
	if (first_fit(id, requiredLength))
	{
		cout << "内存分配成功！" << endl;
	}
	else
	{
		cout << "内存分配失败！原因：空间不足。" << endl;
	}
}
void free()
{
	DNode* q = front->next;
	int id = 0;
	cout << "请输入要释放分区号：";
	cin >> id;
	while (q != NULL)
	{
		if (q->data.index == id)
		{
			q->data.index = 0;
			q->data.flag = false;
			if (q->next == NULL && q->prior == front)//如果之前恰好一次性分配完
			{
				q = front;
				q->prior = NULL;
			}

			//注：如果内存恰好分配完，那么如何回收最后一个占用区至关重要，分为最先回收和最后回收。因为
			//       最后一个占用区之后不存在空闲区，此时q->next=NULL
			else if (q->prior->data.flag == false && q->next == NULL || q->prior->data.flag == false && q->next->data.flag != false)//前空后不空
			{
				q->prior->data.length += q->data.length;
				if (q->next)
				{
					q->prior->next = q->next;
					q->next->prior = q->prior;
				}
				else //最后回收
				{
					q->prior->next = q->next;
				}
			}
			else if (q->prior->data.flag != false && q->next == NULL || q->prior->data.flag != false && q->next->data.flag == false)//前不空后空
			{
				if(q->next == NULL) //最先回收
				{
					q = q->next;
				}

				else if (q->next->next)
				{
					q->data.length += q->next->data.length;
					q->next->next->prior = q;
					q->next = q->next->next;
				}
				else
				{
					q->data.length += q->next->data.length;
					q->next = q->next->next;
				}
			}
			else if (q->prior->data.flag == false && q->next->data.flag == false)//前后都空，一起合并
			{
				q->prior->data.length += q->data.length + q->next->data.length;
				if (q->next->next)
				{
					q->next->next->prior = q->prior;
					q->prior->next = q->next->next;
				}
				else
					q->prior->next = q->next->next;
			}
			break;
		}
		q = q->next;
	}
	q = front->next;
}
void display()
{
	DNode* display = front->next;
	{
		cout << "\n";
		cout << "以下是内存分配信息：" << endl;
		while (display != NULL)
		{
			//输出分区号
			cout << "分区号：";

			if (display->data.index == 0)//当index为0时，说明是空闲区
				cout << "空闲区" << endl;
			else
				cout << display->data.index << endl;

			cout << "分区起始地址：" << display->data.address << endl;
			cout << "分区大小：" << display->data.length << endl;
			cout << "分区状态：";
			if (display->data.flag == true)
				cout << "已分配" << endl;
			else
				cout << "空闲" << endl;

			cout << endl;
			display = display->next;
		}
	}
	cout << "——————————————————————————" << endl;
}