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
    
    float avgTurnaround();
    float avgWaitingTime();
};

SchedulerQueue::SchedulerQueue(){
}
Process* SchedulerQueue::front(){
    Process* p = processes.front();
    queue<Process*> pq = processes;
    while(!pq.empty()){
        if(pq.front()->getPriority() < p->getPriority()){
            p = pq.front();
        }
        pq.pop();
    }
    return p;
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
    Process* p = processes.front();
    queue<Process*> pq = processes;
    while(!pq.empty()){
        if(pq.front()->getPriority() < p->getPriority()){
            p = pq.front();
        }
        pq.pop();
    }
    this->erase(p->getPid());
}

bool SchedulerQueue::empty(){
    return processes.empty();
}

string SchedulerQueue::printQueue(){
    queue<Process*> temp = processes;
    stack<Process*> st;
    string str = "";
    while (!temp.empty()){
        Process* process =  temp.front();
        st.push(process);
        temp.pop();
    }
    while(!st.empty()){
        Process* process = st.top();
        str += (to_string(process->getPid()) + " ");
        st.pop();
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
    s.add("");
    s.add("");
    s.add("");
    s.add("");
    s.add("");
    s.add("");
    s.add("");
    s.endOfRow();
    s.add("Avg. Turnaround: ");
    s.add(to_string(this->avgTurnaround()));
    s.add("");
    s.add("Avg. WaitingTime: ");
    s.add(to_string(this->avgWaitingTime()));
    s.add("");
    s.add("");

    s.endOfRow();
}

queue<Process*> SchedulerQueue::getQueue(){
    return processes;
}

float SchedulerQueue::avgTurnaround(){
    queue<Process*> q = processes;
    int avg = 0, counter = 0;
    while(!q.empty()){
        avg +=  (q.front()->getCompletionTime() - q.front()->getArrivalTime());
        counter++;
        q.pop();
    }
    return (float)avg/counter;
}

float SchedulerQueue::avgWaitingTime(){
    queue<Process*> q = processes;
    int avg = 0, counter = 0;
    while(!q.empty()){
        avg +=  q.front()->getWaitingTime();
        counter++;
        q.pop();
    }
    return (float)avg/counter;
}
#endif /* processesueue_h */
