#include <iostream>
#include <process.h>  
using namespace std;

//定义位示图的数据结构
struct Bitmap
{
    int data;//1或0，为1则占用，为0则空闲
    int index;//所属文件的标识符，默认为-1
};

Bitmap bitmap[8][8];//定义位示图数组

//显示位示图
void display()
{
    cout << "位示图如下（0表示可用）：" << endl;
    for (int i = 0; i <= 7; i++)
    {
        for (int j = 0; j <= 7; j++)
            cout << bitmap[i][j].data << " ";
        cout << endl;
    }
}
//分配位示图
void allocation()
{
    bool flag = true;
    int numberOfBlocks;//所需物理块数
    int numberOfBusyBlocks = 0;//已经占用的块数
    int index;
    cout << "请输入所需的物理块数和文件名（从0开始的数字）：" << endl;
    cin >> numberOfBlocks >> index;
    //计算已占用的块数
    for (int i = 0; i <= 7; i++)
        for (int j = 0; j <= 7; j++)
        {
            numberOfBusyBlocks += bitmap[i][j].data;
        }
    //判断是否有重名
    for (int i = 0; i <= 7; i++)
        for (int j = 0; j <= 7; j++)
        {
            if (bitmap[i][j].index == index)
            {
                flag = false;
                break;
            }
        }

    if (flag == false)
        cout << "文件名重复，请重试！" << endl;
    else if (64 - numberOfBusyBlocks < numberOfBlocks)
        cout << "磁盘空间不足，分配失败！" << endl;
    else
    {
        for(int i=0;i<=7;i++)
            for(int j=0;j<=7;j++)
                if (numberOfBlocks != 0&&bitmap[i][j].data == 0)
                {
                    cout << "已分配：第" << i << "个柱面, " << "第" << j / 4 << "个磁道, " << "第" << j%4 << "个物理记录。" << endl;
                    bitmap[i][j].data = 1;
                    bitmap[i][j].index = index;
                    numberOfBlocks--;
                }
        cout << "分配成功！" << endl;
    }
}
//回收
void recycle()
{
    int recycle_index;
    bool flag = false;//标志回收是否成功，默认为false，回收成功后改为true
    cout << "请输入要回收的文件索引：" << endl;
    cin >> recycle_index;
    for (int i = 0; i <= 7; i++)
        for (int j = 0; j <= 7; j++)
            if (bitmap[i][j].index == recycle_index)
            {
                bitmap[i][j].data = 0;
                bitmap[i][j].index = -1;
                cout << "回收成功！字节号是" << i << "，位数是" << j / 4 * 4 + j % 4 << "。" << endl;
                flag = true;
            }
    if (flag == false)
        cout << "回收失败，输入的文件名不存在！" << endl;
}
int main()
{
    int choice; //选择功能时使用
    //初始化
    for (int i = 0; i <= 7; i++)
        for (int j = 0; j <= 7; j++)
        {
            bitmap[i][j].data = 0;
            bitmap[i][j].index = -1;
        }
    while (true)
    {
        cout << "请输入选择：" << endl;
        cout << "1--分配，2---回收，3--显示位示图，0--退出\n";
        cin >> choice;
        switch (choice)
        {
            case 1: allocation(); break;
            case 2: recycle(); break;
            case 3: display(); break;
            case 0:exit(0);
            default:printf("错误选择！"); break;
        }
    }
    return 0;
}