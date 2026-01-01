/**
 * ======================================================================================
 * HEADER: analytics.h
 * DESCRIPTION: Station search using BST and analytics helper functions
 * ======================================================================================
 */

#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <string>
#include <vector>
#include <utility>

// Forward declaration
class TicketSystem;

// ======================================================================================
//                                   STATION BST CLASS
// ======================================================================================

/**
 * Binary Search Tree for Station Search
 */
class StationBST {
    struct Node {
        std::string name;
        int id;
        Node* left;
        Node* right;
        Node(std::string n, int i);
    };

    Node* root;

    Node* insert(Node* node, std::string name, int id);
    Node* search(Node* node, std::string name);
    void inorder(Node* node);
    void collectMatching(Node* node, const std::string& prefix, std::vector<std::pair<std::string, int>>& results);

public:
    StationBST();
    
    void addStation(std::string name, int id);
    int getStationId(std::string name);
    void listStations();
    std::vector<std::pair<std::string, int>> listMatchingStations(std::string prefix);
};

// ======================================================================================
//                                   ANALYTICS FUNCTIONS
// ======================================================================================

/**
 * Displays comprehensive passenger flow analytics
 * Shows top busiest stations and line-wise distribution
 */
void displayPassengerFlowAnalytics();

/**
 * Displays congestion report with station categorization
 * Categorizes stations as LOW, MEDIUM, HIGH, or SEVERE congestion
 */
void displayCongestionReport();

/**
 * Displays peak-hour statistics and patterns
 * Analyzes current time, capacity utilization, and provides recommendations
 */
void displayPeakHourStatistics();

/**
 * Displays comprehensive analytics dashboard
 * Integrates all data: stations, ticketing, operations, system health
 * @param ticketSystem Reference to the ticketing system for revenue data
 */
void displayComprehensiveAnalytics(const TicketSystem& ticketSystem);

#endif // ANALYTICS_H
