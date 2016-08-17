#include<bits/stdc++.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include <sys/wait.h>
const int N=10005;
using namespace std;
key_t key = IPC_PRIVATE;
int b[N],c[N];
int *a;
void Fork(pid_t &pid)
{
		if((pid=fork())<0){
			printf("fork error\n");
			exit(0);
		}
}
void merge(int l,int r,int m)
{
	int i,j;
	for(i=l;i<=m;i++)b[i]=a[i];
	for(i=m+1;i<=r;i++)c[i]=a[i];
	int s=l;
	i=l,j=m+1;
	while(i<=m && j<=r)
	{
		if(b[i]>c[j])
			a[s++]=c[j++];
		else
			a[s++]=b[i++];
	}
	while(j<=r)
		a[s++]=c[j++];
	while(i<=m)
		a[s++]=b[i++];
}
void ms(int l,int r)
{
	int i,j;
	if((r-l)+1<=5)
	{
		for(i=l;i<=r;i++)
		{
			int cnt=l;
			for(j=l;j<=r;j++){
				if(a[i]>a[j])	cnt++;
				else if(a[i]==a[j] && i>j)	cnt++;
			}
			b[cnt]=a[i];
		}
		for(i=l;i<=r;i++)
			a[i]=b[i];
		exit(0) ;
	}
	pid_t pid,pid2;
	int m=(l+r)/2,status;
	Fork(pid);
	Fork(pid2);
	if(pid==0){
		ms(l,m);
		exit(0);
	}
	else if(pid2==0){
		ms(m+1,r);
		exit(0);
	}
	else if(pid>0 && pid2>0){
		waitpid(pid,&status,0);
		waitpid(pid2,&status,0);
		merge(l,r,m);	
		exit(0);
	}	
}
int main()
{
	int i,n,shmid,status;
	printf("Number of Inputs : ");scanf("%d",&n);
	if(n>N-3)
		n=N-3;
	if((shmid=shmget(key, sizeof(int)*(n+3), IPC_CREAT| 0666)) < 0){
		perror("Shmget failed");
		exit(1);
	}
	if((a =(int *)shmat(shmid, 0, 0)) == (int *)-1){
		perror("Shmat failed");
		exit(1);
	}
	for(i=1;i<=n;i++)
		scanf("%d",a+i);
	pid_t pid;
	Fork(pid);
	if(pid==0){
		ms(1,n);
	}
	else 
	{
		waitpid(pid,&status,0);
		for(i=1;i<=n;i++)
			printf("%d ",a[i]);
	}
	cout<<endl;
 cout.flush();
	return 0;
}
