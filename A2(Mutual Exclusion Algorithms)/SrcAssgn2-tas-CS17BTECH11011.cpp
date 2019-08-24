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

FILE *out = fopen("TAS-log.txt","w");
atomic_flag lock(false);
int n,k;
double lam_1,lam_2;
long long total_waiting_time = 0;
time_t max_waiting_time = -1;


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
		auto start = chrono::system_clock::now();
		time_t reqEnterTime = chrono::system_clock::to_time_t(start);
		fprintf(out, "%dth CS request at %s by thread %d\n",i+1 ,getTime(reqEnterTime).c_str(),id);
		while(atomic_flag_test_and_set_explicit(&lock, memory_order_acquire))
			;
		auto stop = chrono::system_clock::now();
		auto time_taken = chrono::duration_cast<chrono::microseconds> (stop - start);
		total_waiting_time += time_taken.count();
		max_waiting_time = max(time_taken.count(), max_waiting_time);
		time_t actEnterTime = chrono::system_clock::to_time_t(stop);
		fprintf(out, "%dth CS Entery at %s by thread %d\n",i+1 ,getTime(actEnterTime).c_str(),id);
		usleep(dist1(gen1));
		atomic_flag_clear_explicit(&lock,memory_order_release);
		auto exit = chrono::system_clock::now();
		time_t ExitTime = chrono::system_clock::to_time_t(exit);
		fprintf(out, "%dth CS Exit at %s by thread %d\n",i+1 ,getTime(ExitTime).c_str(),id);
		usleep(dist2(gen2));
	}

}

int main()
{
	cin >> n >> k;
	cin>> lam_1 >> lam_2;
	thread ths[n];
	for (int i=0;i<n;i++)
		ths[i] = thread(testCS,i);

	for (int i=0;i<n;i++)
		ths[i].join();

	cout << "Average waiting time is " << total_waiting_time / (n*k) <<" microseconds"<< endl;
	cout << "Maximum waiting time is " << max_waiting_time <<" microseconds" << endl;
}