/**
 * ======================================================================================
 * IMPLEMENTATION: station.cpp
 * DESCRIPTION: Implementation of Station struct and helper functions
 * ======================================================================================
 */

#include "../include/station.h"
#include "../include/scheduling.h"

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
