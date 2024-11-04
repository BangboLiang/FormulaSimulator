#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

// 前向声明
class Link;       // 链路类
class Scheduler;  // 调度器类
class Task;       // 任务类
class Connection; // 连接类
class Node;       // 节点类

// 链路类
class Link {
public:
    int id;
    int bandwidth;  // 链路带宽（每秒可传输字节数）
    queue<Task*> taskQueue;  // 任务队列

    Link(int id, int bandwidth) : id(id), bandwidth(bandwidth) {}

    // 每秒处理任务
    void processTasks();
};

// 节点类
class Node {
public:
    int id;
    vector<Link*> links;  // 每个节点有两条链路

    Node(int id) : id(id) {}

    void addLink(Link* link) {
        links.push_back(link);
    }
};

// 连接类，描述任务的源节点和目的节点，以及它们使用的链路
class Connection {
public:
    int srcNodeId;
    int destNodeId;
    int srcLinkId;
    int destLinkId;

    Connection(int srcNodeId, int destNodeId, int srcLinkId, int destLinkId)
        : srcNodeId(srcNodeId),
          destNodeId(destNodeId),
          srcLinkId(srcLinkId),
          destLinkId(destLinkId) {}
};

// 任务类
class Task {
public:
    Connection connection;
    int remainingBytes;  // 剩余未传输的字节数

    Task(Connection connection, int bytes)
        : connection(connection), remainingBytes(bytes) {}
};

// 调度器类，负责管理任务的调度和完成
class Scheduler {
public:
    vector<Node*> nodes;  // 所有节点
    map<int, Link*> links;  // 链路ID到链路对象的映射
    vector<Task*> tasks;  // 所有正在运行的任务
    int secondsPassed;  // 已经过去的秒数

    Scheduler() : secondsPassed(0) {}

    // 添加节点
    void addNode(Node* node) {
        nodes.push_back(node);
    }

    // 添加链路
    void addLink(Link* link) {
        links[link->id] = link;
    }

    // 添加任务到链路中
    void addTask(Task* task) {
        tasks.push_back(task);
        // 将任务加入对应链路的队列中
        links[task->connection.srcLinkId]->taskQueue.push(task);
    }

    // 每秒处理任务
    void simulate() {
        while (!tasks.empty()) {
            secondsPassed ++ ;
            cout << "Second " << secondsPassed << ": Processing tasks..." << endl;

            // 遍历所有链路，处理任务
            for (auto& linkPair : links) {
                Link* link = linkPair.second;
                link -> processTasks();
            }

            // 检查是否还有未完成的任务
            tasks.erase(remove_if(tasks.begin(), tasks.end(), [](Task* task) {
                return task -> remainingBytes <= 0;
            }), tasks.end());
        }
    }
};

// 链路处理任务的实现
void Link::processTasks() {
    if (!taskQueue.empty()) {
        Task* task = taskQueue.front();
        int bytesToProcess = min(bandwidth, task->remainingBytes); // 计算当前链路带宽（bandwidth）和任务剩余字节数（task->remainingBytes）之间的较小值
        task->remainingBytes -= bytesToProcess; // 更新任务的剩余字节数
        cout << "Link " << id << " processed " << bytesToProcess << " bytes. Remaining: " << task->remainingBytes << " bytes." << endl;

        // 如果任务完成，移出队列
        if (task->remainingBytes <= 0) {
            taskQueue.pop();
            cout << "Task on link " << id << " completed." << endl;
        }
    }
}

// 主程序
int main() {
    Scheduler scheduler;

    // 创建节点和链路
    for (int i = 0; i < 8; ++i) {
        Node* node = new Node(i);
        scheduler.addNode(node);

        // 每个节点有两条链路，每条链路带宽为 100 字节 / 秒
        for (int j = 0; j < 2; j ++ ) {
            Link* link = new Link(i * 2 + j, 100);  // 链路ID从 0 开始
            node->addLink(link);
            scheduler.addLink(link);
        }
    }

    // 创建任务（假设 8 个任务，每个任务有 1000 字节要传输）
    for (int i = 0; i < 8; i ++ ) {
        int srcNodeId = i;
        int destNodeId = (i + 1) % 8;  // 假设目的节点为下一个节点
        int srcLinkId = srcNodeId * 2;  // 假设每个任务使用节点的第一条链路
        int destLinkId = destNodeId * 2;

        Connection connection(srcNodeId, destNodeId, srcLinkId, destLinkId);
        Task* task = new Task(connection, 1000);  // 每个任务传输 1000 字节
        scheduler.addTask(task);
    }

    // 模拟任务处理
    scheduler.simulate();

    cout << "All tasks completed in " << scheduler.secondsPassed << " seconds." << endl;

    return 0;
}