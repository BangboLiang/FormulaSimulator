#include <iostream>
#include <vector>
#include <cstdlib>
#define PRINT_MODE 1 //1 For print on the terminal, 0 for output to the file.

//cmd parameters:
int workernum = 4;
int forwardtime = 1;
int backwardtime = 2;
int batchsize = 4;
int simulatetime = 100;


//pre clarifies
class job;
class worker;
void OneJobIsOver(int jobid);
void PassJob2NextWorker(job& jb, int lastworkerid);


//global variables
std::vector<worker> workerlist;
int GlobalTime;
int ActiveJobs;
int NextJobid;

class job
{
public:
	int arrivedtime;
	int jobid;
	bool isbackward;
	int trainedtime;
};

class worker
{
public:
	int workerid;				//worker id
	std::vector<job> myjoblist;	//job list
	bool lastworker;			//whether this worker is the last one, if so, job will change direction in this worker.
	void get_a_new_job(job& jb){
		if (PRINT_MODE){
			std::cout << "worker:" << workerid << " got a new job " << jb.jobid << " at time " << GlobalTime \
			<< " is backward " << jb.isbackward << " and trainedtime " << jb.trainedtime << "\n";
		}
		myjoblist.push_back(jb);
	}
	int parse_job(job& jb){		//if a job is finished in this worker, return jobid, if not, return 0
		if (PRINT_MODE){
			std::cout << "worker:" << workerid << " is parsing job " << jb.jobid << " at time " << GlobalTime << "\n";
		}
		jb.trainedtime++;
		if( !jb.isbackward ){//forward calculating.
			if( jb.trainedtime >= forwardtime ){
				return jb.jobid;
			}
		}
		else{	//backward calculating.
			if(jb.trainedtime >= backwardtime){
				return jb.jobid;
			}
		}
		return 0;
	}
	void parse_job_list(){
		bool parsed = false;
		std::vector<job>::iterator it;
		for(it = myjoblist.begin(); it != myjoblist.end(); it++){
			//Find whether there is a backward job first.
			if(it->isbackward && it->arrivedtime <= GlobalTime){
				if (PRINT_MODE){
					std::cout << "worker:" << workerid << " found a backward job " << it->jobid << " at time " << GlobalTime << "\n";
				}

				int re = parse_job(*it);
				if (re != 0){	//one job is finished, pass it to the next worker.
					PassJob2NextWorker(*it, workerid);
					it = myjoblist.erase(it);
				}
				parsed = true;
				break;
			}
		}

		if(!parsed){ //If there is no backward job, begin with the first job.
			it = myjoblist.begin();
			if (it != myjoblist.end() ){
				if (it->arrivedtime <= GlobalTime)
				{
					int re = parse_job(*it);
					if (re != 0){	//one forward job is finished, change the direction or pass it to the next worker.
						if (lastworker){//lastworker finish the forward job, change direction.
							it->isbackward = true;
							it->trainedtime = 0;
						}
						else{
							PassJob2NextWorker(*it, workerid);
							it = myjoblist.erase(it);
						}
					}
					parsed = true;
				}
			}
		}
	}
};

void OneJobIsOver(int jobid){
	ActiveJobs--;
	std::cout << "The job " << jobid << " is finished. Global time is " << GlobalTime << "\n";
}

void PassJob2NextWorker(job& jb, int lastworkerid){
	jb.trainedtime = 0;
	std::vector<worker>::iterator it;
	if(jb.isbackward){	// should consider this job is on the first worker or not.
		if(lastworkerid == 1){
			OneJobIsOver(jb.jobid);
			return;
		}
		for(it = workerlist.begin(); it != workerlist.end(); it++){
			if(it->workerid == lastworkerid - 1){
				job newjob;
				newjob.jobid = jb.jobid;
				newjob.isbackward = jb.isbackward;
				newjob.trainedtime = jb.trainedtime;
				newjob.arrivedtime = GlobalTime + 1;
				it->get_a_new_job(newjob);
				break;
			}
		}
		if(PRINT_MODE){
			std::cout << "Job " << jb.jobid << " is passed through worker " << lastworkerid \
					  << " to worker " << it->workerid << " at time " << GlobalTime << "\n";
		}
	}
	else{//forward 
		for(it = workerlist.begin(); it != workerlist.end(); it++){
			if(it->workerid == lastworkerid + 1){
				job newjob;
				newjob.jobid = jb.jobid;
				newjob.isbackward = jb.isbackward;
				newjob.trainedtime = jb.trainedtime;
				newjob.arrivedtime = GlobalTime + 1;
				it->get_a_new_job(newjob);
				break;
			}
		}
		if(PRINT_MODE){
			std::cout << "Job " << jb.jobid << " is passed through worker " << lastworkerid \
					  << " to worker " << it->workerid << " at time " << GlobalTime << "\n";
		}
	}
}


void usage(char* argv)
{
    std::cout << "This is a pipeline parallelism task simulator, here is the usage:\n";
    std::cout << argv << " <workernum> <forwardtime> <backwardtime> <batchsize> <simulatetime>\n";
    std::cout << "Hit: All parameters are integer.\n";
}

int main(int argc, char* argv[])
{
	if(argc != 6){
        usage(argv[0]);
        return 1;
    }
    workernum = std::atoi(argv[1]);
    forwardtime = std::atoi(argv[2]);
    backwardtime = std::atoi(argv[3]);
    batchsize = std::atoi(argv[4]);
    simulatetime = std::atoi(argv[5]);

	ActiveJobs = 0;
	GlobalTime = 1;
	NextJobid = 1;

	for(int i = 1;i <= workernum; i++){
		worker tmpwk;
		tmpwk.workerid = i;
		if(i == workernum){
			tmpwk.lastworker = true;
		}
		else{
			tmpwk.lastworker = false;
		}
		workerlist.push_back(tmpwk);
	}

	while(GlobalTime <= simulatetime){//Start the simulation.
		if(ActiveJobs < batchsize){//new job come to the first worker.
			job newjob;
			newjob.jobid = NextJobid;
			newjob.isbackward = false;
			newjob.trainedtime = 0;
			newjob.arrivedtime = GlobalTime;
			NextJobid++;
			std::vector<worker>::iterator tit;
			for(tit = workerlist.begin(); tit != workerlist.end(); tit++){
				if(tit->workerid == 1 ){
					tit->get_a_new_job(newjob);
				}
			}
			ActiveJobs++;
		}
		std::vector<worker>::iterator it;//everyworker begin to parse joblist.
		for(it = workerlist.begin(); it != workerlist.end(); it++){
			it->parse_job_list();
		}
		GlobalTime++;
	}
}