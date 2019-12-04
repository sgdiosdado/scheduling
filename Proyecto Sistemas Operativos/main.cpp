//
//  main.cpp
//  ProyectoSisops
//
//  Created by Raúl Castellanos on 28/11/19.
//  Copyright © 2019 Raúl Castellanos. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;
#include "Scheduler.h"

int main(int argc, const char * argv[]) {
    
    string input;
    cout << "Ingrese el archivo (con extension) que desea leer: ";
    cin >> input;
    Scheduler scheduler(input);
    scheduler.start();
    
    return 0;
}
