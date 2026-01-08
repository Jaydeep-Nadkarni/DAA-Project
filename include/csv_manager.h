/**
 * ======================================================================================
 * HEADER: csv_manager.h
 * DESCRIPTION: CSV file operations for data persistence
 * ======================================================================================
 */

#ifndef CSV_MANAGER_H
#define CSV_MANAGER_H

#include <string>
#include <vector>
#include "station.h"
#include "ticketing.h"
#include "graph.h"

/**
 * Class: CSVManager
 * Handles persistence of system data to/from CSV files
 */
class CSVManager {
public:
    // File Paths
    static const std::string STATION_FILE;
    static const std::string TICKET_FILE;
    static const std::string ROUTE_FILE;
    static const std::string USER_FILE;

    // Station Operations
    static void saveStations(const std::vector<Station>& stations);
    static bool loadStations(std::vector<Station>& stations);

    // Ticket Operations
    static void saveTickets(const std::vector<Passenger>& tickets);
    static bool loadTickets(std::vector<Passenger>& tickets);
    
    // Append a single ticket (for real-time tracking)
    static void appendTicket(const Passenger& ticket);

    // Route Operations
    static void saveRoutes(RailwayNetwork* network);
    static bool loadRoutes(RailwayNetwork* network);
    
    // Directory Initialization
    static void initializeDataDirectory();
};

#endif // CSV_MANAGER_H
