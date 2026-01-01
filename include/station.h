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
#include <algorithm>
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

// ======================================================================================
//                               STATION BST CLASS
// ======================================================================================

class StationBST {
private:
    struct BSTNode {
        std::string name;        // Original case-preserved name
        std::string nameLower;   // Lowercase for comparisons
        int stationId;
        BSTNode* left;
        BSTNode* right;
        
        BSTNode(const std::string& _name, int _id)
            : name(_name), stationId(_id), left(nullptr), right(nullptr) {
            nameLower = _name;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        }
    };
    
    BSTNode* root;
    
    void insertHelper(BSTNode*& node, const std::string& name, int id);
    int searchHelper(BSTNode* node, const std::string& name) const;
    void listHelper(BSTNode* node) const;
    void matchHelper(BSTNode* node, const std::string& prefix, 
                    std::vector<std::pair<std::string, int>>& results, int& count) const;
    void deleteHelper(BSTNode*& node);
    
public:
    StationBST() : root(nullptr) {}
    ~StationBST() { deleteHelper(root); }
    
    // Add a station to the BST
    void addStation(const std::string& name, int stationId);
    
    // Get station ID by name (case-insensitive)
    int getStationId(const std::string& name) const;
    
    // List all stations in lexical order
    void listStations() const;
    
    // Get up to 10 stations matching a prefix (case-insensitive, lexical order)
    std::vector<std::pair<std::string, int>> listMatchingStations(const std::string& prefix) const;
};

// Forward declarations
class RailwayNetwork;

std::string getLineName(LineType l);
void initializeStations(StationBST& stationDirectory, RailwayNetwork* mumbaiLocal);

#endif // STATION_H
