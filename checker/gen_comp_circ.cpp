#include<cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
using namespace std;
int max_wire = 0;
//returns output wire
int add_gate(int in1,int in2,int c0,int c1,int c2,int c3)
{
	max_wire++;
	printf("%d gate arity 2 operator L %d %d %d %d inputs [ %d %d ]\n",max_wire,c0,c1,c2,c3,in1,in2);
	return max_wire;
}
int solver(int n,vector<int> &B, vector<int> &C)
{
	if(n==1)
		return C[0];
	vector<int> curB,curC;
	for(int i=0; i<n/2; i++)
	{
		int t1 = add_gate(B[2*i],C[2*i+1],1,0,0,0);
		curC.push_back(add_gate(t1,C[2*i],0,1,1,0));
		curB.push_back(add_gate(B[2*i],B[2*i+1],1,0,0,0));
	}
	if(n%2==1) //deal with last one
	{
		fprintf(stderr,"Not supported!");
		exit(0);
		//unsupported yet
	}
	//calculate recursively
	return solver(n/2,curB,curC);
}
int main()
{
	freopen("compare_SHDLcirc.txt","w",stdout);
	int d = 32; //should be a power of 2
	
	max_wire = 2 * d - 1;
	for(int i=0; i<2*d; i++)
		printf("%d input\n",i);
	//compute init B and C
	vector<int> B,C;
	for(int i=0; i<d; i++)
	{
		C.push_back(add_gate(i,i+d,0,1,-1,0));
		B.push_back(add_gate(i,i+d,2,-1,-1,1));
	}
	int out = solver(d,B,C);
	printf("outputs %d\n",out);
	return 0;
}
