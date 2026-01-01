/**
 * ======================================================================================
 * IMPLEMENTATION: scheduling.cpp
 * DESCRIPTION: Implementation of Scheduler class
 * ======================================================================================
 */

#include "../include/scheduling.h"
#include <iostream>
#include <iomanip>
#include <cstdio>

// ======================================================================================
//                                   SCHEDULER IMPLEMENTATION
// ======================================================================================

void Scheduler::scheduleTrain(int id, std::string name, int time, int startStationId) {
    Train t;
    t.trainId = id;
    t.name = name;
    t.arrivalTime = time;
    t.nextStationId = startStationId;
    t.status = ON_TIME;
    t.capacity = 2000;
    t.currentLoad = 0;
    
    trainSchedule.push(t);
}

void Scheduler::showUpcomingTrains() {
    std::cout << "\n--- Upcoming Train Schedule (Sorted by Time) ---\n";
    if (trainSchedule.empty()) {
        std::cout << "No trains scheduled.\n";
        return;
    }

    // Create a copy to display without removing
    MinHeap<Train> temp = trainSchedule;
    
    std::cout << std::left << std::setw(10) << "Time" << std::setw(20) << "Train Name" 
              << std::setw(15) << "Status" << std::endl;
    std::cout << "------------------------------------------------\n";
    
    while (!temp.empty()) {
        Train t = temp.top();
        temp.pop();
        
        int hrs = t.arrivalTime / 60;
        int mins = t.arrivalTime % 60;
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d", hrs, mins);
        
        std::string statusStr = (t.status == ON_TIME) ? "ON TIME" : "DELAYED";
        
        std::cout << std::left << std::setw(10) << timeStr << std::setw(20) << t.name 
                  << std::setw(15) << statusStr << std::endl;
    }
    std::cout << "------------------------------------------------\n";
}

void Scheduler::optimizeFrequency(bool isPeakHour) {
    if (isPeakHour) {
        std::cout << "\n[SYSTEM] Peak Hour Detected! Increasing train frequency...\n";
        scheduleTrain(901, "Peak Special 1", 540, 0); // 9:00 AM
        scheduleTrain(902, "Peak Special 2", 550, 0); // 9:10 AM
    } else {
        std::cout << "\n[SYSTEM] Off-Peak Hours. Standard frequency maintained.\n";
    }
}
