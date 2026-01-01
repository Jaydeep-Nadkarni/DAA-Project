/**
 * ======================================================================================
 * HEADER: station.h
 * DESCRIPTION: Station-related data structures, enums, and constants
 * ======================================================================================
 */

#ifndef STATION_H
#define STATION_H

#include <string>
#include <vector>
#include "queue_manager.h"

// Forward declaration
struct Train;

// ======================================================================================
//                                   CONSTANTS & ENUMS
// ======================================================================================

enum LineType { WESTERN, CENTRAL, HARBOUR, TRANS_HARBOUR };
enum CongestionLevel { LOW, MEDIUM, HIGH, SEVERE };
enum TrainStatus { ON_TIME, DELAYED, CANCELLED };
enum PassengerType { GENERAL, LADIES, SENIOR, DISABILITY };

const int INF = 1e9;
const int MAX_STATIONS = 100;

// ======================================================================================
//                                   STATION STRUCTURE
// ======================================================================================

struct Station {
    int id;
    std::string name;
    LineType line;
    int platforms;
    int passengerCount;
    bool isInterchange;
    std::vector<std::string> exitPoints;
    MyList<Train> trainsAtStation; // Using Custom List

    Station(int _id = 0, std::string _name = "", LineType _line = WESTERN, int _platforms = 2);
};

// ======================================================================================
//                                   HELPER FUNCTIONS
// ======================================================================================

// Forward declarations
class StationBST;
class RailwayNetwork;

std::string getLineName(LineType l);
void initializeStations(StationBST& stationDirectory, RailwayNetwork* mumbaiLocal);

#endif // STATION_H
