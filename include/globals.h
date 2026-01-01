/**
 * ======================================================================================
 * HEADER: globals.h
 * DESCRIPTION: Global variable declarations for the entire system
 * ======================================================================================
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <unordered_map>
#include <string>
#include "station.h"
#include "graph.h"

// ======================================================================================
//                                   GLOBAL VARIABLES
// ======================================================================================

// Station Management
extern std::vector<Station> allStations;
extern std::unordered_map<std::string, int> stationNameToId;
extern std::unordered_map<int, std::string> stationIdToName;

// Graph Adjacency List
extern std::vector<std::vector<Edge>> adj;

#endif // GLOBALS_H
