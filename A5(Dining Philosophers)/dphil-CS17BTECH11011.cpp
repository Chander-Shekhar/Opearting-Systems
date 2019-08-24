#include<iostream>
#include<thread>
#include<ctime>
#include<unistd.h>
#include<string>
#include<chrono>
#include<random>
#include<fstream>
#include<atomic>
#include<mutex>
#include<pthread.h>

using namespace std;

typedef enum{hungry, eating, thinking} state;
int h;
long long n;
double t_CS, t_RS;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

struct phil{
	state status = thinking;
	pthread_cond_t self = PTHREAD_COND_INITIALIZER;
};

FILE *out_file = fopen("Phil-log.txt","w");
double total_waiting_time = 0;
time_t max_waiting_time = -1;
phil *philosopher;

string getTime(time_t inp_time)		//Gets time in the hh:mm:ss format from a time_t variable
{
  struct tm * time;
  time = localtime (&inp_time);
  char out_time[9];
  sprintf(out_time,"%.2d:%.2d:%.2d",time->tm_hour,time->tm_min,time->tm_sec);
  return (string) out_time;
}

void test(int i)
{
	if( philosopher[i].status == hungry &&
		philosopher[(i + 1) % n].status != eating &&
		philosopher[(i - 1 + n) % n].status != eating)
	{
		philosopher[i].status = eating;
		pthread_cond_signal(&(philosopher[i].self));
	}
}

void Pickup(int i)
{
	pthread_mutex_lock(&mtx);
	philosopher[i].status = hungry;
	test(i);
	if(philosopher[i].status != eating)
		pthread_cond_wait(&(philosopher[i].self), &mtx);
	pthread_mutex_unlock(&mtx);
}

void Putdown(int i)
{
	pthread_mutex_lock(&mtx);
	philosopher[i].status = thinking;
	test((i + 1) % n);
	test((i - 1 + n) % n);
	pthread_mutex_unlock(&mtx);
}

void* dphil(void* ptr)
{
	long long id = (long long)ptr;
	default_random_engine gen1, gen2;
	exponential_distribution<double> dist_CS(1.0/t_CS);
	exponential_distribution<double> dist_RS(1.0/t_RS);
	for(int i=0; i<h; i++)
	{
	    auto t1 = chrono::system_clock::now();
		time_t reqEnterTime = chrono::system_clock::to_time_t(t1);
		fprintf(out_file, "%dth eat request by thread %lld at %s \n", i+1, id+1, getTime(reqEnterTime).c_str());
		Pickup(id);
		//Critical Section
		auto t2 = chrono::system_clock::now();
    	time_t EnterTime = chrono::system_clock::to_time_t(t2);
    	auto timeTaken = chrono::duration_cast<chrono::microseconds>(t2 - t1);
    	total_waiting_time += timeTaken.count();
    	max_waiting_time = max(max_waiting_time, timeTaken.count());
    	fprintf(out_file , "%dth CS entry by Philosopher Thread %lld at %s\n", i+1, id+1, getTime(EnterTime).c_str());
    	sleep(dist_CS(gen1)); //Simulation of Eating.
    	auto t3 = chrono::system_clock::now();
    	time_t exitTime = chrono::system_clock::to_time_t(t3);
    	fprintf(out_file , "%dth CS exit by Philosopher thread %lld at %s\n", i+1, id+1, getTime(exitTime).c_str());
    	//End of Critical Section
    	Putdown(id);
    	sleep(dist_RS(gen2)); //Simulation of Thinking.

	}
}

int main()
{
	FILE *avg = fopen("Time.txt","w");
	cin >> n >> h >> t_CS >> t_RS;
	pthread_t Phil[n];
	philosopher = (phil *) malloc(sizeof(phil) * n);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for(int i=0;i<n;i++)
		pthread_create(Phil+i, &attr, dphil,(void*)(intptr_t)i);
	for(int i=0;i<n;i++)
		pthread_join(Phil[i],NULL);
	fprintf(avg,"Avg time taken by Philosophers = %lf microseconds.\n",total_waiting_time/(n*h));
	fprintf(avg,"Worst time taken by Philosophers = %ld microseconds.\n",max_waiting_time);
	fclose(out_file);
	fclose(avg);
}
