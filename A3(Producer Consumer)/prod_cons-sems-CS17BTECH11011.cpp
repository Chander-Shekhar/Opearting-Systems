#include<iostream>
#include<thread>
#include<semaphore.h>
#include<ctime>
#include<unistd.h>
#include<string>
#include<chrono>
#include<random>
#include<fstream>
using namespace std;

bool *buffer;
int cn_tp,cn_tc,capacity;
int in = 0,out = 0; 
double tp,tc;
sem_t mutex, full, empty;
double prod_waiting_time = 0, cons_waiting_time = 0;
FILE *out_file = fopen("semaphore_log.txt","w");

string getTime(time_t inp_time)  //Gets time in the hh:mm:ss format from a time_t variable
{
  struct tm * time;
  time = localtime (&inp_time);
  char out_time[9];
  sprintf(out_time,"%.2d:%.2d:%.2d",time->tm_hour,time->tm_min,time->tm_sec);
  return (string) out_time;
}

void producer(int id)
{
	bool next_produced = false;
	default_random_engine gen1;
	exponential_distribution<double> dist1(1.0/tp);
	for (int i = 0; i < cn_tp; i++)
	{
		sleep(dist1(gen1));
		next_produced = true;
		auto t1 = chrono::system_clock::now();
		sem_wait(&empty);
		sem_wait(&mutex);
			buffer[in] = next_produced;
			auto t2 = chrono::system_clock::now();
			time_t prodTime = chrono::system_clock::to_time_t(t2);
			auto time_taken = chrono::duration_cast<chrono::microseconds> (t2 - t1);
			prod_waiting_time += time_taken.count();
			fprintf(out_file, "%dth item produced by thread %d at %s into buffer location %d\n",i+1, id, getTime(prodTime).c_str(),in);
			in = (in + 1) % capacity;
		sem_post(&mutex);
		sem_post(&full);
	}
}

void consumer(int id)
{
	bool next_consumed = true;
	default_random_engine gen2;
	exponential_distribution<double> dist2(1/tc);
	for(int i = 0;i < cn_tc; i++)
	{
		auto t1 = chrono::system_clock::now();
		sem_wait(&full);
		sem_wait(&mutex);
			next_consumed = buffer[out];
			auto t2 = chrono::system_clock::now();
			time_t consTime = chrono::system_clock::to_time_t(t2);
			auto time_taken = chrono::duration_cast<chrono::microseconds> (t2 - t1);
			cons_waiting_time += time_taken.count();
			fprintf(out_file, "%dth item read from the buffer by the thread %d at %s from location %d\n",i+1, id, getTime(consTime).c_str(),out);
			buffer[out] = false;
			out = (out + 1) % capacity;
		sem_post(&mutex);
		sem_post(&empty);
		sleep(dist2(gen2));
	}
}

int main() 
{
	int np,nc;
	cin >> capacity >> np >> nc >> cn_tp;
	cin >> cn_tc >> tp >> tc;
	buffer = new bool[capacity];
	thread prod[np];
	thread cons[nc];
	sem_init(&mutex,0,1);
	sem_init(&full,0,0);
	sem_init(&empty,0,capacity);

	for(int i=0;i<np;i++)
		prod[i] = thread(producer,i);
	for(int i=0;i<nc;i++)
		cons[i] = thread(consumer,i);

	for(int i=0;i<np;i++)
		prod[i].join();
	for(int i=0;i<nc;i++)
		cons[i].join();
	cout << "Avg waiting time for producers = " << prod_waiting_time / (np * cn_tp) <<" microseconds"<< endl; 
	cout << "Avg waiting time for consumers = " << cons_waiting_time / (nc * cn_tc) <<" microseconds"<< endl; 
	fclose(out_file);
}