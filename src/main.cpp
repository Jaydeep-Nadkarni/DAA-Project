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

enum NavigatorState { MAIN_MENU, STATIONS_MENU, TICKETING_MENU, ANALYTICS_MENU };

// Dashboard and Navigation Functions
void displayMainDashboard();
void displayStationsRoutesDashboard();
void displayTicketingDashboard();
void displayAnalyticsDashboard();

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
 * Initializes the Mumbai Local Railway system with persistence support
 */
void initializeSystem() {
    cout << BOLDBLUE << "\n========================================================\n";
    cout << "         " << BOLDWHITE << "SYSTEM INITIALIZATION IN PROGRESS..." << BOLDBLUE << "          \n";
    cout << "========================================================\n" << RESET;
    
    // Step 1: Create railway network graph
    mumbaiLocal = new RailwayNetwork(MAX_STATIONS);
    
    // Step 2 & 3: Try to load from CSV; otherwise fallback to default
    cout << YELLOW << "Loading station network..." << RESET << "\n";
    
    // Attempt to load from CSV first
    if (CSVManager::loadStations(allStations) && !allStations.empty()) {
        cout << GREEN << "✓ Loaded " << allStations.size() << " stations from data/stations.csv" << RESET << "\n";
        
        // Re-populate BST and lookup maps from loaded vector
        for (const auto& s : allStations) {
            stationDirectory.addStation(s.name, s.id);
            string nameL = s.name;
            std::transform(nameL.begin(), nameL.end(), nameL.begin(), ::tolower);
            stationNameToId[nameL] = s.id;
            stationIdToName[s.id] = s.name;
        }
        
        // Load routes into the graph
        if (CSVManager::loadRoutes(mumbaiLocal)) {
            cout << GREEN << "✓ Loaded network connections from data/routes.csv" << RESET << "\n";
        }
    } else {
        cout << YELLOW << "No station data found. Initializing with default Mumbai network..." << RESET << "\n";
        initializeStations(stationDirectory, mumbaiLocal);
        cout << GREEN << "✓ Default network initialized with " << allStations.size() << " stations.\n" << RESET;
        
        // Save for next time
        CSVManager::saveStations(allStations);
        CSVManager::saveRoutes(mumbaiLocal);
    }
    
    // Step 4: Schedule initial trains (Static for demo)
    cout << YELLOW << "Scheduling initial trains..." << RESET << "\n";
    if (stationNameToId.count("churchgate")) 
        trainScheduler.scheduleTrain(101, "Churchgate Fast", 360, stationNameToId["churchgate"]);
    if (stationNameToId.count("virar"))
        trainScheduler.scheduleTrain(102, "Virar Slow", 375, stationNameToId["virar"]);
    if (stationNameToId.count("dadar"))
        trainScheduler.scheduleTrain(201, "Dadar Special", 480, stationNameToId["dadar"]);
    
    // Step 5: Assign some trains to platform queue
    platformManager.enqueue(101);
    platformManager.enqueue(102);
    
    cout << "\n" << GREEN << "✓ System ready for operations." << RESET << "\n\n";
}

// ======================================================================================
//                                   LOGIN SYSTEM
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

/**
 * Function: authenticateUser
 * Displays login screen and validates credentials
 * Returns true if authentication successful, false otherwise
 */
bool authenticateUser() {
    string username, password;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;
    
    while (attempts < MAX_ATTEMPTS) {
        cout << CYAN << "------------------------------------------\n";
        cout << "             ADMIN LOGIN                  \n";
        cout << "------------------------------------------\n" << RESET;
        
        cout << YELLOW << "  👤 Username: " << RESET;
        cin >> username;
        cout << YELLOW << "  🔒 Password: " << RESET;
        cin >> password;
        
        if (username == "Jaydeep" && password == "jaydeep123") {
            cout << GREEN << "\n✓ Login Successful! Welcome, " << username << ".\n" << RESET;
            return true;
        } else {
            attempts++;
            cout << RED << "\n❌ Invalid credentials! ";
            if (attempts < MAX_ATTEMPTS) {
                cout << "(" << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining)\n" << RESET;
            } else {
                cout << "\n⚠️  ACCESS DENIED: Maximum attempts exceeded.\n" << RESET;
                return false;
            }
        }
    }
    return false;
}

// ======================================================================================
//                                   DASHBOARD DISPLAYS
// ======================================================================================

/**
 * Navigation Dashboard Functions
 * Implements a hierarchical UI for better user experience
 */

void displayMainDashboard() {
    cout << BOLDCYAN << "\n" << "╔════════════════════════════════════════════════════════╗" << "\n";
    cout << "║             " << BOLDWHITE << "SYSTEM CONTROL DASHBOARD" << BOLDCYAN << "                   ║" << "\n";
    cout << "╚════════════════════════════════════════════════════════╝" << RESET << "\n";
    cout << "  1. Stations & Network Management\n";
    cout << "  2. Ticketing & Passenger Services\n";
    cout << "  3. Real-time System Analytics \n";
    cout << "  4. Administrative & Emergency Operations\n";
    cout << "  0. Exit System\n";
    cout << "--------------------------------------------------------\n";
    cout << "Enter category choice: ";
}

void displayStationsRoutesDashboard() {
    cout << BOLDCYAN << "\n" << "╔════════════════════════════════════════════════════════╗" << "\n";
    cout << "║             " << BOLDWHITE << "STATIONS & NETWORK MANAGEMENT" << BOLDCYAN << "             ║" << "\n";
    cout << "╚════════════════════════════════════════════════════════╝" << RESET << "\n";
    cout << "  1. View All Stations (BST Traversal)\n";
    cout << "  2. Search Station (BST Search)\n";
    cout << "  3. Find Fastest Route (Dijkstra's)\n";
    cout << "  4. Check Network Connectivity (BFS)\n";
    cout << "  5. View Network Statistics\n";
    cout << "  9. Back to Main Menu\n";
    cout << "  0. Exit\n";
    cout << "--------------------------------------------------------\n";
    cout << "Enter operation: ";
}

void displayTicketingDashboard() {
    cout << BOLDCYAN << "\n" << "╔════════════════════════════════════════════════════════╗" << "\n";
    cout << "║             " << BOLDWHITE << "TICKETING & PASSENGER SERVICES" << BOLDCYAN << "            ║" << "\n";
    cout << "╚════════════════════════════════════════════════════════╝" << RESET << "\n";
    cout << "  1. Buy Ticket (Booking Queue)\n";
    cout << "  2. View Ticketing Sales Statistics\n";
    cout << "  3. View Train Schedules (Min-Heap)\n";
    cout << "  4. Process Platform Arrivals (Circular Queue)\n";
    cout << "  5. Simulate Passenger Load\n";
    cout << "  9. Back to Main Menu\n";
    cout << "  0. Exit\n";
    cout << "--------------------------------------------------------\n";
    cout << "Enter operation: ";
}

void displayAnalyticsDashboard() {
    cout << BOLDCYAN << "\n" << "╔════════════════════════════════════════════════════════╗" << "\n";
    cout << "║             " << BOLDWHITE << "REAL-TIME SYSTEM ANALYTICS" << BOLDCYAN << "                ║" << "\n";
    cout << "╚════════════════════════════════════════════════════════╝" << RESET << "\n";
    cout << "  1. Passenger Flow Analysis\n";
    cout << "  2. Station Congestion Heatmap\n";
    cout << "  3. Peak Hour Performance Stats\n";
    cout << "  4. Comprehensive System Dashboard\n";
    cout << "  9. Back to Main Menu\n";
    cout << "  0. Exit\n";
    cout << "--------------------------------------------------------\n";
    cout << "Enter analytics choice: ";
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
//                                   MAIN FUNCTION
// ======================================================================================

int main() {
    // Enable UTF-8 console output on Windows
    #ifdef _WIN32
        system("chcp 65001 > nul 2>&1");
    #endif
    
    srand(time(0));
    displayBanner();
    
    if (!authenticateUser()) {
        cout << BOLDRED << "\n❌ Authentication Failed! Access Denied.\n" << RESET;
        return 1;
    }
    
    cout << GREEN << "\n✓ Authentication Successful! Welcome, Jaydeep.\n" << RESET;
    
    // Initialize the entire system
    initializeSystem();
    
    int mainChoice, subChoice;
    bool running = true;
    NavigatorState currentState = MAIN_MENU;
    
    while (running) {
        if (currentState == MAIN_MENU) {
            displayMainDashboard();
            if (!(cin >> mainChoice)) {
                cin.clear(); cin.ignore(10000, '\n');
                continue;
            }
            
            switch (mainChoice) {
                case 1: currentState = STATIONS_MENU; break;
                case 2: currentState = TICKETING_MENU; break;
                case 3: currentState = ANALYTICS_MENU; break;
                case 4:
                    handleTrackFailure();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    cout << RED << "❌ Invalid category.\n" << RESET;
            }
        } 
        else if (currentState == STATIONS_MENU) {
            displayStationsRoutesDashboard();
            if (!(cin >> subChoice)) {
                cin.clear(); cin.ignore(10000, '\n');
                continue;
            }
            
            switch (subChoice) {
                case 1: stationDirectory.listStations(); break;
                case 2: handleStationSearch(); break;
                case 3: handleRouteSearch(); break;
                case 4: handleConnectivityCheck(); break;
                case 5: mumbaiLocal->displayNetworkStats(); break;
                case 9: currentState = MAIN_MENU; break;
                case 0: running = false; break;
                default: cout << RED << "❌ Invalid option.\n" << RESET;
            }
        }
        else if (currentState == TICKETING_MENU) {
            displayTicketingDashboard();
            if (!(cin >> subChoice)) {
                cin.clear(); cin.ignore(10000, '\n');
                continue;
            }
            
            switch (subChoice) {
                case 1: handleTicketing(); break;
                case 2: ticketMachine.showStats(); break;
                case 3: 
                    {
                        cout << "Enter station name: ";
                        cin.ignore();
                        string name; getline(cin, name);
                        string nameL = name;
                        std::transform(nameL.begin(), nameL.end(), nameL.begin(), ::tolower);
                        int id = stationDirectory.getStationId(nameL);
                        if (id != -1) trainScheduler.showTrainsAtStation(id);
                        else cout << RED << "Station not found.\n" << RESET;
                    }
                    break;
                case 4: handlePlatformQueue(); break;
                case 5: simulatePassengerLoad(); break;
                case 9: currentState = MAIN_MENU; break;
                case 0: running = false; break;
                default: cout << RED << "❌ Invalid option.\n" << RESET;
            }
        }
        else if (currentState == ANALYTICS_MENU) {
            displayAnalyticsDashboard();
            if (!(cin >> subChoice)) {
                cin.clear(); cin.ignore(10000, '\n');
                continue;
            }
            
            switch (subChoice) {
                case 1: displayPassengerFlowAnalytics(); break;
                case 2: displayCongestionReport(); break;
                case 3: displayPeakHourStatistics(); break;
                case 4: displayComprehensiveAnalytics(ticketMachine); break;
                case 9: currentState = MAIN_MENU; break;
                case 0: running = false; break;
                default: cout << RED << "❌ Invalid option.\n" << RESET;
            }
        }

        if (running && currentState == MAIN_MENU && mainChoice == 0) running = false;
        
        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore(10000, '\n');
            cin.get();
        }
    }
    
    // Auto-save on exit
    cout << YELLOW << "\nSaving system state..." << RESET << "\n";
    CSVManager::saveStations(allStations);
    CSVManager::saveRoutes(mumbaiLocal);
    
    cout << BOLDCYAN << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║          " << BOLDWHITE << "Thank you for using the system!" << BOLDCYAN << "               ║\n";
    cout << "║              " << BOLDWHITE << "Have a safe journey! 🚂" << BOLDCYAN << "                   ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n" << RESET;
    
    delete mumbaiLocal;
    return 0;
}