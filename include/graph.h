/**
 * ======================================================================================
 * HEADER: graph.h
 * DESCRIPTION: Graph representation and railway network routing system
 * ======================================================================================
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "station.h"
#include "queue_manager.h"

// ======================================================================================
//                                   EDGE STRUCTURE
// ======================================================================================

struct Edge {
    int to;
    int weight; // Time in minutes
    int distance; // Distance in km
    LineType line;
};

// ======================================================================================
//                                   RAILWAY NETWORK CLASS
// ======================================================================================

/**
 * Graph & Navigation System using Custom MyStack and MyQueue
 * 
 * Key Features:
 * - Dijkstra's Algorithm for shortest path (uses Custom MyStack)
 * - BFS for connectivity check (uses Custom MyQueue)
 * - Track management for emergency scenarios
 * - Bidirectional graph representation
 */
class RailwayNetwork {
    int V; // Number of vertices (stations)
    
public:
    RailwayNetwork(int v);
    
    // Track Management
    void addTrack(int u, int v, int w, int distance, LineType line);
    void blockTrack(int u, int v);
    
    // Path Finding Algorithms
    void findFastestRoute(int src, int dest);  // Uses Dijkstra + Custom MyStack
    
    // Network Analysis
    void showConnectivity(int startNode);      // Uses BFS + Custom MyQueue
    void displayNetworkStats();                // Display graph statistics
    
    // Distance Query
    int getDistance(int src, int dest);        // Get distance between two stations
};

#endif // GRAPH_H
