#include<iostream>
#include<bits/stdc++.h>
#include<algorithm>
#include<string>
#include<fstream>
using namespace std;

struct process{
	string process_id;
	int k;
	long long time, period,arrival_time;
	long long current_exec;
};

bool DeadlineCompare(process &a, process &b)
{
	return ((a.arrival_time + a.period) < (b.arrival_time + b.period));
}

void EDF(process *tasks, int n, long long max_time)
{
	ofstream output2_file;
	output2_file.open("EDF-Stats.txt");
	long long time =1,total_processes=0,completed=0,missed_deadlines=0;
	long long total_waiting = 0;
	long long last_time=0;
	int i;
	bool idle = false,sort_req = false;
	ofstream output1_file;
	output1_file.open("EDF-Log.txt");
	int curr_process,prev_process=1;

	for(int i=0;i<n;i++){
		output1_file<<"Process "<<tasks[i].process_id<<": processing time="<<tasks[i].time<<"; deadline:"<<tasks[i].period<<"; period:"<<tasks[i].period<<" joined the system at time 0"<<endl;
		total_processes += tasks[i].k;
	}
	sort(tasks, tasks + n, DeadlineCompare);
	while(time <= max_time)
	{
		curr_process = -1;
		for(i=0;i<n;i++)
		{
			if(tasks[i].arrival_time < time && (tasks[i].arrival_time/tasks[i].period) < tasks[i].k)
			{
				if(idle == true){
					output1_file<<"CPU is idle till time "<<time-1<<endl;
					idle = false;	
				} 
				if(prev_process != i && tasks[prev_process].current_exec !=0)
				{
					output1_file<<"Process "<<tasks[prev_process].process_id<<" is preempted by Process "<<tasks[i].process_id<<" at time "<<time-1<<". Remaining processing time:"<<tasks[prev_process].time - tasks[prev_process].current_exec<<endl;
				}    
				if((prev_process != i && tasks[i].current_exec == 0) || (prev_process == i && tasks[prev_process].current_exec == 0))
					output1_file<<"Process "<<tasks[i].process_id<<" starts execution at time "<<time<<endl;
				else if(prev_process != i && tasks[i].current_exec != 0)
					output1_file <<"Process "<<tasks[i].process_id<<" resumes its execution at time "<<time<<endl;
				curr_process = i;
				prev_process = curr_process;
				break;
			}
		}

		if(i == n) idle = true;
		if(curr_process > -1)
		{
			tasks[curr_process].current_exec++;
			if(tasks[curr_process].current_exec == tasks[curr_process].time)
			{
				output1_file<<"Process "<<tasks[curr_process].process_id<<" finishes execution at time "<<time<<endl;
				completed += 1;
				total_waiting += time - tasks[curr_process].arrival_time - tasks[curr_process].time;
				tasks[curr_process].arrival_time +=tasks[curr_process].period;
				tasks[curr_process].current_exec = 0;
				sort_req = true;
			}
		}
		for(int i=0;i<n;i++)
		{
			if(tasks[i].arrival_time + tasks[i].period <= time && (tasks[i].arrival_time/tasks[i].period) < tasks[i].k)
			{
				output1_file<<"Process "<<tasks[i].process_id<<" missed its deadline at time "<<time<<endl;
				total_waiting += time - tasks[i].arrival_time - tasks[i].current_exec;
				missed_deadlines += 1;
				tasks[i].arrival_time += tasks[i].period;
				tasks[i].current_exec = 0;
				sort_req = true;
			}
		}
		if(sort_req == true) 
		{
			sort(tasks, tasks+n, DeadlineCompare);
			sort_req = false;
		}
		time++;
	}
	output1_file.close();
	output2_file<<"Number of processes that came into the system = "<<total_processes<<endl;
	output2_file<<"Number of processes that successfully completed = "<<completed<<endl;
	output2_file<<"Number of processes that missed their deadlines = "<<missed_deadlines<<endl;
	output2_file<<"Average waiting time = "<<((double)total_waiting)/total_processes<<endl;
	output2_file.close();
}

int main()
{
	int n;
	ifstream input_file;
	input_file.open("inp-params.txt");
	long long max_time = 0;
	input_file >> n;
	process tasks[n];
	for(int i=0;i<n;i++)
	{
		input_file >> tasks[i].process_id >> tasks[i].time;
		input_file >> tasks[i].period >> tasks[i].k;
		tasks[i].arrival_time = tasks[i].current_exec = 0;
		if(max_time < (tasks[i].period * tasks[i].k)) 
			max_time = tasks[i].period * tasks[i].k;
	}
	EDF(tasks,n,max_time);

	input_file.close();
}