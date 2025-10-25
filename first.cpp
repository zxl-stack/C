#include <stdio.h>
#include <time.h>
#include<stdlib.h>
using namespace std;

struct Pair{
	
	float d;  //两点间的距离'  
	float d1, d2;   //一维上的点对坐标 
};

float random_number(){
	
	float result = rand() % 10000;
	return result * 0.01;
}

int input(float s[], int length) {
	
	srand((int)time(0));
	printf("点集在X轴上的坐标: ");
	for(int i = 0; i < length; i++){
		
		s[i] = random_number();
		cout << s[i] << " ";
	}

}

float max(float s[], int l, int r){ //返回s[]中的最大值

	float s_max = s[l];
	for(int i = l + 1; i < r; i++){
		
		if(s_max < s[i]){
			
			s_max = s[i];
		}
	}
	
	return s_max;	
}

float min(float s[], int l, int r){ //返回s[]中的最大值

	float s_min = s[l];
	for(int i = l + 1; i < r; i++){
		
		if(s_min > s[i]){
			
			s_min = s[i];
		}
	}
	
	return s_min;	
}

int partition(float a[], int l, int r, float x){
	
	int i = l;
	int j = r + 1;

	while(true){
		while(a[++i] < x && i < r) ;
		while(a[--j] > x) ;
		if(i >= j)
			break;
		swap(a[i], a[j]);
	}
	a[l] = a[j];
	a[j] = x;
	return j;
}

Pair cpair(float s[], int l, int r){
	
	Pair min_d = {99999, 0, 0};//最短距离
	
	if(r-l<1) 
		return min_d;
	
	float m1 = max(s, l, r);
	float m2 = min(s, l, r);
	
	float m = (m1 + m2) / 2;  //找出点集中的中位数 
	
	int j = partition(s, l, r, m);  //以m为基准进行分割
	//递归
	Pair d1 = cpair(s, l, j);
	Pair d2 = cpair(s, j + 1, r);
	 
	float p = max(s, l, j);
	float q = min(s, j + 1, r);
	
	if(d1.d < d2.d){
		
		if((q-p) < d1.d){
			
			min_d.d = (q-p);
			min_d.d1 = q;
			min_d.d2 = p;
			return min_d;
		}else{
			
			return d1;
		}
		
	}else{
		
		if((q-p) < d2.d){
			
			min_d.d = (q-p);
			min_d.d1 = q;
			min_d.d2 = p;
			return min_d;
		}else{
			
			return d2;
		}
	}
	
}

int main(){
	
	int length;
	cout << "请输入点的数目：";
	cin >> length;
	float s[length];
	input(s, length);  //构造s[]集合 
	
	Pair d;
	d = cpair(s, 0, length-1);
	cout << endl << "最近点对坐标为: (" << d.d1 << ", " << d.d2 << ")";
	cout << endl << "这两点距离为：" << d.d << endl;
	
	return 0;
}	