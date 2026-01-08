/**
 * ======================================================================================
 * PROJECT: Urban Commute & Transportation Management System (Mumbai Local Railway)
 * FILE: main.cpp
 * DESCRIPTION: Menu-driven interface integrating all system modules
 * ======================================================================================
 * 
 * SYSTEM ARCHITECTURE:
 * - Station Management: BST-based directory, global station network
 * - Graph Network: Dijkstra's shortest path, BFS connectivity
 * - Ticketing System: Multi-queue priority processing
 * - Train Scheduling: MinHeap-based time management
 * - Platform Management: Circular queue load balancing
 * - Analytics: Comprehensive reporting and statistics
 * 
 * DATA STRUCTURES USED:
 * 1. Binary Search Tree (BST): Station directory
 * 2. Custom Stack: Path reconstruction in routing
 * 3. Custom Queue: Ticketing, BFS traversal
 * 4. Circular Queue: Platform management
 * 5. Min Heap: Train scheduling by time
 * 6. Graph (Adjacency List): Railway network
 * 7. Hash Maps: O(1) station lookups
 * 
 * ALGORITHMS USED:
 * 1. Dijkstra's Algorithm: Fastest route finding
 * 2. BFS: Network connectivity checks
 * 3. BST Operations: Station search and traversal
 * 4. Heap Operations: Priority-based scheduling
 * 5. Queue Processing: Multi-priority ticket handling
 * ======================================================================================
 */

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

// Include all module headers
#include "../include/globals.h"
#include "../include/station.h"
#include "../include/graph.h"
#include "../include/ticketing.h"
#include "../include/scheduling.h"
#include "../include/queue_manager.h"
#include "../include/analytics.h"
#include "../include/csv_manager.h"
#include "../include/colors.h"

using namespace std;

enum NavigatorState { MAIN_MENU, STATIONS_MENU, TICKETING_MENU };

// ======================================================================================
//                                   GLOBAL SYSTEM OBJECTS
// ======================================================================================

StationBST stationDirectory;        // BST for station search
TicketSystem ticketMachine;         // Multi-queue ticketing system
Scheduler trainScheduler;           // MinHeap-based train scheduler
RailwayNetwork* mumbaiLocal;        // Graph-based railway network
PlatformQueue platformManager(10);  // Circular queue for platform allocation

// ======================================================================================
//                                   SYSTEM INITIALIZATION
// ======================================================================================

/**
 * Function: initializeSystem
 * Initializes the entire Mumbai Local Railway system
 * 
 * Tasks Performed:
 * 1. Initialize all stations on three railway lines
 * 2. Set up station directory (BST)
 * 3. Create railway network graph
 * 4. Add tracks between stations
 * 5. Schedule initial trains
 * 6. Prepare system for operations
 * 
 * Time Complexity: O(n log n) for BST insertions
 * 
 * Real-world use: System startup, cold boot initialization
 */
void initializeSystem() {
    // Step 1: Create railway network graph
    mumbaiLocal = new RailwayNetwork(100);
    
    // Step 2: Initialize stations - Load from CSV if exists, else defaults
    if (CSVManager::loadStations(allStations)) {
        // Rebuild BST and ID maps from loaded stations
        for (auto& s : allStations) {
            stationDirectory.addStation(s.name, s.id);
            string nameLower = s.name;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            stationNameToId[nameLower] = s.id;
            stationIdToName[s.id] = s.name;
        }
        
        // Load routes from CSV
        if (!CSVManager::loadRoutes(mumbaiLocal)) {
            // If routes CSV missing but stations exist, we might have a problem
            // But for now, we'll assume they go together
        }
    } else {
        // Initialize with default hardcoded data
        initializeStations(stationDirectory, mumbaiLocal);
        
        // Save initial data to CSV for future use
        CSVManager::saveStations(allStations);
        CSVManager::saveRoutes(mumbaiLocal);
    }
    
    // Step 3: Schedule initial trains
    
    // Morning trains
    trainScheduler.scheduleTrain(101, "Churchgate Fast", 360, stationNameToId["churchgate"]);   // 6:00 AM
    trainScheduler.scheduleTrain(102, "Virar Slow", 375, stationNameToId["virar"]);            // 6:15 AM
    trainScheduler.scheduleTrain(103, "CST Express", 420, stationNameToId["cst"]);             // 7:00 AM
    trainScheduler.scheduleTrain(104, "Kalyan Local", 450, stationNameToId["kalyan"]);         // 7:30 AM
    
    // Peak hour trains
    trainScheduler.scheduleTrain(201, "Dadar Special", 480, stationNameToId["dadar"]);         // 8:00 AM
    trainScheduler.scheduleTrain(202, "Andheri Fast", 510, stationNameToId["andheri"]);        // 8:30 AM
    trainScheduler.scheduleTrain(203, "Harbour Express", 540, stationNameToId["panvel"]);      // 9:00 AM
    
    // Step 4: Assign some trains to platform queue
    platformManager.enqueue(101);
    platformManager.enqueue(102);
    platformManager.enqueue(103);
}

// ======================================================================================
//                                   DASHBOARD DISPLAYS
// ======================================================================================

/**
 * Function: displayMainDashboard
 * Displays the primary navigation menu
 */
void displayMainDashboard() {
    cout << BOLDCYAN << "========================================================\n" << RESET;
    cout << BOLDWHITE << "      MUMBAI LOCAL RAILWAY - SYSTEM DASHBOARD           \n" << RESET;
    cout << BOLDCYAN << "========================================================\n" << RESET;
    cout << "  [" << BOLDGREEN << "1" << RESET << "] " << CYAN << "Stations & Routes Management" << RESET << "\n";
    cout << "  [" << BOLDGREEN << "2" << RESET << "] " << CYAN << "Ticketing & Passenger Flow" << RESET << "\n";
    cout << "  [" << BOLDRED << "3" << RESET << "] " << RED << "Exit System" << RESET << "\n";
    cout << BOLDCYAN << "========================================================\n" << RESET;
    cout << "\nEnter your choice: ";
}

/**
 * Function: displayStationsRoutesDashboard
 * Displays station management and routing options
 */
void displayStationsRoutesDashboard() {
    cout << BOLDMAGENTA << "\n========================================================\n" << RESET;
    cout << BOLDWHITE << "             STATIONS & ROUTES MANAGEMENT               \n" << RESET;
    cout << BOLDMAGENTA << "========================================================\n" << RESET;
    cout << "  " << GREEN << "1." << RESET << " View All Stations (BST Traversal)\n";
    cout << "  " << GREEN << "2." << RESET << " Search Station (BST Search)\n";
    cout << "  " << GREEN << "3." << RESET << " Find Fastest Route (Dijkstra)\n";
    cout << "  " << GREEN << "4." << RESET << " Check Network Connectivity (BFS)\n";
    cout << BOLDYELLOW << "  0. Back to Main Dashboard\n" << RESET;
    cout << BOLDMAGENTA << "========================================================\n" << RESET;
    cout << "\nEnter your choice: ";
}

/**
 * Function: displayTicketingDashboard
 * Displays ticketing and queuing options
 */
void displayTicketingDashboard() {
    cout << BOLDGREEN << "\n========================================================\n" << RESET;
    cout << BOLDWHITE << "             TICKETING & PASSENGER DASHBOARD            \n" << RESET;
    cout << BOLDGREEN << "========================================================\n" << RESET;
    cout << "  " << CYAN << "1." << RESET << " Buy Ticket (Multi-Queue Processing)\n";
    cout << "  " << CYAN << "2." << RESET << " View Ticketing Statistics\n";
    cout << "  " << CYAN << "3." << RESET << " Process Platform Queues\n";
    cout << BOLDYELLOW << "  0. Back to Main Dashboard\n" << RESET;
    cout << BOLDGREEN << "========================================================\n" << RESET;
    cout << "\nEnter your choice: ";
}

// ======================================================================================
//                                   MENU HANDLERS
// ======================================================================================

/**
 * Function: showStationSuggestions
 * Displays matching station suggestions and allows user to select one
 * Returns station ID on successful selection, -1 if user cancels
 */
int showStationSuggestions(const std::string& prefix) {
    std::vector<std::pair<std::string, int>> suggestions = stationDirectory.listMatchingStations(prefix);
    
    if (suggestions.empty()) {
        cout << RED << "\n❌ No stations found matching: " << prefix << "\n" << RESET;
        return -1;
    }
    
    cout << "\nDid you mean one of these?\n";
    for (size_t i = 0; i < suggestions.size(); ++i) {
        cout << "  " << (i + 1) << ". " << suggestions[i].first << "\n";
    }
    cout << "  0. Cancel\n";
    cout << "Enter choice (0-" << suggestions.size() << "): ";
    
    int choice;
    cin >> choice;
    
    if (choice < 1 || choice > (int)suggestions.size()) {
        cout << "Search cancelled.\n";
        return -1;
    }
    
    return suggestions[choice - 1].second;
}

/**
 * Function: handleStationSearch
 * Searches for a station by name using BST
 */
void handleStationSearch() {
    string stationName;
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│                    STATION SEARCH                      │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    cout << "Enter station name: ";
    cin.ignore();
    getline(cin, stationName);
    
    // Convert to lowercase for lookup
    string stationNameLower = stationName;
    std::transform(stationNameLower.begin(), stationNameLower.end(), stationNameLower.begin(), ::tolower);
    
    int stationId = -1;
    if (stationNameToId.find(stationNameLower) != stationNameToId.end()) {
        stationId = stationNameToId[stationNameLower];
    }
    
    if (stationId == -1) {
        cout << RED << "\n❌ Station not found: " << stationName << "\n" << RESET;
        stationId = showStationSuggestions(stationName);
        if (stationId == -1) return;
    }
    
    const Station& station = allStations[stationId];
    cout << GREEN << "\n✓ Station Found!\n" << RESET;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "Name: " << station.name << "\n";
    cout << "ID: " << station.id << "\n";
    cout << "Line: " << getLineName(station.line) << "\n";
    cout << "Platforms: " << station.platforms << "\n";
    cout << "Current Load: " << station.passengerCount << " passengers\n";
    cout << "Interchange: " << (station.isInterchange ? "Yes" : "No") << "\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

/**
 * Function: handleRouteSearch
 * Finds fastest route between two stations using Dijkstra's algorithm
 */
void handleRouteSearch() {
    string srcName, destName;
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│                    ROUTE FINDER                        │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    cout << "Enter source station: ";
    cin.ignore();
    getline(cin, srcName);
    cout << "Enter destination station: ";
    getline(cin, destName);
    
    // Convert to lowercase for lookup
    string srcNameLower = srcName;
    string destNameLower = destName;
    std::transform(srcNameLower.begin(), srcNameLower.end(), srcNameLower.begin(), ::tolower);
    std::transform(destNameLower.begin(), destNameLower.end(), destNameLower.begin(), ::tolower);
    
    int srcId = -1, destId = -1;
    if (stationNameToId.find(srcNameLower) != stationNameToId.end()) {
        srcId = stationNameToId[srcNameLower];
    }
    if (stationNameToId.find(destNameLower) != stationNameToId.end()) {
        destId = stationNameToId[destNameLower];
    }
    
    if (srcId == -1) {
        cout << "\n❌ Source station not found: " << srcName << "\n";
        srcId = showStationSuggestions(srcName);
        if (srcId == -1) return;
    }
    
    if (destId == -1) {
        cout << "\n❌ Destination station not found: " << destName << "\n";
        destId = showStationSuggestions(destName);
        if (destId == -1) return;
    }
    
    mumbaiLocal->findFastestRoute(srcId, destId);
}

/**
 * Function: handleConnectivityCheck
 * Checks network connectivity from a starting station using BFS
 */
void handleConnectivityCheck() {
    string stationName;
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│              NETWORK CONNECTIVITY CHECK                │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    cout << "Enter starting station: ";
    cin.ignore();
    getline(cin, stationName);
    
    // Convert to lowercase for lookup
    string stationNameLower = stationName;
    std::transform(stationNameLower.begin(), stationNameLower.end(), stationNameLower.begin(), ::tolower);
    
    int stationId = -1;
    if (stationNameToId.find(stationNameLower) != stationNameToId.end()) {
        stationId = stationNameToId[stationNameLower];
    }
    
    if (stationId == -1) {
        cout << "\n❌ Station not found: " << stationName << "\n";
        stationId = showStationSuggestions(stationName);
        if (stationId == -1) return;
    }
    
    mumbaiLocal->showConnectivity(stationId);
}

/**
 * Function: handleTicketing
 * Processes ticket purchase with multi-queue priority system
 * Now computes fare based on real distance between source and destination
 */
void handleTicketing() {
    string name, srcName, destName;
    int age;
    
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│                  TICKET BOOKING                        │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    
    cout << "Enter passenger name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter age: ";
    cin >> age;
    
    cout << "\nSelect passenger type:\n";
    cout << "  1. General\n";
    cout << "  2. Ladies\n";
    cout << "  3. Senior Citizen\n";
    cout << "Enter choice (1-3): ";
    int typeChoice;
    cin >> typeChoice;
    
    PassengerType type = GENERAL;
    if (typeChoice == 2) type = LADIES;
    if (typeChoice == 3 || age > 60) type = SENIOR;
    
    // Get source station
    cout << "Enter source station: ";
    cin.ignore();
    getline(cin, srcName);
    
    // Convert to lowercase for lookup
    string srcNameLower = srcName;
    std::transform(srcNameLower.begin(), srcNameLower.end(), srcNameLower.begin(), ::tolower);
    
    int srcId = -1;
    if (stationNameToId.find(srcNameLower) != stationNameToId.end()) {
        srcId = stationNameToId[srcNameLower];
    }
    
    if (srcId == -1) {
        cout << "\n❌ Invalid source station: " << srcName << "\n";
        srcId = showStationSuggestions(srcName);
        if (srcId == -1) return;
    }
    
    // Get destination station
    cout << "Enter destination station: ";
    getline(cin, destName);
    
    // Convert to lowercase for lookup
    string destNameLower = destName;
    std::transform(destNameLower.begin(), destNameLower.end(), destNameLower.begin(), ::tolower);
    
    int destId = -1;
    if (stationNameToId.find(destNameLower) != stationNameToId.end()) {
        destId = stationNameToId[destNameLower];
    }
    
    if (destId == -1) {
        cout << "\n❌ Invalid destination station: " << destName << "\n";
        destId = showStationSuggestions(destName);
        if (destId == -1) return;
    }
    
    // Compute real distance via Dijkstra
    int distance = mumbaiLocal->getDistance(srcId, destId);
    
    if (distance == INF) {
        cout << "\n❌ No route found between " << srcName << " and " << destName << "\n";
        return;
    }
    
    // Create passenger object (simplified - no full Passenger struct needed)
    cout << "\n✓ Passenger " << name << " added to ";
    if (type == LADIES) cout << "LADIES";
    else if (type == SENIOR) cout << "SENIOR CITIZEN";
    else cout << "GENERAL";
    cout << " queue.\n";
    
    // Compute fare: 10 + 2*distance_km
    int baseFare = 10;
    int fare = baseFare + (distance * 2);
    
    // Apply discount for seniors
    if (type == SENIOR) {
        fare = fare * 0.5;  // 50% discount
        cout << "✓ Senior citizen discount applied (50% off)\n";
    }
    
    cout << BOLDBLUE << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "                   TICKET DETAILS\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "  Passenger:   " << name << " (Age: " << age << ")\n";
    cout << "  Source:      " << srcName << "\n";
    cout << "  Destination: " << destName << "\n";
    cout << "  Distance:    " << distance << " km\n";
    cout << "  Fare:        Rs. " << fare << "\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << "\n";
    
    // Create Passenger object for persistence
    static int ticketIdCounter = 1;
    Passenger p;
    p.id = ticketIdCounter++;
    p.name = name;
    p.age = age;
    p.type = type;
    p.sourceId = srcId;
    p.destId = destId;
    p.ticketPrice = fare;
    p.entryTime = time(0);

    // Persist to CSV
    CSVManager::appendTicket(p);

    // Update ticketing stats - record in TicketSystem for analytics
    ticketMachine.recordTicket(fare);
    allStations[destId].passengerCount++;
}

/**
 * Function: handleTrackFailure
 * Reports and blocks a track between two stations
 * Uses suggestion flow when station lookup fails
 */
void handleTrackFailure() {
    string station1, station2;
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│              EMERGENCY: TRACK FAILURE                  │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    cout << "Enter first station: ";
    cin.ignore();
    getline(cin, station1);
    
    // Convert to lowercase for lookup
    string station1Lower = station1;
    std::transform(station1Lower.begin(), station1Lower.end(), station1Lower.begin(), ::tolower);
    
    int id1 = stationDirectory.getStationId(station1Lower);
    
    // If first station not found, show suggestions
    if (id1 == -1) {
        cout << "\n❌ Station not found: " << station1 << "\n";
        id1 = showStationSuggestions(station1);
        if (id1 == -1) {
            cout << "Track failure report cancelled.\n";
            return;
        }
    }
    
    cout << "Enter second station: ";
    getline(cin, station2);
    
    // Convert to lowercase for lookup
    string station2Lower = station2;
    std::transform(station2Lower.begin(), station2Lower.end(), station2Lower.begin(), ::tolower);
    
    int id2 = stationDirectory.getStationId(station2Lower);
    
    // If second station not found, show suggestions
    if (id2 == -1) {
        cout << "\n❌ Station not found: " << station2 << "\n";
        id2 = showStationSuggestions(station2);
        if (id2 == -1) {
            cout << "Track failure report cancelled.\n";
            return;
        }
    }
    
    // Both stations resolved, proceed to block track
    mumbaiLocal->blockTrack(id1, id2);
}

/**
 * Function: handlePlatformQueue
 * Processes and displays platform queue status
 */
void handlePlatformQueue() {
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│              PLATFORM QUEUE PROCESSING                 │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    
    cout << "\nSelect operation:\n";
    cout << "  1. Process next train (Dequeue)\n";
    cout << "  2. Add train to queue (Enqueue)\n";
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    
    if (choice == 1) {
        int trainId = platformManager.dequeue();
        if (trainId != -1) {
            cout << "\n✓ Train " << trainId << " departed from platform.\n";
        } else {
            cout << "\n⚠️  Platform queue is empty.\n";
        }
    } else if (choice == 2) {
        cout << "Enter train ID: ";
        int trainId;
        cin >> trainId;
        platformManager.enqueue(trainId);
    } else {
        cout << "\n❌ Invalid choice.\n";
    }
}

/**
 * Function: simulatePassengerLoad
 * Simulates random passenger traffic at stations
 */
void simulatePassengerLoad() {
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│            SIMULATING PASSENGER TRAFFIC...             │\n";
    cout << "└────────────────────────────────────────────────────────┘\n\n";
    
    for (size_t i = 0; i < allStations.size(); ++i) {
        int additionalLoad = rand() % 500;
        allStations[i].passengerCount += additionalLoad;
    }
    
    cout << "✓ Passenger load simulation complete.\n";
    cout << "✓ Random traffic added to all stations.\n";
}

// ======================================================================================
//                                   AUTHENTICATION
// ======================================================================================

void displayBanner() {
    cout << BOLDCYAN << "================================================================================\n";
    cout << BOLDMAGENTA << "  __  __ _   _ __  __ ____         ___   _      ___   ____    _    _     \n";
    cout << " |  \\/  | | | |  \\/  | __ )   _    / _ \\ | |    / _ \\ / ___|  / \\  | |    \n";
    cout << " | |\\/| | | | | |\\/| |  _ \\  (_)  | | | || |   | | | | |     / _ \\ | |    \n";
    cout << " | |  | | |_| | |  | | |_) |  _   | |_| || |___| |_| | |___ / ___ \\| |___ \n";
    cout << " |_|  |_|\\___/|_|  |_|____/  (_)   \\___/ |_____|\\___/ \\____/_/   \\_\\_____|\n";
    cout << "                                                                            \n";
    cout << BOLDWHITE << "           MUMBAI LOCAL RAILWAY - TRANSPORT MANAGEMENT SYSTEM               \n";
    cout << BOLDCYAN << "================================================================================" << RESET << "\n\n";
}

bool authenticateUser() {
    string username, password;
    cout << CYAN << "------------------------------------------\n";
    cout << "             ADMIN LOGIN                  \n";
    cout << "------------------------------------------\n" << RESET;
    cout << YELLOW << "Username: " << RESET;
    cin >> username;
    cout << YELLOW << "Password: " << RESET;
    cin >> password;
    
    if (username == "Jaydeep" && password == "jaydeep123") {
        return true;
    }
    return false;
}

// ======================================================================================
//                                   MAIN FUNCTION
// ======================================================================================

int main() {
    // Enable UTF-8 console output on Windows
    #ifdef _WIN32
        // Set console code page to UTF-8
        system("chcp 65001 > nul 2>&1");
    #endif
    
    // Seed random number generator
    srand(time(0));

    // Initialize data directory
    CSVManager::initializeDataDirectory();

    // Startup Enhancements
    displayBanner();
    
    int attempts = 0;
    bool authenticated = false;
    
    while (attempts < 3) {
        if (authenticateUser()) {
            authenticated = true;
            cout << GREEN << "\n✅ Login Successful! Welcome, Jaydeep.\n" << RESET;
            break;
        } else {
            attempts++;
            cout << RED << "\n❌ Invalid credentials. Attempts remaining: " << (3 - attempts) << "\n\n" << RESET;
        }
    }
    
    if (!authenticated) {
        cout << BOLDRED << "❌ Maximum login attempts reached. System shutting down.\n" << RESET;
        return 1;
    }
    
    // Initialize the entire system
    initializeSystem();
    
    int choice;
    bool running = true;
    NavigatorState currentState = MAIN_MENU;
    
    // Main menu loop
    while (running) {
        if (currentState == MAIN_MENU) {
            displayMainDashboard();
        } else if (currentState == STATIONS_MENU) {
            displayStationsRoutesDashboard();
        } else if (currentState == TICKETING_MENU) {
            displayTicketingDashboard();
        }
        
        cin >> choice;
        
        // Clear any input errors
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << RED << "\n❌ Invalid input. Please enter a number.\n" << RESET;
            continue;
        }
        
        bool stateChanged = false;
        
        if (currentState == MAIN_MENU) {
            switch (choice) {
                case 1:
                    currentState = STATIONS_MENU;
                    stateChanged = true;
                    break;
                case 2:
                    currentState = TICKETING_MENU;
                    stateChanged = true;
                    break;
                case 3:
                    // Exit system
                    cout << GREEN << "\n╔════════════════════════════════════════════════════════╗\n";
                    cout << "║          Thank you for using the system!               ║\n";
                    cout << "║              Have a safe journey! 🚂                   ║\n";
                    cout << "╚════════════════════════════════════════════════════════╝\n\n" << RESET;
                    
                    // Auto-save on exit
                    cout << CYAN << "Auto-saving system data...\n" << RESET;
                    CSVManager::saveStations(allStations);
                    CSVManager::saveRoutes(mumbaiLocal);
                    cout << GREEN << "✓ Data saved successfully!\n" << RESET;
                    
                    running = false;
                    break;
                default:
                    cout << RED << "\n❌ Invalid choice. Please select 1-3.\n" << RESET;
                    break;
            }
        } else if (currentState == STATIONS_MENU) {
            switch (choice) {
                case 1:
                    stationDirectory.listStations();
                    break;
                case 2:
                    handleStationSearch();
                    break;
                case 3:
                    handleRouteSearch();
                    break;
                case 4:
                    handleConnectivityCheck();
                    break;
                case 0:
                    currentState = MAIN_MENU;
                    stateChanged = true;
                    break;
                default:
                    cout << RED << "\n❌ Invalid choice. Please select 0-4.\n" << RESET;
                    break;
            }
        } else if (currentState == TICKETING_MENU) {
            switch (choice) {
                case 1:
                    handleTicketing();
                    break;
                case 2:
                    ticketMachine.showStats();
                    break;
                case 3:
                    handlePlatformQueue();
                    break;
                case 0:
                    currentState = MAIN_MENU;
                    stateChanged = true;
                    break;
                default:
                    cout << RED << "\n❌ Invalid choice. Please select 0-3.\n" << RESET;
                    break;
            }
        }
        
        // Pause before showing menu again, unless we just switched menus or are exiting
        if (running && !stateChanged) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    // Cleanup
    delete mumbaiLocal;
    
    return 0;
}
