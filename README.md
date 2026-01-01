# Mumbai Local Transportation Management System

[![C++](https://img.shields.io/badge/C++-11/14-blue.svg)](https://isocpp.org/)

A comprehensive urban transportation management system simulating the Mumbai Local Railway network. This project demonstrates the practical application of various **Data Structures and Algorithms (DSA)** to solve real-world problems including routing, crowd management, train scheduling, and emergency handling.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Data Structures & Algorithms](#data-structures--algorithms)
- [Module Architecture](#module-architecture)
- [Installation & Compilation](#installation--compilation)
- [Usage Guide](#usage-guide)
- [System Requirements](#system-requirements)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

The Mumbai Local Railway system is one of the busiest suburban railway networks in the world, carrying over 7.5 million passengers daily. This simulation models key aspects of the system including:

- **25+ Railway Stations** across three major lines (Western, Central, Harbour)
- **Real-time Route Planning** using shortest path algorithms
- **Multi-Priority Ticketing System** with queue management
- **Dynamic Train Scheduling** with peak-hour optimization
- **Platform Load Balancing** using circular queues
- **Comprehensive Analytics** for operational insights

This project was developed as part of a **Design and Analysis of Algorithms (DAA)** coursework to demonstrate proficiency in implementing and applying fundamental data structures and algorithms.

---

## Features

### Station & Route Management
- **Binary Search Tree (BST)** based station directory for O(log n) search
- **Dijkstra's Algorithm** for finding fastest routes between stations
- **Breadth-First Search (BFS)** for network connectivity analysis
- Real-time track blocking for emergency scenarios

### Smart Ticketing System
- **Multi-Queue Priority Processing**:
  - Senior Citizens (Highest Priority)
  - Ladies Queue
  - General Queue
- Automatic fare calculation based on distance
- Senior citizen discounts (50% off)
- Revenue tracking and statistics

### Train Scheduling & Platform Management
- **Min Heap** based scheduling for O(log n) train insertions
- Chronological train display sorted by arrival time
- **Circular Queue** for platform allocation (load balancing)
- Peak-hour frequency optimization
- Train status monitoring (On-Time, Delayed, Cancelled)

### Advanced Analytics
- Passenger flow analysis with top 5 busiest stations
- Real-time congestion categorization (Low/Medium/High/Severe)
- Peak-hour pattern analysis (Morning: 8-11 AM, Evening: 5-9 PM)
- Capacity utilization monitoring
- Comprehensive system health dashboard

### Emergency & Administration
- Emergency track blockage system
- Network statistics and health monitoring
- Passenger load simulation
- Historical data tracking

---

## Project Structure

```
DAA/
│
├── include/                    # Header files (.h)
│   ├── globals.h              # Global variables (extern declarations)
│   ├── station.h              # Station struct and BST definitions
│   ├── graph.h                # RailwayNetwork graph class
│   ├── ticketing.h            # TicketSystem with multi-queue
│   ├── scheduling.h           # Scheduler with MinHeap
│   ├── queue_manager.h        # PlatformQueue (circular queue)
│   └── analytics.h            # Analytics and reporting functions
│
├── src/                        # Implementation files (.cpp)
│   ├── main.cpp               # Menu-driven interface & system initialization
│   ├── station.cpp            # Station management & global definitions
│   ├── graph.cpp              # Graph algorithms (Dijkstra, BFS)
│   ├── ticketing.cpp          # Ticketing system implementation
│   ├── scheduling.cpp         # Train scheduling with MinHeap
│   ├── queue_manager.cpp      # Circular queue for platforms
│   └── analytics.cpp          # Analytics & reporting logic
│
├── data/                       # Data files (optional)
│   ├── stations.csv           # Station metadata (if used)
│   └── routes.json            # Pre-configured routes (if used)
│
├── DAA-Review.cpp             # Original monolithic implementation (reference)
├── README.md                  # This file
└── commute.exe                # Compiled executable (after build)
```

### Folder Descriptions

#### `include/` - Header Files
Contains all class definitions, function prototypes, and template implementations. Headers use **include guards** to prevent multiple inclusion.

#### `src/` - Source Files
Contains the implementation of all non-template classes and functions. Each module is separated for maintainability and reusability.

#### `data/` - Data Files
Optional directory for storing station metadata, pre-configured routes, or historical data for analytics.

---

## Data Structures & Algorithms

### Data Structures Used

| Data Structure | Module | Purpose | Time Complexity |
|----------------|--------|---------|-----------------|
| **Binary Search Tree (BST)** | `station.cpp/h` | Station directory, alphabetical search | Search: O(log n), Insert: O(log n) |
| **Custom Stack (MyStack)** | `graph.cpp` | Path reconstruction in Dijkstra's | Push/Pop: O(1) |
| **Custom Queue (MyQueue)** | `ticketing.cpp`, `graph.cpp` | Passenger queues, BFS traversal | Enqueue/Dequeue: O(1) |
| **Circular Queue** | `queue_manager.cpp/h` | Platform load balancing | Enqueue/Dequeue: O(1) |
| **Min Heap (MinHeap)** | `scheduling.cpp/h` | Train scheduling by time | Insert: O(log n), ExtractMin: O(log n) |
| **Graph (Adjacency List)** | `graph.cpp/h` | Railway network representation | Add Edge: O(1), Traversal: O(V+E) |
| **Hash Map (unordered_map)** | `globals.h`, `station.cpp` | O(1) station ID lookups | Search/Insert: O(1) avg |
| **Linked List (MyList)** | Template in headers | Dynamic train lists at stations | Insert: O(1), Search: O(n) |
| **Array** | `queue_manager.cpp` | Circular queue backing array | Access: O(1) |

### Algorithms Implemented

| Algorithm | Module | Use Case | Time Complexity |
|-----------|--------|----------|-----------------|
| **Dijkstra's Algorithm** | `graph.cpp` | Shortest path between stations | O((V+E) log V) with priority queue |
| **Breadth-First Search (BFS)** | `graph.cpp` | Network connectivity checking | O(V + E) |
| **BST Insertion/Search** | `analytics.cpp` | Station lookup and traversal | O(log n) average |
| **Heap Operations** | `scheduling.cpp` | Min element extraction for scheduling | O(log n) |
| **Priority Queue Processing** | `ticketing.cpp` | Multi-priority ticket processing | O(1) per dequeue |
| **Circular Queue Operations** | `queue_manager.cpp` | Platform buffer management | O(1) |

---

## Module Architecture

### 1. **Station Management** (`station.h/cpp`)
- **Data Structures**: Binary Search Tree (BST), Hash Maps
- **Purpose**: Station directory, metadata management
- **Key Functions**:
  - `initializeStations()` - Loads all 25+ stations
  - `getLineName()` - Returns railway line name
  - BST operations for alphabetical search
- **Global Variables**: `allStations`, `stationNameToId`, `stationIdToName`

### 2. **Graph & Routing** (`graph.h/cpp`)
- **Data Structures**: Adjacency List (Graph), Custom Stack, Custom Queue
- **Algorithms**: Dijkstra's Shortest Path, BFS
- **Key Functions**:
  - `findFastestRoute(src, dest)` - Dijkstra's algorithm
  - `showConnectivity(start)` - BFS traversal
  - `blockTrack(u, v)` - Emergency track closure
  - `displayNetworkStats()` - Network analytics
- **Complexity**: Route finding in O((V+E) log V)

### 3. **Ticketing System** (`ticketing.h/cpp`)
- **Data Structures**: Three Custom Queues (MyQueue)
- **Purpose**: Priority-based passenger processing
- **Queue Types**:
  1. **Senior Queue** (Highest Priority) - 50% discount
  2. **Ladies Queue** (Medium Priority)
  3. **General Queue** (Standard Priority)
- **Key Functions**:
  - `joinQueue(passenger)` - Adds to appropriate queue
  - `processQueues()` - Processes in priority order
  - `showStats()` - Revenue and ticket analytics
- **Features**: Automatic fare calculation, discount application

### 4. **Train Scheduling** (`scheduling.h/cpp`)
- **Data Structures**: Min Heap (MinHeap)
- **Purpose**: Time-ordered train management
- **Key Functions**:
  - `scheduleTrain(id, name, time, station)` - O(log n) insertion
  - `showUpcomingTrains()` - Displays sorted schedule
  - `optimizeFrequency(isPeakHour)` - Adds peak trains
- **Time Format**: HH:MM (24-hour format)
- **Peak Hours**: Morning (8-11 AM), Evening (5-9 PM)

### 5. **Platform Management** (`queue_manager.h/cpp`)
- **Data Structures**: Circular Queue (Array-based)
- **Purpose**: Platform load balancing
- **Key Functions**:
  - `enqueue(trainId)` - Assigns train to platform
  - `dequeue()` - Releases platform
  - `isFull()` / `isEmpty()` - Status checks
- **Capacity**: Configurable (default: 10 trains)

### 6. **Analytics & Reporting** (`analytics.h/cpp`)
- **Data Structures**: BST (for station search), Vectors
- **Purpose**: Operational insights and reporting
- **Key Functions**:
  - `displayPassengerFlowAnalytics()` - Top 5 busiest stations
  - `displayCongestionReport()` - Categorizes congestion levels
  - `displayPeakHourStatistics()` - Time-based analysis
  - `displayComprehensiveAnalytics()` - Full system dashboard
- **Congestion Levels**: Low (<100), Medium (100-300), High (300-500), Severe (>500)

### 7. **Global Data** (`globals.h`)
- **Purpose**: Shared data structures across modules
- **Contents**:
  - `extern vector<Station> allStations` - All station objects
  - `extern unordered_map<string, int> stationNameToId` - Name→ID mapping
  - `extern unordered_map<int, string> stationIdToName` - ID→Name mapping
  - `extern vector<vector<Edge>> adj` - Adjacency list for graph

---

## 🔧 Installation & Compilation

### Prerequisites
- **C++ Compiler**: g++ (GCC 4.8+) or MSVC (Visual Studio 2015+)
- **C++ Standard**: C++11 or higher
- **Operating System**: Windows, Linux, or macOS

### Compilation Instructions

#### Option 1: Single Command (Recommended)
```bash
g++ src/*.cpp -I include -o commute -std=c++11
```

#### Option 2: With Optimization
```bash
g++ src/*.cpp -I include -o commute -std=c++11 -O2 -Wall
```

#### Option 3: Step-by-Step Compilation
```bash
# Compile individual modules
g++ -c src/main.cpp -I include -o obj/main.o -std=c++11
g++ -c src/station.cpp -I include -o obj/station.o -std=c++11
g++ -c src/graph.cpp -I include -o obj/graph.o -std=c++11
g++ -c src/ticketing.cpp -I include -o obj/ticketing.o -std=c++11
g++ -c src/scheduling.cpp -I include -o obj/scheduling.o -std=c++11
g++ -c src/queue_manager.cpp -I include -o obj/queue_manager.o -std=c++11
g++ -c src/analytics.cpp -I include -o obj/analytics.o -std=c++11

# Link all object files
g++ obj/*.o -o commute -std=c++11
```

#### Windows (MSVC)
```cmd
cl /EHsc /I include src\*.cpp /Fe:commute.exe
```

### Compiler Flags Explained
- `-I include` - Include header directory
- `-std=c++11` - Use C++11 standard
- `-O2` - Optimization level 2 (optional)
- `-Wall` - Enable all warnings (recommended)
- `-o commute` - Output executable name

### Troubleshooting Compilation

**Issue**: `undefined reference to` errors
- **Solution**: Ensure all `.cpp` files are included in compilation

**Issue**: Header not found
- **Solution**: Verify `-I include` flag is present

**Issue**: C++11 features not supported
- **Solution**: Add `-std=c++11` or `-std=c++14` flag

---

## Usage Guide

### Running the Application

#### Windows
```bash
.\commute.exe
```

#### Linux/macOS
```bash
./commute
```

### Menu Navigation

Upon launching, you'll see a comprehensive menu with 16 options organized into categories:

```
╔════════════════════════════════════════════════════════╗
║     MUMBAI LOCAL TRANSPORTATION MANAGEMENT SYSTEM      ║
╚════════════════════════════════════════════════════════╝
┌────────────────────────────────────────────────────────┐
│  STATION & ROUTE MANAGEMENT                            │
├────────────────────────────────────────────────────────┤
│  1. View All Stations (BST Traversal)                 │
│  2. Search Station (BST Search)                       │
│  3. Find Fastest Route (Dijkstra)                     │
│  4. Check Network Connectivity (BFS)                  │
├────────────────────────────────────────────────────────┤
│  TICKETING & PASSENGER MANAGEMENT                      │
├────────────────────────────────────────────────────────┤
│  5. Buy Ticket (Multi-Queue Processing)               │
│  6. View Ticketing Statistics                         │
├────────────────────────────────────────────────────────┤
│  TRAIN SCHEDULING & PLATFORMS                          │
├────────────────────────────────────────────────────────┤
│  7. View Train Schedule (Min Heap)                    │
│  8. Optimize Peak Hour Frequency                      │
│  9. Process Platform Queue                            │
├────────────────────────────────────────────────────────┤
│  ANALYTICS & REPORTS                                   │
├────────────────────────────────────────────────────────┤
│  10. Passenger Flow Analytics                         │
│  11. Station Congestion Report                        │
│  12. Peak Hour Statistics                             │
│  13. Comprehensive Dashboard                          │
├────────────────────────────────────────────────────────┤
│  ADMINISTRATION & EMERGENCY                            │
├────────────────────────────────────────────────────────┤
│  14. Report Track Failure (Block Track)              │
│  15. Display Network Statistics                       │
│  16. Simulate Passenger Load                          │
├────────────────────────────────────────────────────────┤
│  0. Exit System                                       │
└────────────────────────────────────────────────────────┘
```

### Example Usage Scenarios

#### Scenario 1: Finding Fastest Route
1. Select option **3** (Find Fastest Route)
2. Enter source station: `Churchgate`
3. Enter destination station: `Andheri`
4. System displays fastest route using Dijkstra's algorithm

**Output Example:**
```
--- Fastest Route Found ---
Total Time: 23 minutes
Route: Churchgate -> Marine Lines -> Dadar -> Bandra -> Andheri
---------------------------
```

#### Scenario 2: Booking a Ticket
1. Select option **5** (Buy Ticket)
2. Enter passenger details:
   - Name: `Rajesh Kumar`
   - Age: `35`
   - Type: `1` (General)
   - Destination: `CST`
3. System assigns to appropriate queue and generates ticket

**Output Example:**
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                   TICKET DETAILS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Passenger: Rajesh Kumar (Age: 35)
Destination: CST
Distance: 18 km
Fare: Rs. 46
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

#### Scenario 3: Viewing Analytics
1. Select option **13** (Comprehensive Dashboard)
2. View integrated analytics:
   - System overview (stations, passengers)
   - Financial summary (revenue, tickets)
   - Operational metrics
   - System health status

#### Scenario 4: Emergency Response
1. Select option **14** (Report Track Failure)
2. Enter stations: `Dadar` and `Bandra`
3. System blocks track and reroutes trains

---


Pre-configured popular routes:

```json
{
  "routes": [
    {
      "name": "Churchgate to CST Express",
      "stations": ["Churchgate", "Marine Lines", "Dadar", "Kurla", "CST"],
      "distance_km": 25,
      "estimated_time_min": 35
    },
    {
      "name": "Andheri to Panvel Harbour",
      "stations": ["Andheri", "Dadar", "CST", "Vashi", "Nerul", "Panvel"],
      "distance_km": 48,
      "estimated_time_min": 65
    },
    {
      "name": "Virar to Churchgate Fast",
      "stations": ["Virar", "Borivali", "Andheri", "Bandra", "Dadar", "Mumbai Central", "Churchgate"],
      "distance_km": 58,
      "estimated_time_min": 70
    }
  ]
}
```

**Note**: Current implementation uses hardcoded station initialization. These data files are provided for potential future enhancements.

---

## 🖥️ System Requirements

### Minimum Requirements
- **CPU**: 1 GHz or faster processor
- **RAM**: 512 MB
- **Storage**: 10 MB available space
- **OS**: Windows 7+, Linux (kernel 3.x+), macOS 10.10+

### Recommended Requirements
- **CPU**: 2 GHz dual-core processor
- **RAM**: 1 GB
- **Storage**: 50 MB (for logs and data files)
- **Terminal**: Support for UTF-8 and box-drawing characters

---

## Performance Characteristics

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Station Search (BST) | O(log n) | O(n) |
| Route Finding (Dijkstra) | O((V+E) log V) | O(V) |
| Connectivity Check (BFS) | O(V + E) | O(V) |
| Train Scheduling (Heap) | O(log n) insert | O(n) |
| Ticket Processing | O(1) per passenger | O(k) - k queues |
| Platform Queue | O(1) enqueue/dequeue | O(capacity) |

**Where**:
- V = Number of vertices (stations) ≈ 25
- E = Number of edges (tracks) ≈ 30-40
- n = Number of trains or passengers

---

## Testing Scenarios

### Test Case 1: Route Finding
- **Input**: Churchgate → Virar
- **Expected**: Shortest path via Western Line stations
- **Validation**: Total time should be optimal

### Test Case 2: Multi-Queue Priority
- **Input**: Add 3 passengers (General, Ladies, Senior)
- **Expected**: Processing order: Senior → Ladies → General
- **Validation**: Queue priority respected

### Test Case 3: Peak Hour Scheduling
- **Input**: Activate peak hour optimization at 9:00 AM
- **Expected**: Additional trains scheduled
- **Validation**: Train frequency increased

### Test Case 4: Emergency Track Blocking
- **Input**: Block Dadar ↔ Bandra track
- **Expected**: Alternative routes used, infinite weight set
- **Validation**: Blocked track not used in routing

---

## Future Enhancements

- [ ] **Database Integration**: PostgreSQL/MySQL for persistent storage
- [ ] **GUI Interface**: Qt or web-based dashboard
- [ ] **Real-time Tracking**: GPS integration for live train locations
- [ ] **Mobile App**: React Native companion app
- [ ] **Machine Learning**: Predictive analytics for passenger flow
- [ ] **API Endpoints**: RESTful API for third-party integration
- [ ] **Multi-language Support**: Hindi, Marathi translations
- [ ] **Payment Gateway**: UPI/Card payment integration
- [ ] **Seat Reservation**: Coach and seat allocation
- [ ] **Notification System**: SMS/Email alerts for train delays

---

## Authors

- **Project Lead**: Jaydeep Nadkarni
- **Team**: kartik Pujari, Amruta K. , Praveen B.
- **Mentor**: Prof. Vaishali Parab
- **Course**: Design and Analysis of Algorithms (DAA)
- **Institution**: KLE Technological University
- **Year**: 2025-26

---
