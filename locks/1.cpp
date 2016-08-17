#include<bits/stdc++.h>
#include<pthread.h>
#include<semaphore.h>
using namespace std;
#define MP make_pair 
#define F first 
#define S second 
typedef pair<int,int > II;
const int MAXN=1002;
const int MAXSZ=100002;
sem_t written[MAXN];
sem_t read[MAXN];
int val[MAXSZ];
int quit=0;
void *reader(void *args)
{
	II a=*((II *)args);
	int i=a.F;
	int sz=a.S;
	for(int j=0;j>-1;j++)
	{
		sem_wait(&written[i]);
		printf("Thread %d : %d\n",i,val[j%sz]);
		cout.flush();
		sem_post(&read[i]);
	}
}
void *writter(void *args)
{
	II a=*((II *)args);
	int n=a.F;
	int sz=a.S;
	for(int j=0;j>=0;j++)
	{
		for(int i=0;i<n;i++)
			sem_wait(&read[i]);
		cin>>(val[j%sz]);
		if(val[j%sz]==-1){
			quit=1;
			break;
		}
		for(int i=0;i<n;i++)
			sem_post(&written[i]);
	}
}
void Pthread_create(pthread_t* thread,void *func(void *),void *args)
{
	if(pthread_create(thread,NULL,func,args)!=0){
		cout<<"Thread not created properly";
		cout.flush();
		exit(0);
	}
}
void Pthread_join(pthread_t thread)
{
	if(pthread_join(thread,NULL)){
		cout<<"Thread  cannot joined properly";
		cout.flush();
		exit(0);
	}
}
pthread_t threads[MAXN];
pthread_t writer;
II* M_P(int f,int s)
{
	II *a=NULL;
	a=new II;
	*a=MP(f,s);
	return a;
}
int main()
{
	int n,sz,i;
	cout<<"Size of Buffer: ";
	cin>>sz;
	cout<<"Number of Threads: ";
	cin>>n;
	for(i=0;i<n;i++)
	{
		sem_init(read+i,0,n);
		sem_init(written+i,0,0);
	}
	Pthread_create(&writer,writter,M_P(n,sz));
	for(i=0;i<n;i++)
		Pthread_create(threads+i,reader,M_P(i,sz));
	Pthread_join(writer);
	if(quit)
		return 0;
	for(i=0;i<n;i++)  
		Pthread_join(threads[i]);
	return 0;
}
