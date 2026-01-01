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
class StationBST;  // Forward declaration - defined in station.h

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
