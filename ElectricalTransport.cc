#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define PRINT_MODE 1 //1 for debugging

double ConnectionGetThroughput(int srcid, int desid, int srclink, int deslink);

class Link
{
public:
    double capacity;
    int jobnums;
    double get_curr_band(){
        return (capacity / jobnums);
    }
};

class Node
{
public:
    int Nodeid;
    int linknum;
    Link mylinks[10];
};

class Connection
{
public:
    int srcid;
    int desid;
    int srclink;
    int deslink;
    double get_throught()
    {
        return ConnectionGetThroughput(srcid, desid, srclink, deslink);
    }
};

class Job
{
public:
    Connection con;
    bool connected;
    double bytes_remaining;
    int parse()
    {
        if (bytes_remaining <= 0)
        {
            return 1;   //finished
        }

        double cost = con.get_throught();
        bytes_remaining = bytes_remaining - cost;
        if (bytes_remaining <= 0)
        {
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
};


//Global variables
Node nodes[100];
std::vector<Job> runningjobs;
PipelineWorkElement ppworks[1000];
std::vector<FileJob> filejbs;
double CommunicationTime;


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

void ParseAllRunningJobs()
{
    std::vector<Job>::iterator it = runningjobs.begin();
    while (it != runningjobs.end())
    {
        if (it->connected == false)
        {
            
        }
        
    }
    
}

void SimulateByStage(int total_stage)
{
    int CurrentStage = 1;
    while (CurrentStage <= total_stage)
    {
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
                runningjobs.push_back(tmpjb);
                it = filejbs.erase(it);
            }
            else{
                ++it;
            }
        }
    }
    
}


int main(int argc, char const *argv[])
{
    CommunicationTime = 0;
    std::string filename;
    filename = "PPTaskGenResult.txt";
    int total_Stage = ReadFromFile(filename);
    SimulateByStage(total_Stage);

    return 0;
}
