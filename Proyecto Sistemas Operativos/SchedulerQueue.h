//
//  processesueue.h
//  Proyecto Sistemas Operativos
//
//  Created by Sergio Diosdado on 02/12/19.
//  Copyright © 2019 Iñaki Janeiro Olague. All rights reserved.
//

#ifndef SchedulerQueue_h
#define SchedulerQueue_h

class SchedulerQueue {
private:
    queue<Process*> processes;
public:
    SchedulerQueue();
    Process* front();
    Process* search(int pid);
    
    void erase(int pid);
    void addWaitingTime();
    void addIOTime();
    void push(Process* process);
    void pop();
    void printProcesesDetails(TextTable &s);
    
    
    string printQueue();
    bool empty();
    queue<Process*> getQueue();
};

SchedulerQueue::SchedulerQueue(){
}
Process* SchedulerQueue::front(){
    return processes.front();
}

Process* SchedulerQueue::search(int pid){
    queue<Process* > q = processes;
    while(!q.empty()){
        if(q.front()->getPid() == pid){
            return q.front();
        }
        q.pop();
    }
    return nullptr;
}

void SchedulerQueue::erase(int pid){
    queue<Process* > q;
    while(!processes.empty()){
        if(processes.front()->getPid() == pid){
            processes.pop();
            continue;
        }
        q.push(processes.front());
        processes.pop();
    }
    processes = q;
}
void SchedulerQueue::addWaitingTime(){
    queue<Process *> q = processes;
    while(!q.empty()){
        Process* process = q.front();
        process->setWaitingTime(process->getWaitingTime() + 1);
        q.pop();
    }
}

void SchedulerQueue::addIOTime(){
    queue<Process *> q = processes;
    while(!q.empty()){
        Process* process = q.front();
        process->setIOTime(process->getIOTime() + 1);
        q.pop();
    }
}

void SchedulerQueue::push(Process* process){
    processes.push(process);
}

void SchedulerQueue::pop(){
    processes.pop();
}

bool SchedulerQueue::empty(){
    return processes.empty();
}

string SchedulerQueue::printQueue(){
    queue<Process*> temp = processes;
    string str = "";
    while (!temp.empty()){
        Process* process =  temp.front();
        str += (to_string(process->getPid()) + " ");
        temp.pop();
    }
    return str;
}

void SchedulerQueue::printProcesesDetails(TextTable &s){
    queue<Process*> temp = processes;
    while (!temp.empty()){
        Process* process = temp.front();
        temp.pop();
        process->print(s);
    }
}

queue<Process*> SchedulerQueue::getQueue(){
    return processes;
}
#endif /* processesueue_h */
