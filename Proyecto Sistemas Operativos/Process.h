//
//  Process.h
//  ProyectoSisops
//
//  Created by Raúl Castellanos on 28/11/19.
//  Copyright © 2019 Raúl Castellanos. All rights reserved.
//

#ifndef Process_h
#define Process_h

class Process{
public:
    Process(int pid, int arrivalTime);
    Process(int pid, int arrivalTime, int priority);
    
    int getPid();
    int getCPUTime();
    int getWaitingTime();
    int getIOTime();
    int getArrivalTime();
    int getCompletionTime();
    
    int getPriority();
    
    void setPriority(int priority);
    void setCompletionTime(int time);
    void setCPUTime(int time);
    void setWaitingTime(int time);
    void setIOTime(int time);
    void print(TextTable &s);
    
    bool operator()(const Process& p){
        return this->priority < p.priority;
    }
private:
    int pid;
    int arrivalTime;
    int cpuTime;
    int waitingTime;
    int ioTime;
    int completionTime;
    int priority;
};

Process::Process(int pid, int arrivalTime){
    this->pid = pid;
    this->arrivalTime = arrivalTime;
    this->priority = 0;
    this->cpuTime = 0;
    this->waitingTime = 0;
    this->ioTime = 0;
}

Process::Process(int pid, int arrivalTime, int priority){
    this->pid = pid;
    this->arrivalTime = arrivalTime;
    this->priority = priority;
    this->cpuTime = 0;
    this->waitingTime = 0;
    this->ioTime = 0;
}

int Process::getPriority(){
    return priority;
}

void Process::setPriority(int priority){
    this->priority = priority;
}

int Process::getPid(){
    return pid;
}

void Process::setCompletionTime(int time){
    this->completionTime = time;
}

void Process::setCPUTime(int time){
    this->cpuTime = time;
}

int Process::getCPUTime(){
    return this->cpuTime;
}

void Process::setWaitingTime(int time){
    this->waitingTime = time;
}

int Process::getWaitingTime(){
    return this->waitingTime;
}

void Process::setIOTime(int time){
    this->ioTime = time;
}

int Process::getIOTime(){
    return this->ioTime;
}

int Process::getArrivalTime(){
    return arrivalTime;
}

int Process::getCompletionTime(){
    return completionTime;
}

void Process::print(TextTable &s){
    s.add(to_string(pid));
    s.add(to_string(arrivalTime));
    s.add(to_string(completionTime));
    s.add(to_string(cpuTime));
    s.add(to_string(waitingTime));
    s.add(to_string(ioTime));
    s.add(to_string(completionTime - arrivalTime));
    s.endOfRow();
}

#endif /* Process_h */
