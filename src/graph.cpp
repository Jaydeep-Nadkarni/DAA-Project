/**
 * ======================================================================================
 * IMPLEMENTATION: graph.cpp
 * DESCRIPTION: Implementation of RailwayNetwork class (Graph operations)
 * ======================================================================================
 */

#include "../include/graph.h"
#include "../include/globals.h"
#include "../include/queue_manager.h"
#include <iostream>
#include <queue>

// ======================================================================================
//                                   RAILWAY NETWORK IMPLEMENTATION
// ======================================================================================

RailwayNetwork::RailwayNetwork(int v) {
    V = v;
    adj.resize(V);
}

void RailwayNetwork::addTrack(int u, int v, int w, LineType line) {
    Edge e1 = {v, w, 2, line}; 
    Edge e2 = {u, w, 2, line};
    adj[u].push_back(e1);
    adj[v].push_back(e2);
}

void RailwayNetwork::findFastestRoute(int src, int dest) {
    // Using STL priority_queue for Dijkstra as it requires update/decrease key which is complex in custom heap
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;
    std::vector<int> dist(V, INF);
    std::vector<int> parent(V, -1);

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
        std::cout << "No route found between " << stationIdToName[src] << " and " 
                  << stationIdToName[dest] << std::endl;
        return;
    }

    // Reconstruct path using Custom Stack
    MyStack<int> path;
    int curr = dest;
    while (curr != -1) {
        path.push(curr);
        curr = parent[curr];
    }

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

void RailwayNetwork::showConnectivity(int startNode) {
    std::vector<bool> visited(V, false);
    MyQueue<int> q; // Using Custom Queue

    visited[startNode] = true;
    q.push(startNode);

    std::cout << "\n--- Network Connectivity Check (BFS) ---\n";
    std::cout << "Starting from " << stationIdToName[startNode] << ":\n";

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
    std::cout << "Total Reachable Stations: " << count << std::endl;
    std::cout << "----------------------------------------\n";
}

void RailwayNetwork::blockTrack(int u, int v) {
    for(auto& edge : adj[u]) {
        if(edge.to == v) edge.weight = INF;
    }
    for(auto& edge : adj[v]) {
        if(edge.to == u) edge.weight = INF;
    }
    std::cout << "[ALERT] Track between " << stationIdToName[u] << " and " 
              << stationIdToName[v] << " BLOCKED due to emergency.\n";
}
