/**
 * ======================================================================================
 * IMPLEMENTATION: ticketing.cpp
 * DESCRIPTION: Implementation of TicketSystem with Multi-Queue Management
 * 
 * DATA STRUCTURE: Multiple Custom MyQueue instances
 * - generalQueue: Standard passengers
 * - ladiesQueue: Female passengers (priority over general)
 * - seniorQueue: Senior citizens (highest priority)
 * 
 * PROCESSING ORDER: Senior → Ladies → General
 * This implements a priority-based queue system using multiple queues
 * ======================================================================================
 */

#include "../include/ticketing.h"
#include "../include/globals.h"
#include <iostream>
#include <cstdlib>

// ======================================================================================
//                                   TICKET SYSTEM IMPLEMENTATION
// ======================================================================================

/**
 * Constructor: TicketSystem
 * Initializes the multi-queue ticketing system
 * 
 * Initializes:
 *   - Three separate queues (general, ladies, senior)
 *   - Revenue tracking variables
 *   - Ticket counter
 */
TicketSystem::TicketSystem() {
    totalTicketsSold = 0;
    totalRevenue = 0;
}

/**
 * Function: joinQueue
 * Adds a passenger to the appropriate queue based on passenger type
 * 
 * Parameters:
 *   p - Passenger object containing passenger details
 * 
 * Queue Selection Logic:
 *   - LADIES type → ladiesQueue
 *   - SENIOR type → seniorQueue
 *   - GENERAL/DISABILITY type → generalQueue
 * 
 * Time Complexity: O(1) - Custom MyQueue push operation
 * 
 * Uses: Custom MyQueue data structure (demonstrating linked list-based queue)
 */
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

/**
 * Function: processQueues
 * Processes all queues in priority order using custom MyQueue operations
 * 
 * Processing Order (Priority-based):
 *   1. Senior Citizens Queue (Highest Priority)
 *   2. Ladies Queue (Medium Priority)
 *   3. General Queue (Standard Priority)
 * 
 * Algorithm:
 *   - For each queue in priority order:
 *     - While queue is not empty:
 *       - Get front passenger (MyQueue::front())
 *       - Remove from queue (MyQueue::pop())
 *       - Process ticket (fare calculation, revenue update)
 * 
 * Time Complexity: O(n) where n = total passengers in all queues
 * 
 * Uses: Custom MyQueue data structure for FIFO processing within each priority level
 * 
 * Real-world scenario: Ticket counter processes waiting passengers
 */
void TicketSystem::processQueues() {
    std::cout << "\n--- Processing Ticket Queues ---\n";
    
    // Process Senior Citizens first (Highest Priority)
    int seniorCount = 0;
    while (!seniorQueue.empty()) {
        Passenger p = seniorQueue.front();
        seniorQueue.pop();
        processTicket(p);
        seniorCount++;
    }
    if (seniorCount > 0) {
        std::cout << "  [Priority] Processed " << seniorCount << " senior citizen(s)\n";
    }

    // Process Ladies (Medium Priority)
    int ladiesCount = 0;
    while (!ladiesQueue.empty()) {
        Passenger p = ladiesQueue.front();
        ladiesQueue.pop();
        processTicket(p);
        ladiesCount++;
    }
    if (ladiesCount > 0) {
        std::cout << "  [Priority] Processed " << ladiesCount << " ladies queue passenger(s)\n";
    }

    // Process General (Standard Priority)
    int generalCount = 0;
    while (!generalQueue.empty()) {
        Passenger p = generalQueue.front();
        generalQueue.pop();
        processTicket(p);
        generalCount++;
    }
    if (generalCount > 0) {
        std::cout << "  [Standard] Processed " << generalCount << " general queue passenger(s)\n";
    }
    
    std::cout << "--------------------------------\n";
}

/**
 * Function: processTicket
 * Processes individual ticket: fare calculation, revenue tracking, station updates
 * 
 * Parameters:
 *   p - Passenger object to process
 * 
 * Fare Calculation:
 *   - Base fare: Rs. 10
 *   - Variable component: Random(0-49) to simulate distance-based pricing
 *   - Total: Rs. 10-59
 * 
 * Operations:
 *   1. Calculate fare based on distance/zones
 *   2. Update ticket price in passenger object
 *   3. Increment total tickets sold counter
 *   4. Add to total revenue
 *   5. Display ticket details
 *   6. Update station passenger count (for analytics)
 * 
 * Time Complexity: O(1)
 * 
 * Revenue Tracking: Maintains cumulative total for financial analytics
 */
void TicketSystem::processTicket(Passenger p) {
    // Fare calculation (base + distance-based component)
    int fare = 10 + (rand() % 50);  // Rs. 10-59
    p.ticketPrice = fare;
    
    // Update system statistics
    totalTicketsSold++;
    totalRevenue += fare;
    
    // Display ticket information
    std::cout << "[TICKET ISSUED] " << p.name << " | Fare: Rs. " << fare << " | Type: ";
    if (p.type == SENIOR) std::cout << "Senior";
    else if (p.type == LADIES) std::cout << "Ladies";
    else std::cout << "General";
    std::cout << std::endl;
    
    // Update station analytics (passenger flow tracking)
    if (p.sourceId >= 0 && (size_t)p.sourceId < allStations.size()) {
        allStations[p.sourceId].passengerCount++;
    }
}

/**
 * Function: showStats
 * Displays comprehensive ticketing analytics and revenue report
 * 
 * Statistics Shown:
 *   - Total number of tickets sold
 *   - Total revenue collected (in Rupees)
 * 
 * Time Complexity: O(1)
 * 
 * Use Case: Financial reporting, performance metrics, capacity analysis
 */
void TicketSystem::showStats() {
    std::cout << "\n========== Ticketing Analytics ==========\n";
    std::cout << "Total Tickets Sold: " << totalTicketsSold << std::endl;
    std::cout << "Total Revenue: Rs. " << totalRevenue << std::endl;
    
    // Calculate average fare
    if (totalTicketsSold > 0) {
        double avgFare = (double)totalRevenue / totalTicketsSold;
        std::cout << "Average Fare: Rs. " << avgFare << std::endl;
    }
    
    std::cout << "=========================================\n";
}
