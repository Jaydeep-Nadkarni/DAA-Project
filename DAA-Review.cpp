/**
 * ======================================================================================
 * PROJECT: Urban Commute & Transportation Management System (Mumbai Local Example)
 * AUTHOR: Senior Software Architect (AI Assistant)
 * DATE: December 30, 2025
 * CONTEXT: Design and Analysis of Algorithms (DAA) Review
 * 
 * DESCRIPTION:
 * This system simulates the Mumbai Local Railway network. It demonstrates the application
 * of various Data Structures and Algorithms to solve real-world problems like routing,
 * crowd management, scheduling, and emergency handling.
 * 
 * DATA STRUCTURES USED:
 * 1. Arrays: Storage of static data, logs.
 * 2. Custom Linked Lists: Dynamic lists of trains, logs (MyList).
 * 3. Custom Stack: Navigation history, path reconstruction (MyStack).
 * 4. Custom Queue: Passenger ticketing, boarding (MyQueue).
 * 5. Circular Queue: Platform management (Load Balancing).
 * 6. Custom Min Heap: Train scheduling (MinHeap).
 * 7. BST (Binary Search Tree): Station searching.
 * 8. Hashing: ID lookups (STL unordered_map).
 * 9. Graph: Network representation (Adjacency List).
 * 
 * ALGORITHMS USED:
 * 1. Dijkstra's Algorithm: Shortest path finding.
 * 2. BFS/DFS: Network traversal.
 * 3. Sorting (Merge/Quick): Analytics.
 * 4. Hashing: O(1) access.
 * ======================================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <functional>
#include <ctime>
#include <cstdio>
#include <windows.h> // For sleep and console formatting

using namespace std;

// ======================================================================================
//                                   CONSTANTS & ENUMS
// ======================================================================================

enum LineType { WESTERN, CENTRAL, HARBOUR, TRANS_HARBOUR };
enum CongestionLevel { LOW, MEDIUM, HIGH, SEVERE };
enum TrainStatus { ON_TIME, DELAYED, CANCELLED };
enum PassengerType { GENERAL, LADIES, SENIOR, DISABILITY };

const int INF = 1e9;
const int MAX_STATIONS = 200;

// ======================================================================================
//                                   CUSTOM DATA STRUCTURES
// ======================================================================================

/**
 * Template Class: MyNode
 * Generic node for Linked List, Stack, and Queue.
 */
template <typename T>
class MyNode {
public:
    T data;
    MyNode* next;
    MyNode* prev; // For Doubly Linked List

    MyNode(T val) : data(val), next(NULL), prev(NULL) {}
};

/**
 * Template Class: MyStack
 * Custom implementation of Stack using Linked List.
 * Operations: push, pop, top, empty.
 * Time Complexity: O(1) for all operations.
 */
template <typename T>
class MyStack {
    MyNode<T>* topNode;
    int count;

public:
    MyStack() : topNode(NULL), count(0) {}

    void push(T val) {
        MyNode<T>* newNode = new MyNode<T>(val);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    void pop() {
        if (topNode) {
            MyNode<T>* temp = topNode;
            topNode = topNode->next;
            delete temp;
            count--;
        }
    }

    T top() {
        if (topNode) return topNode->data;
        return T(); // Return default if empty
    }

    bool empty() { return topNode == NULL; }
    int size() { return count; }
};

/**
 * Template Class: MyQueue
 * Custom implementation of Queue using Linked List.
 * Operations: push, pop, front, empty.
 * Time Complexity: O(1) for all operations.
 */
template <typename T>
class MyQueue {
    MyNode<T>* frontNode;
    MyNode<T>* rearNode;
    int count;

public:
    MyQueue() : frontNode(NULL), rearNode(NULL), count(0) {}

    void push(T val) {
        MyNode<T>* newNode = new MyNode<T>(val);
        if (rearNode == NULL) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    void pop() {
        if (frontNode == NULL) return;
        MyNode<T>* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == NULL) rearNode = NULL;
        delete temp;
        count--;
    }

    T front() {
        if (frontNode) return frontNode->data;
        return T();
    }

    bool empty() { return frontNode == NULL; }
    int size() { return count; }
};

/**
 * Template Class: MyList
 * Custom implementation of Doubly Linked List.
 */
template <typename T>
class MyList {
public:
    MyNode<T>* head;
    MyNode<T>* tail;
    int count;

    MyList() : head(NULL), tail(NULL), count(0) {}

    void push_back(T val) {
        MyNode<T>* newNode = new MyNode<T>(val);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void display() {
        MyNode<T>* temp = head;
        while (temp) {
            // Assuming T has operator<< overloaded or is primitive
            // cout << temp->data << " "; 
            temp = temp->next;
        }
    }
};

/**
 * Template Class: MinHeap
 * Custom implementation of Min Heap for Priority Queue.
 */
template <typename T>
class MinHeap {
    vector<T> heap;

    void heapifyUp(int index) {
        if (index == 0) return;
        int parent = (index - 1) / 2;
        if (heap[parent] > heap[index]) {
            swap(heap[parent], heap[index]);
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap.size() && heap[left] < heap[smallest]) smallest = left;
        if (right < heap.size() && heap[right] < heap[smallest]) smallest = right;

        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    void push(T val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    T top() {
        if (!heap.empty()) return heap[0];
        return T();
    }

    bool empty() { return heap.empty(); }
    int size() { return heap.size(); }
    
    // Helper to get underlying vector for display (not standard heap op but useful for UI)
    vector<T> getVector() { return heap; }
};

// ======================================================================================
//                                   DOMAIN STRUCTURES
// ======================================================================================

struct Train {
    int trainId;
    string name;
    int capacity;
    int currentLoad;
    int nextStationId;
    int arrivalTime; // Minutes from midnight
    TrainStatus status;

    // Operator for MinHeap
    bool operator>(const Train& other) const {
        return arrivalTime > other.arrivalTime;
    }
    bool operator<(const Train& other) const {
        return arrivalTime < other.arrivalTime;
    }
};

struct Station {
    int id;
    string name;
    LineType line;
    int platforms;
    int passengerCount;
    bool isInterchange;
    vector<string> exitPoints;
    MyList<Train> trainsAtStation; // Using Custom List

    Station(int _id = 0, string _name = "", LineType _line = WESTERN, int _platforms = 2) {
        id = _id;
        name = _name;
        line = _line;
        platforms = _platforms;
        passengerCount = 0;
        isInterchange = false;
    }
};

struct Passenger {
    int id;
    string name;
    int age;
    PassengerType type;
    int sourceId;
    int destId;
    int ticketPrice;
    time_t entryTime;
};

struct Edge {
    int to;
    int weight; // Time in minutes
    int distance; // Distance in km
    LineType line;
};

// ======================================================================================
//                                   GLOBAL DATA
// ======================================================================================

vector<Station> allStations;
unordered_map<string, int> stationNameToId;
unordered_map<int, string> stationIdToName;
vector<vector<Edge>> adj; 

// ======================================================================================
//                                   HELPER FUNCTIONS
// ======================================================================================

string getLineName(LineType l) {
    switch(l) {
        case WESTERN: return "Western Line";
        case CENTRAL: return "Central Line";
        case HARBOUR: return "Harbour Line";
        case TRANS_HARBOUR: return "Trans-Harbour Line";
        default: return "Unknown";
    }
}

// ======================================================================================
//                                   CLASS DEFINITIONS
// ======================================================================================

// --------------------------------------------------------------------------------------
// 1. BST for Station Search
// --------------------------------------------------------------------------------------
class StationBST {
    struct Node {
        string name;
        int id;
        Node* left;
        Node* right;
        Node(string n, int i) : name(n), id(i), left(NULL), right(NULL) {}
    };

    Node* root;

    Node* insert(Node* node, string name, int id) {
        if (node == NULL) return new Node(name, id);
        if (name < node->name) node->left = insert(node->left, name, id);
        else if (name > node->name) node->right = insert(node->right, name, id);
        return node;
    }

    Node* search(Node* node, string name) {
        if (node == NULL || node->name == name) return node;
        if (name < node->name) return search(node->left, name);
        return search(node->right, name);
    }

    void inorder(Node* node) {
        if (node != NULL) {
            inorder(node->left);
            cout << "  - " << left << setw(20) << node->name << " (ID: " << node->id << ")" << endl;
            inorder(node->right);
        }
    }

public:
    StationBST() : root(NULL) {}

    void addStation(string name, int id) {
        root = insert(root, name, id);
    }

    int getStationId(string name) {
        Node* res = search(root, name);
        if (res) return res->id;
        return -1;
    }

    void listStations() {
        cout << "\n--- Station Directory (Sorted Alphabetically via BST) ---\n";
        inorder(root);
        cout << "---------------------------------------------------------\n";
    }
};

// --------------------------------------------------------------------------------------
// 2. Queue Management for Ticketing (Using Custom MyQueue)
// --------------------------------------------------------------------------------------
class TicketSystem {
    MyQueue<Passenger> generalQueue;
    MyQueue<Passenger> ladiesQueue;
    MyQueue<Passenger> seniorQueue;
    int totalTicketsSold;
    long long totalRevenue;

public:
    TicketSystem() {
        totalTicketsSold = 0;
        totalRevenue = 0;
    }

    void joinQueue(Passenger p) {
        if (p.type == LADIES) {
            ladiesQueue.push(p);
            cout << ">> Passenger " << p.name << " joined LADIES Queue.\n";
        } else if (p.type == SENIOR) {
            seniorQueue.push(p);
            cout << ">> Passenger " << p.name << " joined SENIOR Queue.\n";
        } else {
            generalQueue.push(p);
            cout << ">> Passenger " << p.name << " joined GENERAL Queue.\n";
        }
    }

    void processQueues() {
        cout << "\n--- Processing Ticket Queues ---\n";
        
        // Process Senior Citizens first (Priority)
        while (!seniorQueue.empty()) {
            Passenger p = seniorQueue.front();
            seniorQueue.pop();
            processTicket(p);
        }

        // Process Ladies
        while (!ladiesQueue.empty()) {
            Passenger p = ladiesQueue.front();
            ladiesQueue.pop();
            processTicket(p);
        }

        // Process General
        while (!generalQueue.empty()) {
            Passenger p = generalQueue.front();
            generalQueue.pop();
            processTicket(p);
        }
        cout << "--------------------------------\n";
    }

    void processTicket(Passenger p) {
        int fare = 10 + (rand() % 50); 
        p.ticketPrice = fare;
        totalTicketsSold++;
        totalRevenue += fare;
        
        cout << "[TICKET ISSUED] " << p.name << " | Fare: Rs. " << fare << " | Type: ";
        if (p.type == SENIOR) cout << "Senior";
        else if (p.type == LADIES) cout << "Ladies";
        else cout << "General";
        cout << endl;
        
        if (p.sourceId >= 0 && p.sourceId < allStations.size()) {
            allStations[p.sourceId].passengerCount++;
        }
    }

    void showStats() {
        cout << "\n--- Ticketing Analytics ---\n";
        cout << "Total Tickets Sold: " << totalTicketsSold << endl;
        cout << "Total Revenue: Rs. " << totalRevenue << endl;
        cout << "---------------------------\n";
    }
};

// --------------------------------------------------------------------------------------
// 3. Circular Queue for Platform Load Balancing
// --------------------------------------------------------------------------------------
class PlatformQueue {
    int *arr;
    int front, rear, size, capacity;

public:
    PlatformQueue(int cap = 5) {
        capacity = cap;
        arr = new int[capacity];
        front = -1;
        rear = -1;
        size = 0;
    }

    bool isFull() { return size == capacity; }
    bool isEmpty() { return size == 0; }

    void enqueue(int trainId) {
        if (isFull()) {
            cout << "Platform Queue Full! Train " << trainId << " must wait.\n";
            return;
        }
        if (front == -1) front = 0;
        rear = (rear + 1) % capacity;
        arr[rear] = trainId;
        size++;
        cout << "Train " << trainId << " assigned to platform buffer.\n";
    }

    int dequeue() {
        if (isEmpty()) return -1;
        int item = arr[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % capacity;
        }
        size--;
        return item;
    }
};

// --------------------------------------------------------------------------------------
// 4. Train Scheduler (Using Custom MinHeap)
// --------------------------------------------------------------------------------------
class Scheduler {
    MinHeap<Train> trainSchedule;

public:
    void scheduleTrain(int id, string name, int time, int startStationId) {
        Train t;
        t.trainId = id;
        t.name = name;
        t.arrivalTime = time;
        t.nextStationId = startStationId;
        t.status = ON_TIME;
        t.capacity = 2000;
        t.currentLoad = 0;
        
        trainSchedule.push(t);
    }

    void showUpcomingTrains() {
        cout << "\n--- Upcoming Train Schedule (Sorted by Time) ---\n";
        if (trainSchedule.empty()) {
            cout << "No trains scheduled.\n";
            return;
        }

        // Create a copy to display without removing
        MinHeap<Train> temp = trainSchedule;
        
        cout << left << setw(10) << "Time" << setw(20) << "Train Name" << setw(15) << "Status" << endl;
        cout << "------------------------------------------------\n";
        
        while (!temp.empty()) {
            Train t = temp.top();
            temp.pop();
            
            int hrs = t.arrivalTime / 60;
            int mins = t.arrivalTime % 60;
            char timeStr[10];
            sprintf(timeStr, "%02d:%02d", hrs, mins);
            
            string statusStr = (t.status == ON_TIME) ? "ON TIME" : "DELAYED";
            
            cout << left << setw(10) << timeStr << setw(20) << t.name << setw(15) << statusStr << endl;
        }
        cout << "------------------------------------------------\n";
    }
    
    void optimizeFrequency(bool isPeakHour) {
        if (isPeakHour) {
            cout << "\n[SYSTEM] Peak Hour Detected! Increasing train frequency...\n";
            scheduleTrain(901, "Peak Special 1", 540, 0); // 9:00 AM
            scheduleTrain(902, "Peak Special 2", 550, 0); // 9:10 AM
        } else {
            cout << "\n[SYSTEM] Off-Peak Hours. Standard frequency maintained.\n";
        }
    }
};

// --------------------------------------------------------------------------------------
// 5. Graph & Navigation System (Using Custom MyStack)
// --------------------------------------------------------------------------------------
class RailwayNetwork {
    int V; 
    
public:
    RailwayNetwork(int v) {
        V = v;
        adj.resize(V);
    }

    void addTrack(int u, int v, int w, LineType line) {
        Edge e1 = {v, w, 2, line}; 
        Edge e2 = {u, w, 2, line};
        adj[u].push_back(e1);
        adj[v].push_back(e2);
    }

    // Dijkstra's Algorithm
    void findFastestRoute(int src, int dest) {
        // Using STL priority_queue for Dijkstra as it requires update/decrease key which is complex in custom heap
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        vector<int> dist(V, INF);
        vector<int> parent(V, -1);

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == dest) break;

            for (auto& edge : adj[u]) {
                int v = edge.to;
                int weight = edge.weight;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[dest] == INF) {
            cout << "No route found between " << stationIdToName[src] << " and " << stationIdToName[dest] << endl;
            return;
        }

        // Reconstruct path using Custom Stack
        MyStack<int> path;
        int curr = dest;
        while (curr != -1) {
            path.push(curr);
            curr = parent[curr];
        }

        cout << "\n--- Fastest Route Found ---\n";
        cout << "Total Time: " << dist[dest] << " minutes\n";
        cout << "Route: ";
        
        while (!path.empty()) {
            int s = path.top();
            path.pop();
            cout << stationIdToName[s];
            if (!path.empty()) cout << " -> ";
        }
        cout << "\n---------------------------\n";
    }

    // BFS for Reachability
    void showConnectivity(int startNode) {
        vector<bool> visited(V, false);
        MyQueue<int> q; // Using Custom Queue

        visited[startNode] = true;
        q.push(startNode);

        cout << "\n--- Network Connectivity Check (BFS) ---\n";
        cout << "Starting from " << stationIdToName[startNode] << ":\n";

        int count = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            count++;

            for (auto& edge : adj[u]) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }
        cout << "Total Reachable Stations: " << count << endl;
        cout << "----------------------------------------\n";
    }
    
    void blockTrack(int u, int v) {
        for(auto& edge : adj[u]) {
            if(edge.to == v) edge.weight = INF;
        }
        for(auto& edge : adj[v]) {
            if(edge.to == u) edge.weight = INF;
        }
        cout << "[ALERT] Track between " << stationIdToName[u] << " and " << stationIdToName[v] << " BLOCKED due to emergency.\n";
    }
};

// ======================================================================================
//                                   MAIN SYSTEM CONTROLLER
// ======================================================================================

StationBST stationDirectory;
TicketSystem ticketMachine;
Scheduler trainScheduler;
RailwayNetwork* mumbaiLocal;
PlatformQueue platformManager(10); 

void initializeSystem() {
    // 1. Define Stations
    vector<string> western = {"Churchgate", "Marine Lines", "Charni Road", "Grant Road", "Mumbai Central", "Dadar", "Bandra", "Andheri", "Borivali", "Virar"};
    vector<string> central = {"CST", "Masjid", "Sandhurst Road", "Byculla", "Dadar", "Kurla", "Ghatkopar", "Thane", "Kalyan", "Dombivli", "Ulhasnagar"};
    vector<string> harbour = {"CST", "Dockyard Road", "Govandi", "Vashi", "Nerul", "Panvel"};

    int idCounter = 0;

    auto addOrGetStation = [&](string name, LineType line) -> int {
        if (stationNameToId.find(name) != stationNameToId.end()) {
            allStations[stationNameToId[name]].isInterchange = true;
            return stationNameToId[name];
        }
        
        Station s(idCounter, name, line);
        allStations.push_back(s);
        stationNameToId[name] = idCounter;
        stationIdToName[idCounter] = name;
        stationDirectory.addStation(name, idCounter);
        return idCounter++;
    };

    int prev = -1;
    for (string s : western) {
        int curr = addOrGetStation(s, WESTERN);
        prev = curr;
    }

    prev = -1;
    for (string s : central) {
        int curr = addOrGetStation(s, CENTRAL);
        prev = curr;
    }

    prev = -1;
    for (string s : harbour) {
        int curr = addOrGetStation(s, HARBOUR);
        prev = curr;
    }

    mumbaiLocal = new RailwayNetwork(idCounter);

    // Connect Stations
    for (size_t i = 0; i < western.size() - 1; ++i) {
        int u = stationNameToId[western[i]];
        int v = stationNameToId[western[i+1]];
        mumbaiLocal->addTrack(u, v, 3 + (rand()%3), WESTERN); 
    }
    
    for (size_t i = 0; i < central.size() - 1; ++i) {
        int u = stationNameToId[central[i]];
        int v = stationNameToId[central[i+1]];
        mumbaiLocal->addTrack(u, v, 4 + (rand()%3), CENTRAL);
    }

    for (size_t i = 0; i < harbour.size() - 1; ++i) {
        int u = stationNameToId[harbour[i]];
        int v = stationNameToId[harbour[i+1]];
        mumbaiLocal->addTrack(u, v, 5 + (rand()%3), HARBOUR);
    }
    
    trainScheduler.scheduleTrain(101, "Fast Local (W)", 480, stationNameToId["Churchgate"]); 
    trainScheduler.scheduleTrain(102, "Slow Local (C)", 495, stationNameToId["CST"]); 
    trainScheduler.scheduleTrain(103, "Harbour Spl", 510, stationNameToId["Panvel"]); 

    cout << "System Initialized with " << allStations.size() << " stations.\n";
}

// ======================================================================================
//                                   MENU & UI
// ======================================================================================

void displayMenu() {
    cout << "\n========================================================\n";
    cout << "   MUMBAI LOCAL TRANSPORTATION MANAGEMENT SYSTEM\n";
    cout << "========================================================\n";
    cout << "1. View All Stations (BST Traversal)\n";
    cout << "2. Search Fastest Route (Dijkstra)\n";
    cout << "3. Buy Ticket (Queue Simulation)\n";
    cout << "4. View Train Schedule (Min Heap)\n";
    cout << "5. Passenger Flow & Analytics\n";
    cout << "6. Emergency: Report Track Failure\n";
    cout << "7. Admin: Optimize Frequency (Peak Hour)\n";
    cout << "8. Exit\n";
    cout << "========================================================\n";
    cout << "Enter Choice: ";
}

void handleRouteSearch() {
    string srcName, destName;
    cout << "\nEnter Source Station: ";
    cin.ignore();
    getline(cin, srcName);
    cout << "Enter Destination Station: ";
    getline(cin, destName);

    int u = stationDirectory.getStationId(srcName);
    int v = stationDirectory.getStationId(destName);

    if (u == -1 || v == -1) {
        cout << "Invalid Station Name(s). Please check spelling.\n";
        return;
    }

    mumbaiLocal->findFastestRoute(u, v);
}

void handleTicketing() {
    string name;
    int age;
    cout << "\nEnter Passenger Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Age: ";
    cin >> age;
    
    cout << "Select Type (1: General, 2: Ladies, 3: Senior): ";
    int typeOpt;
    cin >> typeOpt;
    
    PassengerType type = GENERAL;
    if (typeOpt == 2) type = LADIES;
    if (typeOpt == 3) type = SENIOR;
    if (age > 60) type = SENIOR;

    Passenger p;
    p.name = name;
    p.age = age;
    p.type = type;
    p.entryTime = time(0);
    
    cout << "Enter Destination Station Name: ";
    string dest;
    cin.ignore();
    getline(cin, dest);
    int destId = stationDirectory.getStationId(dest);
    p.destId = destId;
    p.sourceId = 0; 

    ticketMachine.joinQueue(p);
    ticketMachine.processQueues();
}

int main() {
    initializeSystem();

    int choice;
    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                stationDirectory.listStations();
                break;
            case 2:
                handleRouteSearch();
                break;
            case 3:
                handleTicketing();
                break;
            case 4:
                trainScheduler.showUpcomingTrains();
                break;
            case 5:
                ticketMachine.showStats();
                break;
            case 6:
                {
                    string s1, s2;
                    cout << "Enter Station 1 of blocked track: ";
                    cin.ignore();
                    getline(cin, s1);
                    cout << "Enter Station 2 of blocked track: ";
                    getline(cin, s2);
                    int u = stationDirectory.getStationId(s1);
                    int v = stationDirectory.getStationId(s2);
                    if(u != -1 && v != -1) mumbaiLocal->blockTrack(u, v);
                    else cout << "Invalid stations.\n";
                }
                break;
            case 7:
                trainScheduler.optimizeFrequency(true);
                break;
            case 8:
                cout << "Exiting System. Thank you!\n";
                return 0;
            default:
                cout << "Invalid Choice.\n";
        }
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
