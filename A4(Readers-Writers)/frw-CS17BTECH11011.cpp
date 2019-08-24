#include<iostream>
#include<thread>
#include<ctime>
#include<unistd.h>
#include<string>
#include<chrono>
#include<semaphore.h>
#include<random>
#include<fstream>
#include<atomic>
using namespace std;

int kw,kr,read_count = 0;
sem_t mtx, rw_mtx, in;
double t_CS,t_RS;
double writer_waiting_time = 0, reader_waiting_time = 0;
time_t max_writer_time = -1, max_reader_time = -1;
FILE *out_file = fopen("FairRW-log.txt","w");

string getTime(time_t inp_time)		//Gets time in the hh:mm:ss format from a time_t variable
{
  struct tm * time;
  time = localtime (&inp_time);
  char out_time[9];
  sprintf(out_time,"%.2d:%.2d:%.2d",time->tm_hour,time->tm_min,time->tm_sec);
  return (string) out_time;
}

void reader(int id)
{
	default_random_engine gen1, gen2;
	exponential_distribution<double> dist_CS(1.0/t_CS);
	exponential_distribution<double> dist_RS(1.0/t_RS);
	for(int i = 0; i < kr; i++)
	{
		auto t1 = chrono::system_clock::now();
		time_t reqEnterTime = chrono::system_clock::to_time_t(t1);
		fprintf(out_file, "%dth CS request by reader thread %d at %s \n", i+1, id+1, getTime(reqEnterTime).c_str());
		sem_wait(&in);
		sem_wait(&mtx);
			read_count++;
			if(read_count == 1)
				sem_wait(&rw_mtx);
		sem_post(&mtx);
		sem_post(&in);
		auto t2 = chrono::system_clock::now();
    	time_t actEnterTime=chrono::system_clock::to_time_t(t2);
    	auto timeTaken = chrono::duration_cast<chrono::microseconds>(t2 - t1);
    	reader_waiting_time += timeTaken.count();
    	max_reader_time = max(max_reader_time, timeTaken.count());
    	fprintf(out_file , "%dth CS entry by reader thread %d at %s\n", i+1, id+1, getTime(actEnterTime).c_str());
    	usleep(dist_CS(gen1));
    	sem_wait(&mtx);
    		read_count--;
    		if(read_count == 0)
    			sem_post(&rw_mtx);
    	sem_post(&mtx);
    	auto t3 = chrono::system_clock::now();
    	time_t exit = chrono::system_clock::to_time_t(t3);
    	fprintf(out_file , "%dth CS exit by reader thread %d at %s\n", i+1, id+1, getTime(exit).c_str());
    	usleep(dist_RS(gen2));
	}

}

void writer(int id)
{
	default_random_engine gen1, gen2;
	exponential_distribution<double> dist_CS(1.0/t_CS);
	exponential_distribution<double> dist_RS(1.0/t_RS);
	for(int i = 0; i < kw; i++)
	{
		auto t1 = chrono::system_clock::now();
		time_t reqEnterTime = chrono::system_clock::to_time_t(t1);
		fprintf(out_file, "%dth CS request by writer thread %d at %s \n", i+1, id+1, getTime(reqEnterTime).c_str());
		sem_wait(&in);
		sem_wait(&rw_mtx);
			auto t2 = chrono::system_clock::now();
    		time_t actEnterTime=chrono::system_clock::to_time_t(t2);
    		auto timeTaken = chrono::duration_cast<chrono::microseconds>(t2 - t1);
    		writer_waiting_time += timeTaken.count();
    		max_writer_time = max(max_writer_time, timeTaken.count());
    		fprintf(out_file , "%dth CS entry by writer thread %d at %s\n", i+1, id+1, getTime(actEnterTime).c_str());
    		usleep(dist_CS(gen1));
    	sem_post(&rw_mtx);
    	sem_post(&in);
       	auto t3 = chrono::system_clock::now();
    	time_t exit = chrono::system_clock::to_time_t(t3);
    	fprintf(out_file , "%dth CS exit by writer thread %d at %s\n", i+1, id+1, getTime(exit).c_str());
    	usleep(dist_RS(gen2));
	}
}

int main() 
{
	int nw,nr;
	FILE *avg = fopen("Average_time.txt","a");
	cin >> nw >> nr >> kw;
	cin >> kr >> t_CS >> t_RS;
	thread write[nw];
	thread read[nr];
	
	sem_init(&mtx, 0, 1);
	sem_init(&rw_mtx, 0, 1);
	sem_init(&in, 0 ,1);

	for(int i=0;i<nw;i++)
		write[i] = thread(writer,i);
	for(int i=0;i<nr;i++)
		read[i] = thread(reader,i);

	for(int i=0;i<nw;i++)
		write[i].join();
	for(int i=0;i<nr;i++)
		read[i].join();
	fprintf(avg, "FairRW:\n");
	fprintf(avg, "Avg waiting time for writers = %f microseconds.\n", writer_waiting_time / (nw * kw) );
	fprintf(avg, "Avg waiting time for readers = %f microseconds.\n\n", reader_waiting_time / (nr * kr) );

	cout << "Worst waiting time for writer threads = " << max_writer_time << " microseconds"<< endl;
	cout << "Worst waiting time for reader threads = " << max_reader_time << " microseconds"<< endl;
	fclose(out_file);
	fclose(avg);
}
