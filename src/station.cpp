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
//                               STATION BST IMPLEMENTATION
// ======================================================================================

void StationBST::insertHelper(BSTNode*& node, const std::string& name, int id) {
    if (node == nullptr) {
        node = new BSTNode(name, id);
        return;
    }
    
    // Lowercase comparison for BST ordering
    std::string nameLower = name;
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
    
    if (nameLower < node->nameLower) {
        insertHelper(node->left, name, id);
    } else {
        insertHelper(node->right, name, id);
    }
}

int StationBST::searchHelper(BSTNode* node, const std::string& name) const {
    if (node == nullptr) {
        return -1;
    }
    
    std::string nameLower = name;
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
    
    if (nameLower == node->nameLower) {
        return node->stationId;
    } else if (nameLower < node->nameLower) {
        return searchHelper(node->left, name);
    } else {
        return searchHelper(node->right, name);
    }
}

void StationBST::listHelper(BSTNode* node) const {
    if (node == nullptr) return;
    
    listHelper(node->left);
    std::cout << "  " << node->name << " (ID: " << node->stationId << ")\n";
    listHelper(node->right);
}

void StationBST::matchHelper(BSTNode* node, const std::string& prefix,
                             std::vector<std::pair<std::string, int>>& results, 
                             int& count) const {
    if (node == nullptr || count >= 10) return;
    
    // In-order traversal for lexical order
    matchHelper(node->left, prefix, results, count);
    
    if (count < 10) {
        std::string nameLower = node->nameLower;
        std::string prefixLower = prefix;
        std::transform(prefixLower.begin(), prefixLower.end(), prefixLower.begin(), ::tolower);
        
        if (nameLower.substr(0, prefixLower.length()) == prefixLower) {
            results.push_back({node->name, node->stationId});
            count++;
        }
    }
    
    if (count < 10) {
        matchHelper(node->right, prefix, results, count);
    }
}

void StationBST::deleteHelper(BSTNode*& node) {
    if (node == nullptr) return;
    deleteHelper(node->left);
    deleteHelper(node->right);
    delete node;
    node = nullptr;
}

void StationBST::addStation(const std::string& name, int stationId) {
    insertHelper(root, name, stationId);
}

int StationBST::getStationId(const std::string& name) const {
    return searchHelper(root, name);
}

void StationBST::listStations() const {
    if (root == nullptr) {
        std::cout << "No stations in directory.\n";
        return;
    }
    
    std::cout << "\n┌────────────────────────────────────────────────────────┐\n";
    std::cout << "│           ALL STATIONS (LEXICAL ORDER)                 │\n";
    std::cout << "└────────────────────────────────────────────────────────┘\n";
    listHelper(root);
}

std::vector<std::pair<std::string, int>> StationBST::listMatchingStations(const std::string& prefix) const {
    std::vector<std::pair<std::string, int>> results;
    int count = 0;
    matchHelper(root, prefix, results, count);
    return results;
}

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
    
    // Add Trans-Harbour Line stations
    for (const auto& s : transHarbour) {
        addOrGetStation(s, TRANS_HARBOUR);
    }

    // Connect stations with realistic distances and times
    
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
}
