/**
 * ======================================================================================
 * IMPLEMENTATION: station.cpp
 * DESCRIPTION: Implementation of Station struct, helper functions, and initialization
 * ======================================================================================
 */

#include "../include/station.h"
#include "../include/scheduling.h"
#include "../include/globals.h"
#include "../include/analytics.h"
#include "../include/graph.h"
#include <cstdlib>

// ======================================================================================
//                                   GLOBAL VARIABLE DEFINITIONS
// ======================================================================================

std::vector<Station> allStations;
std::unordered_map<std::string, int> stationNameToId;
std::unordered_map<int, std::string> stationIdToName;
std::vector<std::vector<Edge>> adj;

// ======================================================================================
//                                   STATION IMPLEMENTATION
// ======================================================================================

Station::Station(int _id, std::string _name, LineType _line, int _platforms) {
    id = _id;
    name = _name;
    line = _line;
    platforms = _platforms;
    passengerCount = 0;
    isInterchange = false;
}

// ======================================================================================
//                                   HELPER FUNCTIONS
// ======================================================================================

std::string getLineName(LineType l) {
    switch(l) {
        case WESTERN: return "Western Line";
        case CENTRAL: return "Central Line";
        case HARBOUR: return "Harbour Line";
        case TRANS_HARBOUR: return "Trans-Harbour Line";
        default: return "Unknown";
    }
}

// ======================================================================================
//                                   STATION INITIALIZATION
// ======================================================================================

void initializeStations(StationBST& stationDirectory, RailwayNetwork* mumbaiLocal) {
    // Define station lines
    std::vector<std::string> western = {
        "Churchgate", "Marine Lines", "Charni Road", "Grant Road", 
        "Mumbai Central", "Dadar", "Bandra", "Andheri", "Borivali", "Virar"
    };
    
    std::vector<std::string> central = {
        "CST", "Masjid", "Sandhurst Road", "Byculla", "Dadar", 
        "Kurla", "Ghatkopar", "Thane", "Kalyan", "Dombivli", "Ulhasnagar"
    };
    
    std::vector<std::string> harbour = {
        "CST", "Dockyard Road", "Govandi", "Vashi", "Nerul", "Panvel"
    };

    int idCounter = 0;

    // Lambda function to add or get existing station
    auto addOrGetStation = [&](std::string name, LineType line) -> int {
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

    // Add Western Line stations
    for (const auto& s : western) {
        addOrGetStation(s, WESTERN);
    }

    // Add Central Line stations
    for (const auto& s : central) {
        addOrGetStation(s, CENTRAL);
    }

    // Add Harbour Line stations
    for (const auto& s : harbour) {
        addOrGetStation(s, HARBOUR);
    }

    // Connect Western Line stations
    for (size_t i = 0; i < western.size() - 1; ++i) {
        int u = stationNameToId[western[i]];
        int v = stationNameToId[western[i+1]];
        mumbaiLocal->addTrack(u, v, 3 + (rand() % 3), WESTERN);
    }
    
    // Connect Central Line stations
    for (size_t i = 0; i < central.size() - 1; ++i) {
        int u = stationNameToId[central[i]];
        int v = stationNameToId[central[i+1]];
        mumbaiLocal->addTrack(u, v, 4 + (rand() % 3), CENTRAL);
    }

    // Connect Harbour Line stations
    for (size_t i = 0; i < harbour.size() - 1; ++i) {
        int u = stationNameToId[harbour[i]];
        int v = stationNameToId[harbour[i+1]];
        mumbaiLocal->addTrack(u, v, 5 + (rand() % 3), HARBOUR);
    }
}
