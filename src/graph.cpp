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
 *   distance - Distance in kilometers
 *   line - LineType (WESTERN, CENTRAL, HARBOUR, etc.)
 * Time Complexity: O(1)
 */
void RailwayNetwork::addTrack(int u, int v, int w, int distance, LineType line) {
    Edge e1 = {v, w, distance, line};
    Edge e2 = {u, w, distance, line};
    adj[u].push_back(e1);
    adj[v].push_back(e2);
}

/**
 * Function: findFastestRoute
 * Implements Dijkstra's Algorithm to find the shortest path between two stations
 * Uses STL priority_queue for efficient min-heap operations
 * Uses CUSTOM MyStack for path reconstruction (demonstrating custom data structure)
 * Tracks both time and distance, computes ticket cost based on distance
 * 
 * Parameters:
 *   src - Source station ID
 *   dest - Destination station ID
 * 
 * Algorithm Steps:
 *   1. Initialize distances to infinity, source distance to 0
 *   2. Use priority queue to process nodes in order of shortest distance
 *   3. Relax edges and update distances (using real edge distances)
 *   4. Reconstruct path using Custom MyStack (LIFO for reverse traversal)
 *   5. Calculate total distance_km and cost (formula: 10 + 2*distance_km)
 *   6. Display route with distance, time, and cost
 * 
 * Time Complexity: O((V + E) log V) where V = vertices, E = edges
 * Space Complexity: O(V)
 */
void RailwayNetwork::findFastestRoute(int src, int dest) {
    // Using STL priority_queue for Dijkstra as it requires update/decrease key which is complex in custom heap
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;
    std::vector<int> dist(V, INF);
    std::vector<int> distKm(V, 0);  // Track distance in km
    std::vector<int> parent(V, -1);

    dist[src] = 0;
    distKm[src] = 0;
    pq.push({0, src});

    // Dijkstra's main loop
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == dest) break;  // Early termination optimization

        // Relax all edges from current node
        for (auto& edge : adj[u]) {
            int v = edge.to;
            int weight = edge.weight;  // Time in minutes
            int edgeDistance = edge.distance;  // Real distance in km

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                distKm[v] = distKm[u] + edgeDistance;  // Accumulate distance
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

    // Calculate cost: 10 + 2 * distance_km
    int totalDistanceKm = distKm[dest];
    int ticketCost = 10 + (2 * totalDistanceKm);

    // Display results
    std::cout << "\n========== Route Details ==========";
    std::cout << "\nRoute: ";
    
    MyStack<int> pathCopy;  // Copy for displaying route
    curr = dest;
    while (curr != -1) {
        pathCopy.push(curr);
        curr = parent[curr];
    }
    
    while (!pathCopy.empty()) {
        int s = pathCopy.top();
        pathCopy.pop();
        std::cout << stationIdToName[s];
        if (!pathCopy.empty()) std::cout << " -> ";
    }
    
    std::cout << "\n\nDistance (km): " << totalDistanceKm;
    std::cout << "\nTime (min):    " << dist[dest];
    std::cout << "\nCost (Rs):     " << ticketCost;
    std::cout << "\n==================================\n";
}

/**
 * Function: showConnectivity
 * Implements BFS (Breadth-First Search) to check network connectivity
 * Uses CUSTOM MyQueue for level-order traversal (demonstrating custom data structure)
 * Lists all reachable stations with numbered display
 * 
 * Parameters:
 *   startNode - Starting station ID for connectivity check
 * 
 * Algorithm Steps:
 *   1. Initialize visited array
 *   2. Use Custom MyQueue for BFS traversal
 *   3. Process nodes level by level
 *   4. Collect and number all reachable stations
 *   5. Display formatted list with station names and IDs
 * 
 * Time Complexity: O(V + E) where V = vertices, E = edges
 * Space Complexity: O(V)
 * 
 * Purpose: Used to verify network integrity and display all reachable destinations
 */
void RailwayNetwork::showConnectivity(int startNode) {
    std::vector<bool> visited(V, false);
    std::vector<int> reachableStations;  // Store all reachable stations
    MyQueue<int> q;  // Using CUSTOM Queue for BFS

    visited[startNode] = true;
    q.push(startNode);

    std::cout << "\n========== Network Connectivity (BFS) ==========";
    std::cout << "\nStarting from: " << stationIdToName[startNode];
    std::cout << "\n\nReachable Stations:\n";

    int count = 0;
    
    // BFS traversal using custom queue
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        reachableStations.push_back(u);
        count++;

        // Explore all adjacent stations
        for (auto& edge : adj[u]) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                q.push(edge.to);
            }
        }
    }
    
    // Display numbered list of reachable stations
    for (int i = 0; i < (int)reachableStations.size(); ++i) {
        int stationId = reachableStations[i];
        std::cout << "  " << (i + 1) << ". " << stationIdToName[stationId] 
                  << " (ID: " << stationId << ")\n";
    }
    
    std::cout << "\nTotal Reachable: " << count << " stations";
    std::cout << "\n===============================================\n";
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

/**
 * Function: getDistance
 * Computes the shortest distance (in km) between two stations using Dijkstra's algorithm
 * 
 * Parameters:
 *   src - Source station ID
 *   dest - Destination station ID
 * 
 * Returns:
 *   Total distance in kilometers, or INF if no path exists
 * 
 * Algorithm:
 *   Same as findFastestRoute but returns distance instead of displaying route
 * 
 * Time Complexity: O((V + E) log V)
 * Use Case: Computing fare based on actual route distance
 */
int RailwayNetwork::getDistance(int src, int dest) {
    // Handle invalid station IDs
    if (src < 0 || src >= V || dest < 0 || dest >= V) {
        return INF;
    }
    
    // Use Dijkstra's algorithm to find minimum distance
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;
    std::vector<int> distKm(V, INF);
    
    distKm[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        if (d > distKm[u]) continue;  // Skip if already processed
        if (u == dest) break;          // Early termination if destination reached
        
        // Relax edges from u
        for (auto& edge : adj[u]) {
            int v = edge.to;
            int edgeDistance = edge.distance;
            
            if (distKm[u] + edgeDistance < distKm[v]) {
                distKm[v] = distKm[u] + edgeDistance;
                pq.push({distKm[v], v});
            }
        }
    }
    
    return distKm[dest];
}
