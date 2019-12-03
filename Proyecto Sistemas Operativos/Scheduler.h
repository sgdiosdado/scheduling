//
//  Scheduler.h
//  ProyectoSisops
//
//  Created by Iñaki Janeiro Olague on 01/12/19.
//  Copyright © 2019 Raúl Castellanos. All rights reserved.
//

#ifndef Scheduler_h
#define Scheduler_h
#include <queue>
#include <regex>
#include <sstream>
#include <map>

#include "TextTable.h"
#include "Process.h"
#include "CPU.h"
#include "SchedulerQueue.h"

struct Instruction {
    int pid, prio;
    string event;
    Instruction(){};
    Instruction(int pid, string event){
        this->pid = pid;
        this->event = event;
        this->prio = 1;
    }
};

class Scheduler {
public:
    Scheduler(string inputName);
    void getMetaData();
    void start();
    void run();
    bool checkSyntax(string line);
    void performInstruction(int timestamp);
    void storeEvents(string line);
    void processArrives(int pid, int timestamp);
    void processExits(int pid, int timestamp);
    void addWaitingTime();
    void printProcesses();
    void printTable(int timestamp);
    Process* searchQueue(queue<Process*> &search, int pid);
    void blockProcess(Process* process);
    void unblockProcess(int pid);
    void printTableHeader(char c);
private:
    ifstream file;
    set<string>policies = {"RR", "prioNonPreemptive"};
    string policy;
    CPU *cpu;
    int quantum;
    bool simulationIsRunning = true;
    map<int, Instruction> instructions;
    
    SchedulerQueue ready;
    SchedulerQueue blocked;
    SchedulerQueue finished;
    
    TextTable t;
    TextTable s;
};

Scheduler::Scheduler(string inputName){
    this->file.open(inputName);
    cpu->CPU::createInstance();
    getMetaData();
    TextTable t( '-', '|', '+' );
    TextTable s( '-', '|', '+' );
}

// TODO: Not return string
void Scheduler::getMetaData(){
    string policy;
    string word;
    int quantumNumber = -1;
    this->file >> policy;
    if(policies.find(policy) == policies.end())
        throw "Policy not supported";
    this->file >> word >> quantumNumber;
    
    if (word != "QUANTUM")
        throw "'QUANTUM' statement not found";
    if (policy == "RR" && quantumNumber <= 0)
        throw "Unvalid Quantum for Round Robin";
    if (policy == "prioNonPreemptive" && quantumNumber != 0)
        throw "Unvalid Quantum for prioNonPreemptive";

    this->quantum = quantumNumber;
    this->policy = policy;
}

void Scheduler::start(){
    string line;
    while (getline(this->file, line)){
        if (checkSyntax(line)){
            storeEvents(line);
        }
    }
    run();
    cout << endl << endl;
    printProcesses();
}

void Scheduler::run(){
    
    printTableHeader('h');
    unsigned int clock = 0;
    while (simulationIsRunning){
        bool instructionExists = instructions.find(clock) != instructions.end();
        if (clock % quantum == 0 || instructionExists){
            performInstruction(clock);
            if (cpu->empty() && !ready.empty()){
                cpu->attachProcess(ready.front());
                ready.pop();
            }
            printTable(clock);
        }
        cpu->run();
        addWaitingTime();   
        clock++;
    }
    t.setAlignment( 2, TextTable::Alignment::RIGHT );
    cout << t << endl;
}

bool Scheduler::checkSyntax(string line){
    //TODO: Ignorar comentarios
    regex reg1("^(( *)//( *)([a-z|A-Z]*) )?( *)[0-9]+( +)[a-z|A-Z|/]+( *)[0-9]+( *)(//( *)([a-z|A-Z]*)( *))?$", regex::icase);
    regex reg2("^(( *)//( *)([a-z|A-Z]*) )?( *)[0-9]+( +)(endSimulacion|endSimulación)( *)(//( *)([a-z|A-Z]*)( *))?$", regex::icase);
    return regex_match(line, reg1) || regex_match(line, reg2);
}

void Scheduler::storeEvents(string line){
    istringstream istr (line);
    string timeS;
    int pid, timestamp, prio;
    string event, word;
    istr >> timeS >> event >> pid;
    if (timeS[0] == '/')
        return;
    else
        timestamp = stoi(timeS);
    for (int i = 0; i < event.length(); i++){
        event[i] = tolower(event[i]);
    }
    Instruction inst(pid, event);
    if(policy == "prioNonPreemptive"){
        istr >> word >> prio;
        inst.prio = prio;
    }
    instructions[timestamp] = inst;
}

void Scheduler::performInstruction(int timestamp){
    
    string event = instructions[timestamp].event;
    int pid = instructions[timestamp].pid;
    
    if (event == "llega") // Create process
        processArrives(pid, timestamp);
    else if (event == "acaba")
        processExits(pid, timestamp);
    else if (event == "starti/o")
        blockProcess(cpu->getProcess());
    else if (event == "endi/o")
        unblockProcess(pid);
    else if (event == "endsimulacion" || event == "endsimulación")
        simulationIsRunning = false;
    else if (timestamp % quantum == 0){
        Process* process = cpu->getProcess();
        cpu->unattachProcess();
        ready.push(process);
    }
    else
        throw "Unsupported action: " + event;
}

void Scheduler::processArrives(int pid, int timestamp){
    ready.push(new Process(pid, timestamp));
}

void Scheduler::processExits(int pid, int timestamp){
    Process *process = cpu->getProcess();
    if (process && process->getPid() == pid){
        cpu->unattachProcess();
        finished.push(process);
        process->setCompletionTime(timestamp);
        return;
    }
    
    // TODO: There must be a better way to do this
    process = ready.search(pid);
    if (process){
        finished.push(process);
        process->setCompletionTime(timestamp);
        return;
    }
    process = blocked.search(pid);
    finished.push(process);
    process->setCompletionTime(timestamp);


}

Process* Scheduler::searchQueue(queue<Process*> &search, int pid){
    queue<Process*> temp;
    
    Process* process = nullptr;
    while (!search.empty()){
        Process* processFound = search.front();
        search.pop();
        if (processFound->getPid() == pid) {
            process = processFound;
        }
        else {
            temp.push(processFound);
        }
    }
    search = temp;
    return process;
}

void Scheduler::blockProcess(Process *process){
    cpu->unattachProcess();
    blocked.push(process);
}

void Scheduler::unblockProcess(int pid){
    ready.push(blocked.search(pid));
    blocked.erase(pid);
}

void Scheduler::addWaitingTime(){
    ready.addWaitingTime();
    
    blocked.addWaitingTime();
    blocked.addIOTime();
}

void Scheduler::printProcesses(){
    printTableHeader('m');
    finished.printProcesesDetails(s);
    cout << s;
}

void Scheduler::printTable(int timestamp){
    string str = "";
    
    if (instructions[timestamp].event != ""){
        str += to_string(timestamp) +" "+ instructions[timestamp].event;
        if(instructions[timestamp].event != "endsimulacion"){
            str += "  "+ to_string(instructions[timestamp].pid);
        }
        t.add(str);
    }
    else {
        t.add(to_string(timestamp) + " quantum");
    }
    
    str = ready.printQueue();
    t.add(str);
    
    str = "";
    Process* process = cpu->getProcess();
    if (process)
        str += to_string(process->getPid());
    else
        str += "-";
    t.add(str);
    
    str = blocked.printQueue();
    t.add(str);
    
    str = finished.printQueue();
    t.add(str);

    t.endOfRow();
}

void Scheduler::printTableHeader(char c){
    if(c == 'H' || c == 'h'){
        t.add("");
        t.add("");
        t.add("HISTORIAL");
        t.add("");
        t.add("");
        t.endOfRow();
        
        t.add("Proceso");
        t.add("Listos");
        t.add("CPU");
        t.add("Bloquedos");
        t.add("Terminados");
        t.endOfRow();
    } else{
        s.add("");
        s.add("");
        s.add("");
        s.add("MÉTRICAS");
        s.add("");
        s.add("");
        s.add("");
        s.endOfRow();
        
        s.add("Proceso");
        s.add("Llegada");
        s.add("Terminada");
        s.add("CPU");
        s.add("Espera");
        s.add("Turnaround");
        s.add("I/O");
        s.endOfRow();
    }
}
#endif /* Scheduler_h */
