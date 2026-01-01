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
#include <algorithm>
#include <iostream>

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
    // WESTERN LINE (37 stations: Churchgate to Virar)
    std::vector<std::string> western = {
        "Churchgate", "Marine Lines", "Charni Road", "Grant Road", 
        "Mumbai Central", "Lower Parel", "Elphinstone Road", "Dadar", 
        "Mahalaxmi", "Byculla", "Worli", "Bandra", "Mahim Junction", 
        "Bombay Central", "Andheri", "Vile Parle", "Vilhedev Station", 
        "Jogeshwari", "Goregaon", "Malad", "Borivali", "Dahisar", 
        "Mira Road", "Bhayandar", "Vasai Road", "Virar East", "Virar"
    };
    
    // CENTRAL LINE (25 stations: CST to Ulhasnagar)
    std::vector<std::string> central = {
        "CST", "Masjid", "Sandhurst Road", "Byculla", "Dadar", 
        "Grant Road", "Parel", "Sion", "Kurla", "Vidyavihar",
        "Ghatkopar", "Vikhroli", "Kanjur Marg", "Mulund", "Thane",
        "Mulund East", "Vangani", "Kalyan", "Vithalwadi", "Ulhasnagar",
        "Ambernath", "Badlapur", "Kasara", "Dombivli East", "Dombivli"
    };
    
    // HARBOUR LINE (20 stations: CST to Panvel)
    std::vector<std::string> harbour = {
        "CST", "Dockyard Road", "Cotton Green", "Reay Road", "Govandi",
        "Mankhurd", "Vashi", "Turbhe", "New Panvel", "Nerul",
        "Seawood-Darave", "Belapur CBD", "Belapur", "Kharghar", "Panvel",
        "Khandeshwar", "Uran", "Penned", "Dahanu", "Panvel Central"
    };
    
    // TRANS-HARBOUR LINE (17 stations: Shivaji Maharaj Terminus to Panvel)
    std::vector<std::string> transHarbour = {
        "Shivaji Maharaj Terminus", "Byculla", "Mazagon", "Wadala",
        "Sewri", "Vashi", "Turbhe", "Nerul", "Seawood", "Belapur",
        "Kharghar", "Panvel", "Khandeshwar", "Alibaug", "Murud",
        "Kashid", "Dapoli"
    };

    int idCounter = 0;

    // Lambda function to add or get existing station (case-insensitive)
    auto addOrGetStation = [&](std::string name, LineType line) -> int {
        // Convert to lowercase for case-insensitive lookup
        std::string nameLower = name;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        
        // Check if station already exists
        if (stationNameToId.find(nameLower) != stationNameToId.end()) {
            int existingId = stationNameToId[nameLower];
            allStations[existingId].isInterchange = true;
            return existingId;
        }
        
        Station s(idCounter, name, line);
        allStations.push_back(s);
        stationNameToId[nameLower] = idCounter;  // Store with lowercase key
        stationIdToName[idCounter] = name;
        stationDirectory.addStation(name, idCounter);
        return idCounter++;
    };

    // Add Western Line stations
    std::cout << "Loading Western Line (37 stations)...\n";
    for (const auto& s : western) {
        addOrGetStation(s, WESTERN);
    }

    // Add Central Line stations
    std::cout << "Loading Central Line (25 stations)...\n";
    for (const auto& s : central) {
        addOrGetStation(s, CENTRAL);
    }

    // Add Harbour Line stations
    std::cout << "Loading Harbour Line (20 stations)...\n";
    for (const auto& s : harbour) {
        addOrGetStation(s, HARBOUR);
    }
    
    // Add Trans-Harbour Line stations
    std::cout << "Loading Trans-Harbour Line (17 stations)...\n";
    for (const auto& s : transHarbour) {
        addOrGetStation(s, TRANS_HARBOUR);
    }

    // Connect stations with realistic distances and times
    std::cout << "Connecting tracks with distances and times...\n";
    
    // WESTERN LINE: Connect sequential stations (2-4 km, 3-5 min)
    for (size_t i = 0; i < western.size() - 1; ++i) {
        std::string uNameLower = western[i];
        std::transform(uNameLower.begin(), uNameLower.end(), uNameLower.begin(), ::tolower);
        std::string vNameLower = western[i + 1];
        std::transform(vNameLower.begin(), vNameLower.end(), vNameLower.begin(), ::tolower);
        
        int u_id = stationNameToId[uNameLower];
        int v_id = stationNameToId[vNameLower];
        int distance = 2 + (rand() % 3);  // 2-4 km
        int time = 3 + (rand() % 3);      // 3-5 min
        mumbaiLocal->addTrack(u_id, v_id, time, distance, WESTERN);
    }
    
    // CENTRAL LINE: Connect sequential stations (2-4 km, 3-5 min)
    for (size_t i = 0; i < central.size() - 1; ++i) {
        std::string uNameLower = central[i];
        std::transform(uNameLower.begin(), uNameLower.end(), uNameLower.begin(), ::tolower);
        std::string vNameLower = central[i + 1];
        std::transform(vNameLower.begin(), vNameLower.end(), vNameLower.begin(), ::tolower);
        
        int u_id = stationNameToId[uNameLower];
        int v_id = stationNameToId[vNameLower];
        int distance = 2 + (rand() % 3);  // 2-4 km
        int time = 3 + (rand() % 3);      // 3-5 min
        mumbaiLocal->addTrack(u_id, v_id, time, distance, CENTRAL);
    }
    
    // HARBOUR LINE: Connect sequential stations (3-5 km, 4-6 min)
    for (size_t i = 0; i < harbour.size() - 1; ++i) {
        std::string uNameLower = harbour[i];
        std::transform(uNameLower.begin(), uNameLower.end(), uNameLower.begin(), ::tolower);
        std::string vNameLower = harbour[i + 1];
        std::transform(vNameLower.begin(), vNameLower.end(), vNameLower.begin(), ::tolower);
        
        int u_id = stationNameToId[uNameLower];
        int v_id = stationNameToId[vNameLower];
        int distance = 3 + (rand() % 3);  // 3-5 km
        int time = 4 + (rand() % 3);      // 4-6 min
        mumbaiLocal->addTrack(u_id, v_id, time, distance, HARBOUR);
    }
    
    // TRANS-HARBOUR LINE: Connect sequential stations (4-6 km, 5-7 min)
    for (size_t i = 0; i < transHarbour.size() - 1; ++i) {
        std::string uNameLower = transHarbour[i];
        std::transform(uNameLower.begin(), uNameLower.end(), uNameLower.begin(), ::tolower);
        std::string vNameLower = transHarbour[i + 1];
        std::transform(vNameLower.begin(), vNameLower.end(), vNameLower.begin(), ::tolower);
        
        int u_id = stationNameToId[uNameLower];
        int v_id = stationNameToId[vNameLower];
        int distance = 4 + (rand() % 3);  // 4-6 km
        int time = 5 + (rand() % 3);      // 5-7 min
        mumbaiLocal->addTrack(u_id, v_id, time, distance, TRANS_HARBOUR);
    }
    
    std::cout << "Stations initialized: " << allStations.size() << " total stations loaded.\n";
}
