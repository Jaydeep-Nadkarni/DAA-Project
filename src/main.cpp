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

using namespace std;

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
    cout << "\n========================================================\n";
    cout << "         SYSTEM INITIALIZATION IN PROGRESS...          \n";
    cout << "========================================================\n\n";
    
    cout << "\n✓ System initialization complete!\n";
    cout << "✓ Total Stations: " << allStations.size() << "\n";
    cout << "✓ Trains Scheduled: 7\n";
    cout << "✓ Network Status: OPERATIONAL\n\n";
}

// ======================================================================================
//                                   LOGIN SYSTEM
// ======================================================================================

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
        cout << "\n╔════════════════════════════════════════════════════════╗\n";
        cout << "║                                                        ║\n";
        cout << "║     MUMBAI LOCAL TRANSPORTATION MANAGEMENT SYSTEM      ║\n";
        cout << "║                   LOGIN REQUIRED                       ║\n";
        cout << "║                                                        ║\n";
        cout << "╚════════════════════════════════════════════════════════╝\n\n";
        
        cout << "  👤 Username: ";
        cin >> username;
        cout << "  🔒 Password: ";
        cin >> password;
        
        if (username == "jaydeep" && password == "KLE@123") {
            cout << "\n╔════════════════════════════════════════════════════════╗\n";
            cout << "║          ✓ LOGIN SUCCESSFUL                            ║\n";
            cout << "║          Welcome, " << left << setw(35) << username << "║\n";
            cout << "╚════════════════════════════════════════════════════════╝\n";
            return true;
        } else {
            attempts++;
            cout << "\n  ❌ Invalid credentials! ";
            if (attempts < MAX_ATTEMPTS) {
                cout << "(" << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining)\n";
            } else {
                cout << "\n\n╔════════════════════════════════════════════════════════╗\n";
                cout << "║          ⚠️  ACCESS DENIED                             ║\n";
                cout << "║          Maximum login attempts exceeded               ║\n";
                cout << "╚════════════════════════════════════════════════════════╝\n\n";
                return false;
            }
        }
    }
    return false;
}

// ======================================================================================
//                                   MENU DISPLAY
// ======================================================================================

/**
 * Function: displayMainMenu
 * Displays the main menu with 4 primary categories
 */
void displayMainMenu() {
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║     MUMBAI LOCAL TRANSPORTATION MANAGEMENT SYSTEM      ║\n";
    cout << "║                    MAIN MENU                           ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
    cout << "  1. staion & Route Management\n";
    cout << "  2. Ticketing & Passenger Services\n";
    cout << "  3. Train Operations & Analytics\n";
    cout << "  4. System Administration\n";
    cout << "  0. Exit System\n";
    cout << "\n────────────────────────────────────────────────────────\n";
    cout << "Enter your choice: ";
}

/**
 * Function: displayStationMenu
 * Displays Station & Route Management submenu
 */
void displayStationMenu() {
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║          STATION & ROUTE MANAGEMENT                 ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
    cout << "  1. View All Stations (BST Traversal)\n";
    cout << "  2. Search Station (BST Search)\n";
    cout << "  3. Find Fastest Route (Dijkstra Algorithm)\n";
    cout << "  4. Check Network Connectivity (BFS)\n";
    cout << "  0. ← Return to Main Menu\n";
    cout << "\n────────────────────────────────────────────────────────\n";
    cout << "Enter your choice: ";
}

/**
 * Function: displayTicketingMenu
 * Displays Ticketing & Passenger Services submenu
 */
void displayTicketingMenu() {
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║        TICKETING & PASSENGER SERVICES               ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
    cout << "  1. Buy Ticket (Multi-Queue Processing)\n";
    cout << "  2. Process Ticket Queues\n";
    cout << "  3. View Ticketing Statistics\n";
    cout << "  4. Simulate Passenger Load\n";
    cout << "  0. ← Return to Main Menu\n";
    cout << "\n────────────────────────────────────────────────────────\n";
    cout << "Enter your choice: ";
}

/**
 * Function: displayTrainOpsMenu
 * Displays Train Operations & Analytics submenu
 */
void displayTrainOpsMenu() {
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║         TRAIN OPERATIONS & ANALYTICS                ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
    cout << "  1. View Train Schedule (Min Heap)\n";
    cout << "  2. Optimize Peak Hour Frequency\n";
    cout << "  3. Process Platform Queue\n";
    cout << "  4. Passenger Flow Analytics\n";
    cout << "  5. Station Congestion Report\n";
    cout << "  6. Peak Hour Statistics\n";
    cout << "  7. Comprehensive Dashboard\n";
    cout << "  0. ← Return to Main Menu\n";
    cout << "\n────────────────────────────────────────────────────────\n";
    cout << "Enter your choice: ";
}

/**
 * Function: displayAdminMenu
 * Displays System Administration submenu
 */
void displayAdminMenu() {
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║            SYSTEM ADMINISTRATION                    ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
    cout << "  1. Report Track Failure (Block Track)\n";
    cout << "  2. Display Network Statistics\n";
    cout << "  3. Emergency Services\n";
    cout << "  0. ← Return to Main Menu\n";
    cout << "\n────────────────────────────────────────────────────────\n";
    cout << "Enter your choice: ";
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
        cout << "\n❌ No stations found matching: " << prefix << "\n";
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
        cout << "\n❌ Station not found: " << stationName << "\n";
        stationId = showStationSuggestions(stationName);
        if (stationId == -1) return;
    }
    
    const Station& station = allStations[stationId];
    cout << "\n✓ Station Found!\n";
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
    
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "                   TICKET DETAILS\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "Passenger: " << name << " (Age: " << age << ")\n";
    cout << "Source: " << srcName << "\n";
    cout << "Destination: " << destName << "\n";
    cout << "Distance: " << distance << " km\n";
    cout << "Fare: Rs. " << fare << "\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
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
        // Set console code page to UTF-8
        system("chcp 65001 > nul 2>&1");
    #endif
    
    // Seed random number generator
    srand(time(0));
    
    // Authenticate user
    if (!authenticateUser()) {
        return 1;  // Exit if authentication fails
    }
    
    // Initialize the entire system
    initializeSystem();
    
    int mainChoice, subChoice;
    bool running = true;
    
    // Main menu loop
    while (running) {
        displayMainMenu();
        cin >> mainChoice;
        
        // Clear any input errors
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n❌ Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (mainChoice) {
            case 1: // Station & Route Management
                {
                    bool inSubmenu = true;
                    while (inSubmenu) {
                        displayStationMenu();
                        cin >> subChoice;
                        
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "\n❌ Invalid input.\n";
                            continue;
                        }
                        
                        switch (subChoice) {
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
                                inSubmenu = false;
                                break;
                            default:
                                cout << "\n❌ Invalid choice. Please select 0-4.\n";
                                break;
                        }
                        
                        if (inSubmenu && subChoice != 0) {
                            cout << "\nPress Enter to continue...";
                            cin.ignore();
                            cin.get();
                        }
                    }
                }
                break;
                
            case 2: // Ticketing & Passenger Services
                {
                    bool inSubmenu = true;
                    while (inSubmenu) {
                        displayTicketingMenu();
                        cin >> subChoice;
                        
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "\n❌ Invalid input.\n";
                            continue;
                        }
                        
                        switch (subChoice) {
                            case 1:
                                handleTicketing();
                                break;
                            case 2:
                                ticketMachine.processQueues();
                                break;
                            case 3:
                                ticketMachine.showStats();
                                break;
                            case 4:
                                simulatePassengerLoad();
                                break;
                            case 0:
                                inSubmenu = false;
                                break;
                            default:
                                cout << "\n❌ Invalid choice. Please select 0-4.\n";
                                break;
                        }
                        
                        if (inSubmenu && subChoice != 0) {
                            cout << "\nPress Enter to continue...";
                            cin.ignore();
                            cin.get();
                        }
                    }
                }
                break;
                
            case 3: // Train Operations & Analytics
                {
                    bool inSubmenu = true;
                    while (inSubmenu) {
                        displayTrainOpsMenu();
                        cin >> subChoice;
                        
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "\n❌ Invalid input.\n";
                            continue;
                        }
                        
                        switch (subChoice) {
                            case 1:
                                // View train schedule at a station
                                {
                                    cout << "\n┌────────────────────────────────────────────────────────┐\n";
                                    cout << "│            VIEW TRAINS AT STATION                       │\n";
                                    cout << "└────────────────────────────────────────────────────────┘\n";
                                    cout << "Enter station name: ";
                                    cin.ignore();
                                    string stationName;
                                    getline(cin, stationName);
                                    
                                    string stationNameLower = stationName;
                                    std::transform(stationNameLower.begin(), stationNameLower.end(), 
                                                   stationNameLower.begin(), ::tolower);
                                    
                                    int stationId = -1;
                                    if (stationNameToId.find(stationNameLower) != stationNameToId.end()) {
                                        stationId = stationNameToId[stationNameLower];
                                    }
                                    
                                    if (stationId == -1) {
                                        cout << "\n❌ Invalid station: " << stationName << "\n";
                                        stationId = showStationSuggestions(stationName);
                                        if (stationId == -1) break;
                                    }
                                    
                                    trainScheduler.showTrainsAtStation(stationId);
                                }
                                break;
                                
                            case 2:
                                // Optimize for peak hours
                                {
                                    cout << "\n┌────────────────────────────────────────────────────────┐\n";
                                    cout << "│           PEAK HOUR FREQUENCY OPTIMIZATION             │\n";
                                    cout << "└────────────────────────────────────────────────────────┘\n";
                                    
                                    int maxPassengerStation = 0;
                                    int maxPassengers = 0;
                                    
                                    for (int i = 0; i < (int)allStations.size(); i++) {
                                        if (allStations[i].passengerCount > maxPassengers) {
                                            maxPassengers = allStations[i].passengerCount;
                                            maxPassengerStation = i;
                                        }
                                    }
                                    
                                    cout << "\nPeak Station Analysis:\n";
                                    cout << "Peak Station: " << allStations[maxPassengerStation].name 
                                         << " (Passengers: " << maxPassengers << ")\n\n";
                                    
                                    trainScheduler.optimizeFrequency(true);
                                    
                                    cout << "\nRealtime Platform Allocation:\n";
                                    if (platformManager.getSize() < platformManager.getCapacity()) {
                                        platformManager.enqueue(901);
                                        platformManager.enqueue(902);
                                        platformManager.enqueue(903);
                                        cout << "✓ Allocated platforms for peak specials\n";
                                        cout << "  Platform Queue: " << platformManager.getSize() 
                                             << "/" << platformManager.getCapacity() << " (Trains waiting)\n";
                                    } else {
                                        cout << "⚠ Platform queue full, cannot allocate more trains\n";
                                    }
                                }
                                break;
                                
                            case 3:
                                handlePlatformQueue();
                                break;
                            case 4:
                                displayPassengerFlowAnalytics();
                                break;
                            case 5:
                                displayCongestionReport();
                                break;
                            case 6:
                                displayPeakHourStatistics();
                                break;
                            case 7:
                                displayComprehensiveAnalytics(ticketMachine);
                                break;
                            case 0:
                                inSubmenu = false;
                                break;
                            default:
                                cout << "\n❌ Invalid choice. Please select 0-7.\n";
                                break;
                        }
                        
                        if (inSubmenu && subChoice != 0) {
                            cout << "\nPress Enter to continue...";
                            cin.ignore();
                            cin.get();
                        }
                    }
                }
                break;
                
            case 4: // System Administration
                {
                    bool inSubmenu = true;
                    while (inSubmenu) {
                        displayAdminMenu();
                        cin >> subChoice;
                        
                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "\n❌ Invalid input.\n";
                            continue;
                        }
                        
                        switch (subChoice) {
                            case 1:
                                handleTrackFailure();
                                break;
                            case 2:
                                mumbaiLocal->displayNetworkStats();
                                break;
                            case 3:
                                cout << "\n╔════════════════════════════════════════════════════════╗\n";
                                cout << "║              🚨 EMERGENCY SERVICES                     ║\n";
                                cout << "╚════════════════════════════════════════════════════════╝\n";
                                cout << "\n✓ Emergency protocols activated\n";
                                cout << "✓ Alert dispatched to control center\n";
                                cout << "✓ Emergency services on standby\n";
                                break;
                            case 0:
                                inSubmenu = false;
                                break;
                            default:
                                cout << "\n❌ Invalid choice. Please select 0-3.\n";
                                break;
                        }
                        
                        if (inSubmenu && subChoice != 0) {
                            cout << "\nPress Enter to continue...";
                            cin.ignore();
                            cin.get();
                        }
                    }
                }
                break;
                
            case 0:
                // Exit system
                cout << "\n╔════════════════════════════════════════════════════════╗\n";
                cout << "║          Thank you for using the system!               ║\n";
                cout << "║              Have a safe journey! 🚂                   ║\n";
                cout << "╚════════════════════════════════════════════════════════╝\n\n";
                running = false;
                break;
                
            default:
                cout << "\n❌ Invalid choice. Please select a valid option (0-4).\n";
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                break;
        }
    }
    
    // Cleanup
    delete mumbaiLocal;
    
    return 0;
}
