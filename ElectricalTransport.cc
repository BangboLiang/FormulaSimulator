#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define PRINT_MODE 1 //1 for debugging
#define NODE_LINK_NUM 2
#define LINK_CAPACITY 400
#define JOB_BYTESIZE 2

double ConnectionGetThroughput(int srcid, int desid, int srclink, int deslink);

class Job;
void JobCreateConnection(Job &jb);

class Link
{
public:
    double capacity;
    int jobnums;
    double get_curr_band(){
		if (jobnums == 0)
		{
			return capacity;
		}
        return (capacity / jobnums);
    }
};

class Node
{
public:
    int Nodeid;
    int linknum;
    Link mylinks[10];
    int SetNewConnection()
    {
		//std::cout << "Node " << Nodeid << " is setting connection.\n";
        //Find a link which jobnum is lowest.
        double maxjobnum = 0xffffffff;
        int result = 0;
        for (int i = 0; i < linknum; i++)
        {
            if (mylinks[i].jobnums < maxjobnum)
            {
                maxjobnum = mylinks[i].jobnums;
                result = i;
            }
        }
        mylinks[result].jobnums++;
		//std::cout << "Node " << Nodeid << " got a new connection on link:" << result <<"\n";
        return result;
    }
    void RemOldConnection(int tlinknum)
    {
        mylinks[tlinknum].jobnums--;
		//std::cout << "Node " << Nodeid << " deleted an old connection on link:" << tlinknum <<"\n";
    }
};

class Connection
{
public:
    int srcid;
    int desid;
    int srclink;
    int deslink;
    double get_throughput()
    {
        return ConnectionGetThroughput(srcid, desid, srclink, deslink);
    }
};

class Job
{
public:
    Connection con;
    bool connected;
	int jobid;
	double start_time;
    double bytes_remaining;
    int parse()
    {
        if (bytes_remaining <= 0)
        {
            //job finished
            return 1;
        }

        double cost = con.get_throughput();
        bytes_remaining = bytes_remaining - cost;
        if (bytes_remaining <= 0)
        {
            std::cout << "Job " << jobid << " byte remaining:" << bytes_remaining << '\n';
            return 1;
        }
        return 0;
    }
};

class PipelineWorkElement
{
public:
    int stage;
    int srcid;
    int desid;
    double bytes;
};

struct FileJob
{
    int stage;
    int src;
    int des;
    double jobbytes;
};


//Global variables
Node nodes[100];
std::vector<Job> runningjobs;
PipelineWorkElement ppworks[1000];
std::vector<FileJob> filejbs;
double CommunicationTime;
int totaljobnum;
std::vector<double> job_finish_times; // Use this to calculate job average finish time.

void JobCreateConnection(Job &jb)
{
	//std::cout << "running JobCreateConnection.\n";
    int srcidx = jb.con.srcid;
    int desidx = jb.con.desid;
    int tsrclink = nodes[srcidx].SetNewConnection();
    int tdeslink = nodes[desidx].SetNewConnection();
    jb.con.srclink = tsrclink;
    jb.con.deslink = tdeslink;
    jb.connected = true;
	jb.start_time = CommunicationTime;
	std::cout << "Job "<<jb.jobid <<"'s connection established, src " << jb.con.srcid << " link " << jb.con.srclink << " des " << jb.con.desid << " link " << jb.con.deslink << '\n';
}


double ConnectionGetThroughput(int srcid, int desid, int srclink, int deslink)
{
    double srcbd = nodes[srcid].mylinks[srclink].get_curr_band();
    double desbd = nodes[desid].mylinks[deslink].get_curr_band();
    if (srcbd <= desbd)
    {
        return srcbd;
    }
    else
    {
        return desbd;
    }
}


int ReadFromFile(std::string file_name)
{
    std::ifstream ifile;
    ifile.open(file_name);
    if (PRINT_MODE)
    {
        std::cout << "Reading file: " << file_name << '\n';
    }
    int tmpstage;
    int tmpsrc;
    int tmpdes;
    int totalstages = 0;

    while (1)
    {
        ifile >> tmpstage;
        if (tmpstage == -1)
        {
            break;
        }
        if (tmpstage > totalstages)
        {
            totalstages = tmpstage;
        }
        
        ifile >> tmpsrc >> tmpdes;
        FileJob tmpfjb;
        tmpfjb.stage = tmpstage;
        tmpfjb.src = tmpsrc;
        tmpfjb.des = tmpdes;
        filejbs.push_back(tmpfjb);
    }
    return totalstages;
}

int readFromJobFile(std::string file_name)
{
    std::ifstream ifile;
    ifile.open(file_name);
    if (PRINT_MODE)
    {
        std::cout << "Reading file: " << file_name << '\n';
    }
    int tmpstage;
    int tmpsrc;
    int tmpdes;
    double tmpbyte;
    int totalstages = 0;
    while (1)
    {
        ifile >> tmpstage;
        if (tmpstage == -1)
        {
            break;
        }
        if (tmpstage > totalstages)
        {
            totalstages = tmpstage;
        }
        
        ifile >> tmpsrc >> tmpdes >> tmpbyte;
        std::cout << "Read: " << tmpsrc << ' ' << tmpdes << ' ' << tmpbyte << '\n';
        FileJob tmpfjb;
        tmpfjb.stage = tmpstage;
        tmpfjb.src = tmpsrc;
        tmpfjb.des = tmpdes;
        tmpfjb.jobbytes = tmpbyte;
        filejbs.push_back(tmpfjb);
    }
    return totalstages;
}

void ParseAllRunningJobs()
{
	//std::cout << "Running ParseAllRunningJobs\n";
    std::vector<Job>::iterator it = runningjobs.begin();
	//establish connections for unconnected jobs
	while(it != runningjobs.end())
	{
		if (it->connected == false)
        {
			//std::cout << "Trying to create a connection for job.\n";
            JobCreateConnection(*it);
        }
		it++;
	}
	/*
		Communication time should increase after the job parsing finish,
		but I put this sentence here for recording job finish time easily.
	*/
	CommunicationTime++;
	it = runningjobs.begin();
    while (it != runningjobs.end())
    {
        // if (it->connected == false)
        // {
		// 	std::cout << "Trying to create a connection for job.\n";
        //     JobCreateConnection(*it);
        // }
        int parse_result = it->parse();
        if (parse_result)
        {//This job is finished.
            nodes[it->con.srcid].RemOldConnection(it->con.srclink);
            nodes[it->con.desid].RemOldConnection(it->con.deslink);
            if (PRINT_MODE)
            {
                std::cout << "Job " << it->jobid << " is finished, src " << it->con.srcid << " link " << it->con.srclink \
                << " des " << it->con.desid << " link " << it->con.deslink << '\n';
            }
			std::cout << "job " << it->jobid << "'s finish time: " << CommunicationTime - it->start_time << '\n';
			job_finish_times.push_back(CommunicationTime - it->start_time);
            it = runningjobs.erase(it);
        }
        else
        {//This job is not finished.
            if (PRINT_MODE)
            {
                std::cout << "Job " << it->jobid << " is not finished, src " << it->con.srcid << " link " << it->con.srclink \
                << " des " << it->con.desid << " link " << it->con.deslink << " Bytes remaining: " \
                << it->bytes_remaining << '\n';
            }
			it++;
        }
    }
	
	std::cout << "Communication time now : " << CommunicationTime << "\n" ;
}

void SimulateByStage(int total_stage)
{
    int CurrentStage = 1;
    while (CurrentStage <= total_stage)
    {
        if (PRINT_MODE)
        {
            std::cout << "Stage: " << CurrentStage << '\n';
        }
        //fetch jobs
        std::vector<FileJob>::iterator it;
        for (it= filejbs.begin(); it != filejbs.end();)
        {
            if (it->stage == CurrentStage) 
            {//Put the job into running jobs
                Job tmpjb;
                tmpjb.connected = false;
                tmpjb.con.srcid = it->src;
                tmpjb.con.desid = it->des;
				//tmpjb.bytes_remaining = JOB_BYTESIZE;
                tmpjb.bytes_remaining = it->jobbytes;
				tmpjb.jobid = totaljobnum++;
                runningjobs.push_back(tmpjb);
                it = filejbs.erase(it);
				//std::cout << "push backed a job.\n";
            }
            else{
                ++it;
            }
        }
        while (runningjobs.size() != 0)
        { //still have running jobs.
            ParseAllRunningJobs();
        }
        CurrentStage++;
    }
}

void Nodesinit()
{
    for (int i = 0; i < 100; i++)
    {
        nodes[i].linknum = NODE_LINK_NUM;
        nodes[i].Nodeid = i;
		for (int j = 0; j < 10; j++)
		{
			nodes[i].mylinks[j].jobnums = 0;
			nodes[i].mylinks[j].capacity = LINK_CAPACITY;
		}
    }
}

int main(int argc, char const *argv[])
{
	totaljobnum = 0;
    Nodesinit();
    CommunicationTime = 0;
    std::string filename;
    filename = "./MoEtask.txt";
    int total_Stage = readFromJobFile(filename);
    SimulateByStage(total_Stage);
    std::cout << "Communication Time: " << CommunicationTime << '\n';
	//Get job average finish time
	double joballtime = 0;
	std::vector<double>::iterator it = job_finish_times.begin();
	while (it != job_finish_times.end())
	{
		joballtime += *it;
		it++;
	}
	double averagefinishtime = joballtime / totaljobnum;
	std::cout << "Job average finish time: " << averagefinishtime << '\n';
    return 0;
}
