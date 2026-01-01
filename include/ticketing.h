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
 * 
 * Features:
 * - Multi-queue priority system (Senior > Ladies > General)
 * - Custom MyQueue data structure usage
 * - Revenue and analytics tracking
 * - Fare calculation based on distance
 */
class TicketSystem {
    MyQueue<Passenger> generalQueue;   // Standard passengers
    MyQueue<Passenger> ladiesQueue;    // Female passengers (priority)
    MyQueue<Passenger> seniorQueue;    // Senior citizens (highest priority)
    int totalTicketsSold;              // Total tickets counter
    long long totalRevenue;            // Cumulative revenue in Rupees

public:
    TicketSystem();
    
    // Queue Operations
    void joinQueue(Passenger p);       // Add passenger to appropriate queue
    void processQueues();              // Process all queues in priority order
    void processTicket(Passenger p);   // Process individual ticket
    void showStats();                  // Display analytics and revenue
    
    // Getters for analytics
    int getTotalTickets() const { return totalTicketsSold; }
    long long getTotalRevenue() const { return totalRevenue; }
    
    // Direct revenue tracking for ticketing
    void recordTicket(int fare) {
        totalTicketsSold++;
        totalRevenue += fare;
    }
};

#endif // TICKETING_H
