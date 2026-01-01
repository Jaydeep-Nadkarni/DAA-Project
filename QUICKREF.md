# Quick Reference Guide - Mumbai Local Transportation Management System

## Quick Start

### Compile & Run (One Command)
```bash
g++ src/*.cpp -I include -o commute -std=c++11 && ./commute
```

### Windows (PowerShell)
```powershell
g++ src/*.cpp -I include -o commute.exe -std=c++11; .\commute.exe
```

---

## Data Structures Cheat Sheet

| Module | Data Structure | File | Purpose |
|--------|----------------|------|---------|
| **Station Management** | Binary Search Tree | `station.h/cpp` | O(log n) station search |
| **Route Finding** | Graph (Adjacency List) | `graph.h/cpp` | Network representation |
| **Route Finding** | Custom Stack | `graph.cpp` | Path reconstruction |
| **Connectivity** | Custom Queue | `graph.cpp` | BFS traversal |
| **Ticketing** | 3x Custom Queue | `ticketing.h/cpp` | Priority processing |
| **Scheduling** | Min Heap | `scheduling.h/cpp` | Time-ordered trains |
| **Platform Mgmt** | Circular Queue | `queue_manager.h/cpp` | Load balancing |
| **Lookups** | Hash Maps | `globals.h` | O(1) station ID lookup |

---

## Algorithm Reference

| Algorithm | Module | Time Complexity | Use Case |
|-----------|--------|-----------------|----------|
| **Dijkstra's Shortest Path** | `graph.cpp::findFastestRoute()` | O((V+E) log V) | Fastest route |
| **Breadth-First Search** | `graph.cpp::showConnectivity()` | O(V + E) | Reachability |
| **BST Insert/Search** | `analytics.cpp::StationBST` | O(log n) avg | Station lookup |
| **Heap Insert/Extract** | `scheduling.cpp::Scheduler` | O(log n) | Train priority |
| **Queue Operations** | `ticketing.cpp::TicketSystem` | O(1) | Passenger processing |

---

## Common Tasks

### Find Fastest Route
```
Menu: 3 → Enter source → Enter destination
Example: Churchgate → Andheri
Algorithm: Dijkstra's Shortest Path
```

### Book Ticket
```
Menu: 5 → Enter name → Enter age → Select type → Enter destination
Queue Priority: Senior > Ladies > General
```

### View Train Schedule
```
Menu: 7
Data Structure: Min Heap (sorted by arrival time)
Output: HH:MM format, chronological order
```

### Check Station
```
Menu: 2 → Enter station name
Data Structure: Binary Search Tree
Complexity: O(log n)
```

### Emergency Track Block
```
Menu: 14 → Enter station 1 → Enter station 2
Effect: Sets edge weight to ∞ (infinity)
Routing: Automatically uses alternative paths
```

---

## Module Dependencies

```
main.cpp
├── globals.h (shared data)
├── station.h/cpp
│   └── Station struct, initializeStations(), getLineName()
├── graph.h/cpp
│   └── RailwayNetwork, Dijkstra, BFS
├── ticketing.h/cpp
│   └── TicketSystem, multi-queue processing
├── scheduling.h/cpp
│   └── Scheduler, MinHeap, time formatting
├── queue_manager.h/cpp
│   └── PlatformQueue, circular queue
└── analytics.h/cpp
    └── StationBST, analytics functions
```

---

## Station Network

### Western Line (10 stations)
```
Churchgate → Marine Lines → Charni Road → Grant Road → 
Mumbai Central → Dadar → Bandra → Andheri → Borivali → Virar
```

### Central Line (10 stations)
```
CST → Masjid → Sandhurst Road → Byculla → Dadar → 
Kurla → Ghatkopar → Thane → Kalyan → Dombivli
```

### Harbour Line (8 stations)
```
CST → Dockyard Road → Govandi → Mankhurd → 
Vashi → Nerul → Belapur → Panvel
```

### Interchange Stations
- **Dadar**: Western ↔ Central
- **CST**: Central ↔ Harbour
- **Kurla**: Central ↔ Harbour
- **Mumbai Central**: Western hub

---

## Key Features

### 1. Station Search (BST)
- **Complexity**: O(log n) search, O(log n) insert
- **Function**: `stationDirectory.getStationId(name)`
- **Returns**: Station ID or -1 if not found

### 2. Fastest Route (Dijkstra)
- **Complexity**: O((V+E) log V) with priority queue
- **Function**: `mumbaiLocal->findFastestRoute(src, dest)`
- **Output**: Total time + path reconstruction using MyStack

### 3. Multi-Queue Ticketing
- **Queues**: Senior (priority 1), Ladies (priority 2), General (priority 3)
- **Processing**: `ticketMachine.processQueues()`
- **Discounts**: 50% for seniors (age > 60)

### 4. Train Scheduling (Min Heap)
- **Complexity**: O(log n) insert, O(log n) extract-min
- **Function**: `trainScheduler.scheduleTrain(id, name, time, station)`
- **Display**: Sorted by arrival time (earliest first)

### 5. Platform Queue (Circular)
- **Capacity**: 10 trains (configurable)
- **Operations**: `enqueue(trainId)`, `dequeue()`
- **Advantage**: O(1) operations, no shifting required

---

## Compilation Options

### Standard Build
```bash
g++ src/*.cpp -I include -o commute -std=c++11
```

### Optimized Build
```bash
g++ src/*.cpp -I include -o commute -std=c++11 -O2 -Wall
```

### Debug Build
```bash
g++ src/*.cpp -I include -o commute -std=c++11 -g -DDEBUG
```

### Windows (MSVC)
```cmd
cl /EHsc /I include src\*.cpp /Fe:commute.exe
```

### Using Makefile (Linux/macOS)
```bash
make              # Build
make clean        # Clean
make run          # Build and run
make debug        # Debug build
```

### Using Batch File (Windows)
```cmd
build.bat         # Automated Windows build
```

---

## Performance Notes

### Typical Operation Times (for n=25 stations)
- Station search: **~4 comparisons** (BST depth)
- Route finding: **<1 second** (Dijkstra)
- Ticket processing: **O(1)** per passenger
- Train scheduling: **~5 comparisons** (heap depth)
- Connectivity check: **~30-40 operations** (BFS)

### Memory Usage
- **Stations**: ~25 × 64 bytes = ~1.6 KB
- **Graph**: ~40 edges × 32 bytes = ~1.3 KB
- **Queues**: Dynamic, grows with passengers
- **Heap**: Dynamic, grows with trains
- **Total**: <10 KB base memory footprint

---

## Troubleshooting

### Compilation Errors

**Error**: `undefined reference to initializeStations`
- **Fix**: Ensure all `.cpp` files are included: `src/*.cpp`

**Error**: `fatal error: globals.h: No such file or directory`
- **Fix**: Add include directory: `-I include`

**Error**: C++11 required
- **Fix**: Add flag: `-std=c++11`

### Runtime Issues

**Issue**: Station not found
- **Fix**: Check exact spelling (case-sensitive)
- **Tip**: Use menu option 1 to view all stations

**Issue**: No route found
- **Fix**: Verify both stations are on connected lines
- **Tip**: Use interchange stations (Dadar, CST)

**Issue**: Platform queue full
- **Fix**: Process queue (menu 9) or increase capacity
- **Location**: `main.cpp` → `PlatformQueue platformManager(10);`

---

## Quick Help

### View All Commands
- Menu option **0**: Exit
- Menu option **1-16**: Various features
- **Ctrl+C**: Force quit (not recommended)

### Sample Test Data
```
Source: Churchgate
Destination: Andheri
Expected Route: Churchgate → Marine Lines → Dadar → Bandra → Andheri
Expected Time: ~23 minutes
```

### Analytics Thresholds
- **Low Congestion**: < 100 passengers
- **Medium**: 100-300 passengers
- **High**: 300-500 passengers
- **Severe**: > 500 passengers

---

## Further Reading

- **CLRS**: Chapter 22 (Graphs), Chapter 24 (Shortest Paths)
- **Sedgewick**: Algorithms in C++, Part 5
- **GeeksforGeeks**: [Dijkstra](https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/)
- **VisuAlgo**: [Graph Visualization](https://visualgo.net/en/sssp)

---
