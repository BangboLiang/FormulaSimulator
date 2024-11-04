#include <iostream>
#include <vector>
#include <cstdlib>
#define PRINT_MODE 1 //1 For print on the terminal, 0 for output to the file.

//parameters:
int workernum = 4;
int forwardtime = 1;
int backwardtime = 2;
int batchsize = 4;
int simulatetime = 100;

int ActiveJobs;
int Nextjobnum;

std::vector<bool> worker_has_backward_job;

void NewJobPushBack(int newid);
void OneJobFinished (int rmjobid);

class job 
{
public:
    int jobid;      //job's id
    int position;   //which worker this job lies in
    int direction;  // +1 or -1
    bool newjob;    //clarify whether this job is new arrived or not
    int jobdelay;   //time cost of this job.
    void jobparse(){
        jobdelay = jobdelay + 1;
        if(newjob){//First to arrive the list, do nothing.
            newjob = false;
            return;
        }
        if( position == workernum ){    
            if(direction == 1 && jobdelay >= forwardtime){//Time to change the direction
                direction = -1;
                jobdelay = 0;
                return;
            }
        }
        else if (position == 1){
            if(direction == -1 && jobdelay >= backwardtime){    //Job is finished
                OneJobFinished(jobid);
            }
        }
        
        //For normal workers, check the direction
        if (direction == 1 && jobdelay >= forwardtime){
            if (!worker_has_backward_job[position + direction - 1]){
                position = position + direction;
            }
        }
        if (direction == -1 && jobdelay >= backwardtime){
            if (!worker_has_backward_job[position + direction - 1]){
                position = position + direction;
            }
        }
        if (direction == -1){
            worker_has_backward_job[postion-1] = true;
        }
    }
    job(){}
};

std::vector<job> Joblist;

void NewJobPushBack(int newid)
{
    job temp;
    temp.jobid = newid;
    temp.position = 1;
    temp.direction = 1;
    temp.newjob = true;
    Joblist.push_back(temp);
    Nextjobnum = Nextjobnum + 1;
}

void OneJobFinished(int rmjobid)
{
    std::vector<job>::iterator it;
    for (it = Joblist.begin(); it != Joblist.end(); it++){
        if(it->jobid == rmjobid){
            it = Joblist.erase(it);
            //jobfinished = true;
            ActiveJobs = ActiveJobs - 1;
            break;
        }
    }
    return;
}

void usage(char* argv)
{
    std::cout << "This is a pipeline parallelism task simulator, here is the usage:\n";
    std::cout << argv << " <workernum> <forwardtime> <backwardtime> <batchsize> <simulatetime>\n";
    std::cout << "Hit: All parameters are integer.\n";
}

int main (int argc, char* argv[])
{
    ActiveJobs = 0;
    Nextjobnum = 0;

    if(argc != 6){
        usage(argv[0]);
        return 1;
    }
    workernum = std::atoi(argv[1]);
    forwardtime = std::atoi(argv[2]);
    backwardtime = std::atoi(argv[3]);
    batchsize = std::atoi(argv[4]);
    simulatetime = std::atoi(argv[5]);

    for (int i = 0;i < workernum; i++){
        worker_has_backward_job.push_back(false);
    }
    
    int GlobalTime = 1;
    // std::cout << "worker num is " << workernum << " forwardtime is " << forwardtime << " backwardtime is " \
    << backwardtime << " batchsize is " << batchsize << '\n';
    
    //Prepare for the simulation
    for (int i = 0; i < batchsize; i++){
        NewJobPushBack(i+1);
    }

    while(GlobalTime <= simulatetime){
        if(Joblist.size() < batchsize && !worker_has_backward_job[0]){//A new job should be arrive.
            NewJobPushBack(Nextjobnum + 1);
        }
        std::vector<job>::iterator tit;
        for (tit = Joblist.begin(); tit != Joblist.end(); tit++){
            tit->jobparse();
        }
        GlobalTime++;
    }
    return 0;
}