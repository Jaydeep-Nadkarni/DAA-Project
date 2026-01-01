/**
 * ======================================================================================
 * IMPLEMENTATION: ticketing.cpp
 * DESCRIPTION: Implementation of TicketSystem class
 * ======================================================================================
 */

#include "../include/ticketing.h"
#include <iostream>
#include <cstdlib>

// External global data (declared in main.cpp)
extern std::vector<Station> allStations;

// ======================================================================================
//                                   TICKET SYSTEM IMPLEMENTATION
// ======================================================================================

TicketSystem::TicketSystem() {
    totalTicketsSold = 0;
    totalRevenue = 0;
}

void TicketSystem::joinQueue(Passenger p) {
    if (p.type == LADIES) {
        ladiesQueue.push(p);
        std::cout << ">> Passenger " << p.name << " joined LADIES Queue.\n";
    } else if (p.type == SENIOR) {
        seniorQueue.push(p);
        std::cout << ">> Passenger " << p.name << " joined SENIOR Queue.\n";
    } else {
        generalQueue.push(p);
        std::cout << ">> Passenger " << p.name << " joined GENERAL Queue.\n";
    }
}

void TicketSystem::processQueues() {
    std::cout << "\n--- Processing Ticket Queues ---\n";
    
    // Process Senior Citizens first (Priority)
    while (!seniorQueue.empty()) {
        Passenger p = seniorQueue.front();
        seniorQueue.pop();
        processTicket(p);
    }

    // Process Ladies
    while (!ladiesQueue.empty()) {
        Passenger p = ladiesQueue.front();
        ladiesQueue.pop();
        processTicket(p);
    }

    // Process General
    while (!generalQueue.empty()) {
        Passenger p = generalQueue.front();
        generalQueue.pop();
        processTicket(p);
    }
    std::cout << "--------------------------------\n";
}

void TicketSystem::processTicket(Passenger p) {
    int fare = 10 + (rand() % 50); 
    p.ticketPrice = fare;
    totalTicketsSold++;
    totalRevenue += fare;
    
    std::cout << "[TICKET ISSUED] " << p.name << " | Fare: Rs. " << fare << " | Type: ";
    if (p.type == SENIOR) std::cout << "Senior";
    else if (p.type == LADIES) std::cout << "Ladies";
    else std::cout << "General";
    std::cout << std::endl;
    
    if (p.sourceId >= 0 && p.sourceId < allStations.size()) {
        allStations[p.sourceId].passengerCount++;
    }
}

void TicketSystem::showStats() {
    std::cout << "\n--- Ticketing Analytics ---\n";
    std::cout << "Total Tickets Sold: " << totalTicketsSold << std::endl;
    std::cout << "Total Revenue: Rs. " << totalRevenue << std::endl;
    std::cout << "---------------------------\n";
}
