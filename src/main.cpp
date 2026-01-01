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
//                                   PASSENGER TYPE HELPER
// ======================================================================================

enum PassengerType { GENERAL, LADIES, SENIOR, DISABILITY };

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
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║         SYSTEM INITIALIZATION IN PROGRESS...          ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
    
    // Step 1: Initialize stations and global data structures
    cout << "[1/5] Initializing stations across all lines...\n";
    initializeStations();  // From station.cpp - populates allStations
    
    // Step 2: Build station directory (BST)
    cout << "[2/5] Building station directory (BST)...\n";
    for (const auto& station : allStations) {
        stationDirectory.addStation(station.name, station.id);
    }
    
    // Step 3: Create railway network graph
    cout << "[3/5] Creating railway network graph...\n";
    mumbaiLocal = new RailwayNetwork(allStations.size());
    
    // Step 4: Add tracks between stations
    cout << "[4/5] Connecting stations with tracks...\n";
    
    // Connect Western Line stations
    vector<string> westernStations = {
        "Churchgate", "Marine Lines", "Charni Road", "Grant Road", 
        "Mumbai Central", "Dadar", "Bandra", "Andheri", "Borivali", "Virar"
    };
    for (size_t i = 0; i < westernStations.size() - 1; ++i) {
        int u = stationNameToId[westernStations[i]];
        int v = stationNameToId[westernStations[i + 1]];
        mumbaiLocal->addTrack(u, v, 3 + (rand() % 3));
    }
    
    // Connect Central Line stations
    vector<string> centralStations = {
        "CST", "Masjid", "Sandhurst Road", "Byculla", "Dadar", 
        "Kurla", "Ghatkopar", "Thane", "Kalyan", "Dombivli"
    };
    for (size_t i = 0; i < centralStations.size() - 1; ++i) {
        int u = stationNameToId[centralStations[i]];
        int v = stationNameToId[centralStations[i + 1]];
        mumbaiLocal->addTrack(u, v, 4 + (rand() % 3));
    }
    
    // Connect Harbour Line stations
    vector<string> harbourStations = {
        "CST", "Dockyard Road", "Govandi", "Mankhurd", 
        "Vashi", "Nerul", "Belapur", "Panvel"
    };
    for (size_t i = 0; i < harbourStations.size() - 1; ++i) {
        int u = stationNameToId[harbourStations[i]];
        int v = stationNameToId[harbourStations[i + 1]];
        mumbaiLocal->addTrack(u, v, 5 + (rand() % 3));
    }
    
    // Step 5: Schedule initial trains
    cout << "[5/5] Scheduling initial trains...\n";
    
    // Morning trains
    trainScheduler.scheduleTrain(101, "Churchgate Fast", 360, stationNameToId["Churchgate"]);   // 6:00 AM
    trainScheduler.scheduleTrain(102, "Virar Slow", 375, stationNameToId["Virar"]);            // 6:15 AM
    trainScheduler.scheduleTrain(103, "CST Express", 420, stationNameToId["CST"]);             // 7:00 AM
    trainScheduler.scheduleTrain(104, "Kalyan Local", 450, stationNameToId["Kalyan"]);         // 7:30 AM
    
    // Peak hour trains
    trainScheduler.scheduleTrain(201, "Dadar Special", 480, stationNameToId["Dadar"]);         // 8:00 AM
    trainScheduler.scheduleTrain(202, "Andheri Fast", 510, stationNameToId["Andheri"]);        // 8:30 AM
    trainScheduler.scheduleTrain(203, "Harbour Express", 540, stationNameToId["Panvel"]);      // 9:00 AM
    
    // Assign some trains to platform queue
    platformManager.enqueue(101);
    platformManager.enqueue(102);
    platformManager.enqueue(103);
    
    cout << "\n✓ System initialization complete!\n";
    cout << "✓ Total Stations: " << allStations.size() << "\n";
    cout << "✓ Trains Scheduled: 7\n";
    cout << "✓ Network Status: OPERATIONAL\n\n";
}

// ======================================================================================
//                                   MENU DISPLAY
// ======================================================================================

/**
 * Function: displayMenu
 * Displays the main menu interface
 * 
 * Menu Options:
 * 1. Station Management (BST operations)
 * 2. Route Finding (Graph algorithms)
 * 3. Ticketing (Queue management)
 * 4. Train Scheduling (Heap operations)
 * 5. Platform Management (Circular queue)
 * 6. Analytics & Reports
 * 7. System Administration
 * 8. Exit
 */
void displayMenu() {
    cout << "\n╔════════════════════════════════════════════════════════╗\n";
    cout << "║     MUMBAI LOCAL TRANSPORTATION MANAGEMENT SYSTEM      ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n";
    cout << "┌────────────────────────────────────────────────────────┐\n";
    cout << "│  STATION & ROUTE MANAGEMENT                            │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  1. View All Stations (BST Traversal)                 │\n";
    cout << "│  2. Search Station (BST Search)                       │\n";
    cout << "│  3. Find Fastest Route (Dijkstra)                     │\n";
    cout << "│  4. Check Network Connectivity (BFS)                  │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  TICKETING & PASSENGER MANAGEMENT                      │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  5. Buy Ticket (Multi-Queue Processing)               │\n";
    cout << "│  6. View Ticketing Statistics                         │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  TRAIN SCHEDULING & PLATFORMS                          │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  7. View Train Schedule (Min Heap)                    │\n";
    cout << "│  8. Optimize Peak Hour Frequency                      │\n";
    cout << "│  9. Process Platform Queue                            │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  ANALYTICS & REPORTS                                   │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  10. Passenger Flow Analytics                         │\n";
    cout << "│  11. Station Congestion Report                        │\n";
    cout << "│  12. Peak Hour Statistics                             │\n";
    cout << "│  13. Comprehensive Dashboard                          │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  ADMINISTRATION & EMERGENCY                            │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  14. Report Track Failure (Block Track)              │\n";
    cout << "│  15. Display Network Statistics                       │\n";
    cout << "│  16. Simulate Passenger Load                          │\n";
    cout << "├────────────────────────────────────────────────────────┤\n";
    cout << "│  0. Exit System                                       │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    cout << "\nEnter your choice: ";
}

// ======================================================================================
//                                   MENU HANDLERS
// ======================================================================================

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
    
    int stationId = stationDirectory.getStationId(stationName);
    
    if (stationId == -1) {
        cout << "\n❌ Station not found: " << stationName << "\n";
        cout << "Please check the spelling and try again.\n";
    } else {
        const Station& station = allStations[stationId];
        cout << "\n✓ Station Found!\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        cout << "Name: " << station.name << "\n";
        cout << "ID: " << station.id << "\n";
        cout << "Line: " << station.getLineName() << "\n";
        cout << "Platforms: " << station.platforms << "\n";
        cout << "Current Load: " << station.passengerCount << " passengers\n";
        cout << "Interchange: " << (station.isInterchange ? "Yes" : "No") << "\n";
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    }
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
    
    int srcId = stationDirectory.getStationId(srcName);
    int destId = stationDirectory.getStationId(destName);
    
    if (srcId == -1 || destId == -1) {
        cout << "\n❌ Invalid station name(s). Please check spelling.\n";
        return;
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
    
    int stationId = stationDirectory.getStationId(stationName);
    
    if (stationId == -1) {
        cout << "\n❌ Station not found.\n";
        return;
    }
    
    mumbaiLocal->showConnectivity(stationId);
}

/**
 * Function: handleTicketing
 * Processes ticket purchase with multi-queue priority system
 */
void handleTicketing() {
    string name, destName;
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
    
    cout << "Enter destination station: ";
    cin.ignore();
    getline(cin, destName);
    
    int destId = stationDirectory.getStationId(destName);
    if (destId == -1) {
        cout << "\n❌ Invalid destination station.\n";
        return;
    }
    
    // Create passenger object (simplified - no full Passenger struct needed)
    cout << "\n✓ Passenger " << name << " added to ";
    if (type == LADIES) cout << "LADIES";
    else if (type == SENIOR) cout << "SENIOR CITIZEN";
    else cout << "GENERAL";
    cout << " queue.\n";
    
    // Simulate ticket generation
    int baseFare = 10;
    int distance = (rand() % 50) + 5;  // Random distance 5-55 km
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
    cout << "Destination: " << destName << "\n";
    cout << "Distance: " << distance << " km\n";
    cout << "Fare: Rs. " << fare << "\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    // Update ticketing stats (simplified)
    allStations[destId].passengerCount++;
}

/**
 * Function: handleTrackFailure
 * Reports and blocks a track between two stations
 */
void handleTrackFailure() {
    string station1, station2;
    cout << "\n┌────────────────────────────────────────────────────────┐\n";
    cout << "│              EMERGENCY: TRACK FAILURE                  │\n";
    cout << "└────────────────────────────────────────────────────────┘\n";
    cout << "Enter first station: ";
    cin.ignore();
    getline(cin, station1);
    cout << "Enter second station: ";
    getline(cin, station2);
    
    int id1 = stationDirectory.getStationId(station1);
    int id2 = stationDirectory.getStationId(station2);
    
    if (id1 == -1 || id2 == -1) {
        cout << "\n❌ Invalid station name(s).\n";
        return;
    }
    
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
    // Seed random number generator
    srand(time(0));
    
    // Initialize the entire system
    initializeSystem();
    
    int choice;
    bool running = true;
    
    // Main menu loop
    while (running) {
        displayMenu();
        cin >> choice;
        
        // Clear any input errors
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n❌ Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                // View all stations (BST traversal)
                stationDirectory.listStations();
                break;
                
            case 2:
                // Search for a specific station
                handleStationSearch();
                break;
                
            case 3:
                // Find fastest route
                handleRouteSearch();
                break;
                
            case 4:
                // Check network connectivity
                handleConnectivityCheck();
                break;
                
            case 5:
                // Buy ticket
                handleTicketing();
                break;
                
            case 6:
                // View ticketing statistics
                ticketMachine.showStats();
                break;
                
            case 7:
                // View train schedule
                trainScheduler.showUpcomingTrains();
                break;
                
            case 8:
                // Optimize for peak hours
                {
                    cout << "\n┌────────────────────────────────────────────────────────┐\n";
                    cout << "│           PEAK HOUR FREQUENCY OPTIMIZATION             │\n";
                    cout << "└────────────────────────────────────────────────────────┘\n";
                    trainScheduler.optimizeFrequency(true);
                }
                break;
                
            case 9:
                // Process platform queue
                handlePlatformQueue();
                break;
                
            case 10:
                // Passenger flow analytics
                displayPassengerFlowAnalytics();
                break;
                
            case 11:
                // Station congestion report
                displayCongestionReport();
                break;
                
            case 12:
                // Peak hour statistics
                displayPeakHourStatistics();
                break;
                
            case 13:
                // Comprehensive dashboard
                displayComprehensiveAnalytics(ticketMachine);
                break;
                
            case 14:
                // Report track failure
                handleTrackFailure();
                break;
                
            case 15:
                // Display network statistics
                mumbaiLocal->displayNetworkStats();
                break;
                
            case 16:
                // Simulate passenger load
                simulatePassengerLoad();
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
                cout << "\n❌ Invalid choice. Please select a valid option (0-16).\n";
                break;
        }
        
        // Pause before showing menu again
        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
    
    // Cleanup
    delete mumbaiLocal;
    
    return 0;
}
