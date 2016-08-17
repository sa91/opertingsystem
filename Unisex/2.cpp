#include<bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;
pthread_t threads[1245678];
pthread_mutex_t lock;
pthread_cond_t  con = PTHREAD_COND_INITIALIZER;
int type_inn;
int counter;
int INN_sz;
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
typedef struct a{
	int ent_t,time;
	char name[125];
}sol;
queue <sol> q1,q2;
char name[125];
int done;
void *create(void *arg)
{
	int type,time,i=0;
	while(1)
	{
		sol c;
		scanf("%d %s %d",&type,c.name,&c.time);
		if(type==0)
			c.ent_t=i,q1.push(c);
		else if(type ==1)
			c.ent_t=i,q2.push(c);
		else
			done =1;
				i++;
	}
}
void *thread(void *args)
{
	int type;
	sol s =*((sol *)args);
	struct timespec tim, tim2;
	tim.tv_sec = s.time;
	tim.tv_nsec = 50;
	pthread_mutex_lock(&lock);
	if(counter<INN_sz)
		counter++;
		type=thread_type;
	printf("Entering INN:id of Thread %d and Thread_type %d and Thread Name %s\n",s.ent_t,type_inn,s.name);
		cout.flush();
	pthread_cond_signal(&con);
	pthread_mutex_unlock(&lock);
	if(nanosleep(&tim , &tim2) < 0 )   
	{
		printf("Nano sleep system call failed \n");
	}
	pthread_mutex_lock(&lock);
	printf("Exiting INN:id of Thread %d and Thread_type %d and Thread Name %s\n",s.ent_t,type,s.name);
		counter--;
	pthread_mutex_unlock(&lock);
}
void push(sol s)
{
	pthread_mutex_lock(&lock);
	Pthread_create(threads+counter,thread,&s);
	pthread_cond_wait(&con, &lock);
	pthread_mutex_unlock(&lock);
	return ;
}
int main()
{
	sol s;
	int sz,i;
	printf("Size of INN : ");
	scanf("%d",&INN_sz);
	done=0;
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\n mutex init failed\n");
		cout.flush();
		return 1;
	}
	pthread_t creator;
	Pthread_create(&creator,create,NULL);
	int vac;type_inn=-1;
	counter=0;
	while(!done)
	{
		if(counter<INN_sz)vac=1;
		else vac=0;
		if(counter==0)
		{
			type_inn=-1;
		}
		if(!vac)
			continue;
		if(q1.empty() && q2.empty())
			continue;
//		printf("%d %d",q1.empty(),q2.empty());
		if(!(q1.empty()) && q2.empty())
		{
			sol s=q1.front(); 
			if((type_inn==1||type_inn==-1)&& vac){
				type_inn=0;
				q1.pop();
				push(s);
			}
				continue;
		}
//		printf("%d %d",q1.empty(),q2.empty());
		if((q1.empty()) && !(q2.empty()))
		{
			sol s=q2.front(); 
			if((type_inn==1 || type_inn==-1)&& vac){
				type_inn=1;
				q2.pop();
				push(s);
			}
					continue;
			}
//		printf("%d %d",q1.empty(),q2.empty());
			sol s0=q1.front();sol s1=q2.front();
			if(s0.ent_t<s1.ent_t)
			{
				q1.pop();
				if(type_inn==0)
					push(s0);	
				else if(type_inn==-1){
					push(s0);type_inn=0;
				}
				continue;
			}
			q2.pop();
			if(type_inn==1)
				push(s1);
			else if(type_inn==-1)
				push(s1);type_inn=1;

		}
	for(int j=0;j<counter;j++)  
		Pthread_join(threads[j]);
	return 0;
}


