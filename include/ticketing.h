/**
 * ======================================================================================
 * HEADER: ticketing.h
 * DESCRIPTION: Passenger struct and ticket management system
 * ======================================================================================
 */

#ifndef TICKETING_H
#define TICKETING_H

#include <string>
#include <ctime>
#include "station.h"
#include "queue_manager.h"

// ======================================================================================
//                                   PASSENGER STRUCTURE
// ======================================================================================

struct Passenger {
    int id;
    std::string name;
    int age;
    PassengerType type;
    int sourceId;
    int destId;
    int ticketPrice;
    time_t entryTime;
};

// ======================================================================================
//                                   TICKET SYSTEM CLASS
// ======================================================================================

/**
 * Queue Management for Ticketing using Custom MyQueue
 */
class TicketSystem {
    MyQueue<Passenger> generalQueue;
    MyQueue<Passenger> ladiesQueue;
    MyQueue<Passenger> seniorQueue;
    int totalTicketsSold;
    long long totalRevenue;

public:
    TicketSystem();
    
    void joinQueue(Passenger p);
    void processQueues();
    void processTicket(Passenger p);
    void showStats();
};

#endif // TICKETING_H
