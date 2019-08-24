#include<iostream>
#include<thread>
#include<ctime>
#include<atomic>
#include<unistd.h>
#include<string>
#include<stdio.h>
#include<random>
#include<chrono>
#include<fstream>
using namespace std;

FILE *out = fopen("Bounded_CAS-log.txt","w");
atomic<int>lock(0);
int n,k;
double lam_1,lam_2;
long long total_waiting_time = 0;
time_t max_waiting_time = -1;
atomic<bool>* wait;

string getTime(time_t inp_time)
{
  struct tm * time;
  time = localtime (&inp_time);
  char out_time[9];
  sprintf(out_time,"%.2d:%.2d:%.2d",time->tm_hour,time->tm_min,time->tm_sec);
  return (string) out_time;
}

void testCS(int i)
{
	default_random_engine gen1,gen2;
	exponential_distribution<double> dist1(1.0/lam_1);
	exponential_distribution<double> dist2(1.0/lam_2);
	int id = i;
	for (int i=0;i<k;i++)
	{
		wait[id] = true;
		bool key = false;
		auto start = chrono::system_clock::now();
		time_t reqEnterTime = chrono::system_clock::to_time_t(start);
		fprintf(out, "%dth CS request at %s by thread %d\n",i+1 ,getTime(reqEnterTime).c_str(),id);
		while(wait[id] && !key)
		{
			int x=0,y=1;
			key = lock.compare_exchange_strong(x,y);
		} 
		wait[id] = false;
		auto stop = chrono::system_clock::now();
		auto time_taken = chrono::duration_cast<chrono::microseconds> (stop - start);
		total_waiting_time += time_taken.count();
		max_waiting_time = max(time_taken.count(), max_waiting_time);
		time_t actEnterTime = chrono::system_clock::to_time_t(stop);
		fprintf(out, "%dth CS Entery at %s by thread %d\n",i+1 ,getTime(actEnterTime).c_str(),id);
		usleep(dist1(gen1));
		auto exit = chrono::system_clock::now();
		time_t ExitTime = chrono::system_clock::to_time_t(exit);
		fprintf(out, "%dth CS Exit at %s by thread %d\n",i+1 ,getTime(ExitTime).c_str(),id);
		int j = (id+1)%n;
		while((j != id) && !wait[j])
			j = (j+1) % n;
		if(j == id)
			lock = 0;
		else
			wait[j] = false;

		usleep(dist2(gen2));

	}

}

int main()
{
	cin >> n >> k;
	cin>> lam_1 >> lam_2;
	thread ths[n];
	wait = (atomic<bool>*) malloc(sizeof(atomic<bool>*) * n);
	for (int i=0;i<n;i++)
		ths[i] = thread(testCS,i);

	for (int i=0;i<n;i++)
		ths[i].join();

	cout << "Average waiting time is " << total_waiting_time / (n*k) <<" microseconds" << endl;
	cout << "Maximum waiting time is " << max_waiting_time <<" microseconds" << endl;
}