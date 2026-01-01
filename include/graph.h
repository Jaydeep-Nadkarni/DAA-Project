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
 */
class RailwayNetwork {
    int V; // Number of vertices (stations)
    
public:
    RailwayNetwork(int v);
    
    void addTrack(int u, int v, int w, LineType line);
    void findFastestRoute(int src, int dest);
    void showConnectivity(int startNode);
    void blockTrack(int u, int v);
};

#endif // GRAPH_H
