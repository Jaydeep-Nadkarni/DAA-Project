/**
 * ======================================================================================
 * IMPLEMENTATION: graph.cpp
 * DESCRIPTION: Implementation of RailwayNetwork class (Graph operations)
 * 
 * ALGORITHMS IMPLEMENTED:
 * 1. Dijkstra's Algorithm - Shortest path finding (uses Custom MyStack)
 * 2. BFS (Breadth-First Search) - Network connectivity check (uses Custom MyQueue)
 * 3. Graph Operations - Add/Block tracks for emergency scenarios
 * ======================================================================================
 */

#include "../include/graph.h"
#include "../include/globals.h"
#include "../include/queue_manager.h"
#include <iostream>
#include <queue>
#include <vector>

// ======================================================================================
//                                   RAILWAY NETWORK IMPLEMENTATION
// ======================================================================================

/**
 * Constructor: RailwayNetwork
 * Initializes the railway network graph with V vertices (stations)
 * Resizes the global adjacency list to accommodate all stations
 */
RailwayNetwork::RailwayNetwork(int v) {
    V = v;
    adj.resize(V);
}

/**
 * Function: addTrack
 * Adds a bidirectional track (edge) between two stations
 * Parameters:
 *   u, v - Station IDs to connect
 *   w - Weight (travel time in minutes)
 *   line - LineType (WESTERN, CENTRAL, HARBOUR, etc.)
 * Time Complexity: O(1)
 */
void RailwayNetwork::addTrack(int u, int v, int w, LineType line) {
    Edge e1 = {v, w, 2, line};  // Distance defaulted to 2 km
    Edge e2 = {u, w, 2, line};
    adj[u].push_back(e1);
    adj[v].push_back(e2);
}

/**
 * Function: findFastestRoute
 * Implements Dijkstra's Algorithm to find the shortest path between two stations
 * Uses STL priority_queue for efficient min-heap operations
 * Uses CUSTOM MyStack for path reconstruction (demonstrating custom data structure)
 * 
 * Parameters:
 *   src - Source station ID
 *   dest - Destination station ID
 * 
 * Algorithm Steps:
 *   1. Initialize distances to infinity, source distance to 0
 *   2. Use priority queue to process nodes in order of shortest distance
 *   3. Relax edges and update distances
 *   4. Reconstruct path using Custom MyStack (LIFO for reverse traversal)
 *   5. Display route and total time
 * 
 * Time Complexity: O((V + E) log V) where V = vertices, E = edges
 * Space Complexity: O(V)
 */
void RailwayNetwork::findFastestRoute(int src, int dest) {
    // Using STL priority_queue for Dijkstra as it requires update/decrease key which is complex in custom heap
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;
    std::vector<int> dist(V, INF);
    std::vector<int> parent(V, -1);

    dist[src] = 0;
    pq.push({0, src});

    // Dijkstra's main loop
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == dest) break;  // Early termination optimization

        // Relax all edges from current node
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

    // Check if destination is unreachable
    if (dist[dest] == INF) {
        std::cout << "No route found between " << stationIdToName[src] << " and " 
                  << stationIdToName[dest] << std::endl;
        return;
    }

    // Path reconstruction using CUSTOM MyStack
    // Stack is used because we need to reverse the path (dest -> src becomes src -> dest)
    MyStack<int> path;
    int curr = dest;
    while (curr != -1) {
        path.push(curr);
        curr = parent[curr];
    }

    // Display results
    std::cout << "\n--- Fastest Route Found ---\n";
    std::cout << "Total Time: " << dist[dest] << " minutes\n";
    std::cout << "Route: ";
    
    while (!path.empty()) {
        int s = path.top();
        path.pop();
        std::cout << stationIdToName[s];
        if (!path.empty()) std::cout << " -> ";
    }
    std::cout << "\n---------------------------\n";
}

/**
 * Function: showConnectivity
 * Implements BFS (Breadth-First Search) to check network connectivity
 * Uses CUSTOM MyQueue for level-order traversal (demonstrating custom data structure)
 * 
 * Parameters:
 *   startNode - Starting station ID for connectivity check
 * 
 * Algorithm Steps:
 *   1. Initialize visited array
 *   2. Use Custom MyQueue for BFS traversal
 *   3. Process nodes level by level
 *   4. Count and display total reachable stations
 * 
 * Time Complexity: O(V + E) where V = vertices, E = edges
 * Space Complexity: O(V)
 * 
 * Purpose: Used to verify network integrity and detect disconnected components
 */
void RailwayNetwork::showConnectivity(int startNode) {
    std::vector<bool> visited(V, false);
    MyQueue<int> q;  // Using CUSTOM Queue for BFS

    visited[startNode] = true;
    q.push(startNode);

    std::cout << "\n--- Network Connectivity Check (BFS) ---\n";
    std::cout << "Starting from " << stationIdToName[startNode] << ":\n";

    int count = 0;
    
    // BFS traversal using custom queue
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        count++;

        // Explore all adjacent stations
        for (auto& edge : adj[u]) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                q.push(edge.to);
            }
        }
    }
    
    std::cout << "Total Reachable Stations: " << count << std::endl;
    std::cout << "----------------------------------------\n";
}

/**
 * Function: blockTrack
 * Simulates emergency track blockage by setting edge weights to infinity
 * Used for emergency scenarios like track maintenance or accidents
 * 
 * Parameters:
 *   u, v - Station IDs representing the track to be blocked
 * 
 * Implementation:
 *   - Sets weight to INF in both directions (bidirectional edge)
 *   - Dijkstra's algorithm will naturally avoid these edges
 *   - Allows testing of alternate route finding
 * 
 * Time Complexity: O(E) where E = edges adjacent to u and v
 * 
 * Real-world use: Track failures, maintenance, accidents, signal problems
 */
void RailwayNetwork::blockTrack(int u, int v) {
    // Block track from u to v
    for(auto& edge : adj[u]) {
        if(edge.to == v) edge.weight = INF;
    }
    
    // Block track from v to u (bidirectional)
    for(auto& edge : adj[v]) {
        if(edge.to == u) edge.weight = INF;
    }
    
    std::cout << "[ALERT] Track between " << stationIdToName[u] << " and " 
              << stationIdToName[v] << " BLOCKED due to emergency.\n";
}

/**
 * Function: displayNetworkStats
 * Displays comprehensive statistics about the railway network
 * 
 * Statistics Shown:
 *   - Total number of stations (vertices)
 *   - Total number of tracks (edges)
 *   - Average connections per station
 *   - Most connected station (hub identification)
 * 
 * Time Complexity: O(V + E)
 * 
 * Purpose: Network analysis and capacity planning
 */
void RailwayNetwork::displayNetworkStats() {
    int totalEdges = 0;
    int maxConnections = 0;
    int maxConnectedStation = 0;
    
    std::cout << "\n========== Railway Network Statistics ==========\n";
    std::cout << "Total Stations: " << V << std::endl;
    
    // Count edges and find most connected station
    for (int i = 0; i < V; i++) {
        int connections = adj[i].size();
        totalEdges += connections;
        
        if (connections > maxConnections) {
            maxConnections = connections;
            maxConnectedStation = i;
        }
    }
    
    totalEdges /= 2;  // Each edge counted twice (bidirectional)
    
    std::cout << "Total Tracks: " << totalEdges << std::endl;
    std::cout << "Average Connections per Station: " 
              << (V > 0 ? (double)totalEdges * 2 / V : 0) << std::endl;
    std::cout << "Most Connected Station (Hub): " 
              << stationIdToName[maxConnectedStation] 
              << " (" << maxConnections << " connections)" << std::endl;
    std::cout << "================================================\n";
}
