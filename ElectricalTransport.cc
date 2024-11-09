#include <iostream>
#include <string>
#include <fstream>

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

//Global variables
Node nodes[100];
Job runningjobs[100];
PipelineWorkElement ppworks[1000];

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


void ReadFromFile(std::string file_name)
{
    std::ifstream ifile;
    ifile.open(file_name);
    int stagenum;
    ifile >> stagenum;
    if (PRINT_MODE)
    {
        std::cout << "Readfile stagenums: " << stagenum << '\n';
    }

    for (int i = 0; i < stagenum; i++)
    {
        /* code */
    }
    

    
}


void SimulateByStage()
{
    
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
