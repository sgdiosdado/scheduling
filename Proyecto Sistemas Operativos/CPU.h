//
//  CPU.h
//  Proyecto Sistemas Operativos
//
//  Created by Iñaki Janeiro Olague on 02/12/19.
//  Copyright © 2019 Iñaki Janeiro Olague. All rights reserved.
//

#ifndef CPU_h
#define CPU_h

#include "Process.h"

// Singleton object CPU
class CPU{
private:
    CPU();
    static CPU* instance;
    static Process* process;
public:
    static CPU* createInstance();
    static void attachProcess(Process* process);
    static void unattachProcess();
    static bool empty();
    static Process* getProcess();
    static void run();
};

CPU* CPU::instance = nullptr;
Process* CPU::process = nullptr;

CPU* CPU::createInstance() {
    if (instance == nullptr) {
        instance = new CPU();
    }

    return instance;
}

CPU::CPU()
{}

void CPU::attachProcess(Process* process){
    CPU::process = process;
}

bool CPU::empty(){
    return process == nullptr;
}

Process* CPU::getProcess(){
    return process;
}

void CPU::unattachProcess(){
    process = nullptr;
}

void CPU::run(){
    if (process)
        process->setCPUTime(process->getCPUTime() + 1);
}
#endif /* CPU_h */
