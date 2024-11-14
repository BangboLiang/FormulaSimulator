#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#define PRINT_MODE 1 //1 for debugging
#define NODE_LINK_NUM 2
#define LINK_CAPACITY 1
#define JOB_BYTESIZE 2

// class connection
// {
//     int srcid;
//     int desid;
//     int srclink;
//     int deslink;
// };

class Link
{
public:
    int linkid;
    double capacity;
};

class Job
{
public:
    int jobid;
    int srcid;
    int desid;
    int start_time;
    double bytes_remaining;
};

class Node
{
public:
    int nodeid;
    int linknum;
    Link mylinks[10];
    std::vector<Job> myjoblist;
};

struct FileJob
{
    int stage;
    int src;
    int des;
};


//Global variables
Node nodes[100];
Link links[200];
bool OpticalGraph[200][200];//true for connected, false for not connected.
std::map<int, std::vector<int> > Node2Links; //Nodeid->vector<int> link ids.
std::vector<FileJob> filejbs;
double CommunicationTime;
int totaljobnum;
std::vector<double> job_finish_times;


/*
    Function: Node2NodeConnected
    Return: Linkid if this link is connected with destination's link
            -1 if src and des are not connected.
*/
int Node2NodeConnected(int srcid, int desid)
{   //
    bool result = false;
    
    for (int i = 0; i < NODE_LINK_NUM; i++)
    {
        for (int j = 0; j < NODE_LINK_NUM; j++)
        {
            int srclinkid, deslinkid;
            
            if (1)
            {
                /* code */
            }
            
        }
        
    }
    
}