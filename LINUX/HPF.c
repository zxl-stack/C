#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
//最大作业数量
const int MAXJOB=50;
//作业的数据结构
typedef struct node
{
	int number;//作业号        
	int reach_time;//作业抵达时间
	int need_time;//作业的执行时间
	int privilege;//作业优先权
	float excellent;//响应比
	int start_time;//作业开始时间
	int wait_time;//等待时间
	int visited;//作业是否被访问过
	bool isreached;//作业是否抵达执行队列
}job;
job jobs[50];
int quantity;//作业数量

void initial_jobs();
int findrearlyjob(job jobs[],int count);
void readJobdata();
int findhighestprivilege(job jobs[], int count, int current_time);
void HPF();

int main()
{  
	initial_jobs(); 
	readJobdata(); 
    HPF();
	system("pause");
	return 0;
}

//初始化作业序列
void initial_jobs()
{
	int i;
	for(i=0;i<MAXJOB;i++)
	{
		jobs[i].number=0;//作业号 
		jobs[i].reach_time=0;
		jobs[i].privilege=0;
		jobs[i].excellent=0;
		jobs[i].start_time=0;
		jobs[i].wait_time=0;
		jobs[i].visited=0;
		jobs[i].isreached=false;
	}
	quantity=0;
}

//读取作业数据
void readJobdata()
{
	int num;
    //输入测试文件文件名
	printf("输入作业数量:");
    scanf("%d",&num);
	if(num==0)
	{
		printf("作业数量为0，无需调度\n");
	}
	else
	{
		//依次读取作业信息
		printf("输出作业信息\n");
		printf("---------------------------------------------------------------------\n");
		printf("\t作业号\t到达时间\t执行时间\t优先级\n");
		for(int i=0;i<num;i++)
		{
	        scanf("%d %d %d %d",&jobs[quantity].number,&jobs[quantity].reach_time,&jobs[quantity].need_time,&jobs[quantity].privilege);
			quantity++;
		}
	}
}

//查找最早到达作业，返回作业序列号，若全部到达返回-1.
int findrearlyjob(job jobs[],int count)
{
	int rearlyloc=-1;//作业序列号
	int rearlyjob=-1;//作业到达时间
	for(int i=0;i<count;i++)
	{
		if(rearlyloc==-1){
			if(jobs[i].visited==0){
			rearlyloc=i;
			rearlyjob=jobs[i].reach_time;
			}
		}
		else if(rearlyjob>jobs[i].reach_time&&jobs[i].visited==0)
		{
			rearlyjob=jobs[i].reach_time;
			rearlyloc=i;
		}
	}
	return rearlyloc;
}

// 查找当前current_time已到达未执行的最高优先级作业,若无返回-1
int findhighestprivilege(job jobs[], int count, int current_time)
{
    int highestprivilege = -1; // 最高优先级值
    int highestloc = -1;       // 最高优先级作业的位置
    
    for(int i = 0; i < count; i++)
    {
        // 如果作业已到达且未执行
        if(jobs[i].reach_time <= current_time && jobs[i].visited == 0)
        {
            // 找到第一个符合条件的作业或优先级更高的作业
            if(highestloc == -1 || jobs[i].privilege > highestprivilege)
            {
                highestprivilege = jobs[i].privilege;
                highestloc = i;
            }
        }
    }
    
    return highestloc;
}

// 优先权高者优先算法
void HPF() 
{ 
    int i; 
    int current_time = 0;      // 当前总执行时间
    int loc;
    int total_waitime = 0;     // 当前总等待时间
    int total_roundtime = 0;   // 总周转时间
    
    // 获取最早到达的作业
    loc = findrearlyjob(jobs, quantity);
    if(loc == -1) return;      // 没有作业，直接返回
    
    // 初始化当前时间为第一个作业的到达时间
    current_time = jobs[loc].reach_time;
    
    // 输出作业流
    printf("\n\nHPF算法作业流\n");
    printf("------------------------------------------------------------------------\n"); 
    printf("\tjobID\treachtime\tstarttime\twaittime\troundtime\tprivilege\n");
    
    // 每次循环找出当前时间点优先级最高的作业并执行
    for(i = 0; i < quantity; i++)
    { 
        // 查找当前时间点优先级最高的作业
        loc = findhighestprivilege(jobs, quantity, current_time);
        
        // 如果没有找到可执行的作业，时间推进到下一个作业到达的时间
        if(loc == -1)
        {
            loc = findrearlyjob(jobs, quantity);
            current_time = jobs[loc].reach_time;
        }
        
        // 计算作业的开始时间和等待时间
        if(jobs[loc].reach_time > current_time)
        {
            jobs[loc].start_time = jobs[loc].reach_time;
            current_time = jobs[loc].reach_time;
        }
        else
        {
            jobs[loc].start_time = current_time;
        }
        
        jobs[loc].wait_time = jobs[loc].start_time - jobs[loc].reach_time;
        
        // 输出作业信息
        printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n", 
               jobs[loc].number, jobs[loc].reach_time, jobs[loc].start_time, 
               jobs[loc].wait_time, jobs[loc].wait_time + jobs[loc].need_time,
               jobs[loc].privilege);
        
        jobs[loc].visited = 1;
        current_time += jobs[loc].need_time;
        total_waitime += jobs[loc].wait_time; 
        total_roundtime += jobs[loc].wait_time + jobs[loc].need_time;
    } 
    
    printf("总等待时间:%-8d 总周转时间:%-8d\n", total_waitime, total_roundtime); 
    printf("平均等待时间: %4.2f 平均周转时间: %4.2f\n", 
           (float)total_waitime/quantity, (float)total_roundtime/quantity); 
}
