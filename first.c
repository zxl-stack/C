#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// 定义点的结构体
typedef struct {
    double x;
    double y;
}point;
// 函数声明
void RandomPoints(point P[], int n);// 生成随机点
void QuickSort_X(point s[],int low,int high);//快排一趟排序(X轴)
void QuickSort_Y(point s[],int low,int high);//快排一趟排序(Y轴)
double Closest(point S[], point *Q,int low,int high);//函数实现找出最近点对
int main(){
    int size;
    printf("随机生成横纵坐标值均在1-10的点数个数:");
    scanf("%d",&size);
    int low=0;
    int high=size-1;
    point *S=malloc(sizeof(point)*size);
    point Q[2];
// 生成随机点
    RandomPoints(S,size);
    QuickSort_X(S,low,high);
    double MAX=Closest(S,Q,low,high);
    printf("\n");
    printf("一对最近点对分别为:(%.2f,%.2f),(%.2f,%.2f)\n",Q[0].x,Q[0].y,Q[1].x,Q[1].y);
    printf("最短的距离为:%.2f",MAX);
    free(S);
    return 0;
}

// 生成随机点
void RandomPoints(point P[], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        P[i].x = (double)rand() / RAND_MAX * 9.0 + 1;
        P[i].y = (double)rand() / RAND_MAX * 9.0 + 1;
        printf("(%.2f,%.2f)",P[i].x,P[i].y);
    }
}
// 计算两点之间的距离
double Distance(point a,point b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

//快排一趟排序(X轴)
 int Partition_X(point S[],int low,int high)
    {
        int i=low,j=high;
        while(i<j)
        {
            while(i<j&&S[i].x<=S[j].x) j--;
        if(i<j)
        {
            point temp=S[i];S[i]=S[j];S[j]=temp;
            i++;
        }
        while(i<j&&S[i].x<=S[j].x) i++;
        if (i<j)
        {
            point temp=S[i];S[i]=S[j];S[j]=temp;
            j--; 
        }
    }
        return i;
    }
    void QuickSort_X(point s[],int low,int high)
    {
        int pivot;
        if(low<high)
        {
            pivot=Partition_X(s,low,high); 
            QuickSort_X(s,low,pivot-1);
            QuickSort_X(s,pivot+1,high);
        }
    }
 //快排一趟排序(Y轴)
 int Partition_Y(point S[],int low,int high)
 {
     int i=low,j=high;
     while(i<j)
     {
         while(i<j&&S[i].y<=S[j].y) j--;
     if(i<j)
     {
         point temp=S[i];S[i]=S[j];S[j]=temp;
         i++;
     }
     while(i<j&&S[i].y<=S[j].y) i++;
     if (i<j)
     {
         point temp=S[i];S[i]=S[j];S[j]=temp;
         j--; 
     }
 }
     return i;
 }
 void QuickSort_Y(point S[],int low,int high)
 {
     int pivot;
     if(low<high)
     {
         pivot=Partition_Y(S,low,high); 
         QuickSort_Y(S,low,pivot-1);
         QuickSort_Y(S,pivot+1,high);
     }
 }
//函数实现找出最近点对
double Closest(point S[], point *Q,int low,int high)
{
    double d1,d2,d3,d;
    int mid,i,j,index;
    point *P=malloc(sizeof(point)*(high-low+1));//存放点集合P1和P2；
    point Q1[2],Q2[2];
    if(high-low==1)
    {
        Q[0]=S[low];
        Q[1]=S[high];
        return Distance(S[low],S[high]);
    }
        
    if(high-low==2)
    {
        d1=Distance(S[low],S[low+1]);
        d2=Distance(S[low+1],S[high]);
        d3=Distance(S[low],S[high]);
        if((d1<d2)&&(d1<d3))
        {
            Q[0]=S[low];
            Q[1]=S[low+1];
            return d1;
        }
        else if((d2<d3))
        {
            Q[0]=S[low+1];
            Q[1]=S[high];
            return d2;
        }
           
        else
        {
            Q[0]=S[low];
            Q[1]=S[high];
            return d3;
        }
            

    }
    mid =(low+high)/2;
    d1=Closest(S,Q1,low,mid);
    d2=Closest(S,Q2,mid+1,high);
    if(d1<=d2)
    {
        d=d1;
        Q[0]=Q1[0];
        Q[1]=Q1[1]; 
    } 
    else
    {
        d=d2;
        Q[0]=Q2[0];
        Q[1]=Q2[1]; 
    } 
    index=0;
    for(i=mid;(i>=low)&&(S[mid].x-S[i].x<d);i--)
        P[index++]=S[i];
    for(i=mid+1;(i<=high)&&(S[i].x-S[mid].x<d);i++)
        P[index++]=S[i];
    QuickSort_Y(P,0,index-1);
    for(i=0;i<index;i++)
    {
        for(j=i+1;j<index;j++)
        {
            if(P[j].y-P[i].y>=d)
                break;
            else{
                d3=Distance(P[i],P[j]);
                if(d3<d)
                {
                    Q[0]=P[i];
                    Q[1]=P[j];
                    d=d3; 
                } 
            }
        }

    }
    free(P);
    return d;
}