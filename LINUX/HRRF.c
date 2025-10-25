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
void readJobdata();
int findrearlyjob(job jobs[],int count);
void calculateResponseRatio(int current_time);
int findHighestResponseRatioJob(int current_time);
void HRRN();


int main()
{  
	initial_jobs(); 
	readJobdata(); 
	HRRN();
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

// 计算响应比
void calculateResponseRatio(int current_time) {
    for (int i = 0; i < quantity; i++) {
        if (jobs[i].visited == 0 && jobs[i].reach_time <= current_time) {
            // 响应比 = (等待时间 + 服务时间) / 服务时间
            jobs[i].excellent = 1.0 + (current_time - jobs[i].reach_time) / (float)jobs[i].need_time;
        }
    }
}

// 查找响应比最高的作业
int findHighestResponseRatioJob(int current_time) {
    int highestRatioJob = -1;
    float highestRatio = -1.0;
    
    for (int i = 0; i < quantity; i++) {
        // 只考虑已到达且未执行的作业
        if (jobs[i].visited == 0 && jobs[i].reach_time <= current_time) {
            if (highestRatioJob == -1 || jobs[i].excellent > highestRatio) {
                highestRatio = jobs[i].excellent;
                highestRatioJob = i;
            }
        }
    }
    
    return highestRatioJob;
}

// HRRN - 高响应比优先算法
void HRRN() {
    int i;
    int current_time = 0; // 当前总执行时间
    int selected_job;
    int total_waitime = 0; // 当前总等待时间
    int total_roundtime = 0; // 总周转时间
    
    // 获取最早到达的作业
    int earliest_job = findrearlyjob(jobs, quantity);
    if (earliest_job == -1) return; // 没有作业
    
    // 输出作业流
    printf("\n\nHRRN算法作业流\n");
    printf("------------------------------------------------------------------------\n"); 
    printf("\tjobID\treachtime\tstarttime\twaittime\troundtime\tresponseratio\n");
    
    current_time = jobs[earliest_job].reach_time;
    
    // 每次循环找出响应比最高的作业并打印相关信息
    for (i = 0; i < quantity; i++) {
        // 计算当前时间点所有已到达作业的响应比
        calculateResponseRatio(current_time);
        
        // 查找响应比最高的作业
        selected_job = findHighestResponseRatioJob(current_time);
        
        if (selected_job == -1) {
            // 如果没有可执行的作业，时间前进到下一个作业到达的时间
            selected_job = findrearlyjob(jobs, quantity);
            current_time = jobs[selected_job].reach_time;
            calculateResponseRatio(current_time);
        }
        
        // 设置作业开始时间
        jobs[selected_job].start_time = current_time;
        
        // 计算等待时间
        jobs[selected_job].wait_time = current_time - jobs[selected_job].reach_time;
        
        // 输出作业信息
        printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\t%-4.2f\n", 
               jobs[selected_job].number, 
               jobs[selected_job].reach_time, 
               jobs[selected_job].start_time, 
               jobs[selected_job].wait_time, 
               jobs[selected_job].wait_time + jobs[selected_job].need_time,
               jobs[selected_job].excellent);
        
        // 标记作业为已访问
        jobs[selected_job].visited = 1;
        
        // 更新当前时间
        current_time += jobs[selected_job].need_time;
        
        // 累计总等待时间和总周转时间
        total_waitime += jobs[selected_job].wait_time;
        total_roundtime += jobs[selected_job].wait_time + jobs[selected_job].need_time;
    }
    
    printf("总等待时间:%-8d 总周转时间:%-8d\n", total_waitime, total_roundtime); 
    printf("平均等待时间: %4.2f 平均周转时间: %4.2f\n", 
           (float)total_waitime / quantity, 
           (float)total_roundtime / quantity);
}
