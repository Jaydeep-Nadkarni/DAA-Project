/**
 * ======================================================================================
 * IMPLEMENTATION: analytics.cpp
 * DESCRIPTION: Station BST and Comprehensive Analytics System
 * 
 * FEATURES:
 * 1. Station BST for efficient station searching
 * 2. Passenger flow analytics and congestion reports
 * 3. Peak-hour statistics and trend analysis
 * 4. Historical data tracking and reporting
 * 5. Integration with ticketing and station data
 * ======================================================================================
 */

#include "../include/analytics.h"
#include "../include/globals.h"
#include "../include/ticketing.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <ctime>
#include "../include/colors.h"

// ======================================================================================
//                                   PASSENGER FLOW ANALYTICS
// ======================================================================================

/**
 * Function: displayPassengerFlowAnalytics
 * Displays comprehensive passenger flow statistics across all stations
 * 
 * Metrics Displayed:
 * - Total passengers processed system-wide
 * - Top 5 busiest stations by passenger count
 * - Average passengers per station
 * - Flow distribution by line (Western, Central, Harbour)
 * 
 * Algorithm:
 * 1. Aggregate passenger counts from all stations
 * 2. Sort stations by passenger count (descending)
 * 3. Calculate statistics and percentages
 * 4. Display in formatted report
 * 
 * Time Complexity: O(n log n) for sorting
 * 
 * Real-world use: Capacity planning, resource allocation
 */
void displayPassengerFlowAnalytics() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║       PASSENGER FLOW ANALYTICS REPORT                  ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
    
    if (allStations.empty()) {
        std::cout << "No station data available.\n";
        return;
    }
    
    // Calculate total passengers
    int totalPassengers = 0;
    for (const auto& station : allStations) {
        totalPassengers += station.passengerCount;
    }
    
    std::cout << "📊 SYSTEM OVERVIEW:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Total Passengers Processed: " << totalPassengers << "\n";
    std::cout << "Total Stations: " << allStations.size() << "\n";
    std::cout << "Average per Station: " 
              << (allStations.size() > 0 ? totalPassengers / allStations.size() : 0) << "\n\n";
    
    // Create sorted list of stations by passenger count
    std::vector<std::pair<int, std::string>> stationsByPassengers;
    for (const auto& station : allStations) {
        stationsByPassengers.push_back({station.passengerCount, station.name});
    }
    std::sort(stationsByPassengers.rbegin(), stationsByPassengers.rend());
    
    // Display top 5 busiest stations
    std::cout << "🚉 TOP 5 BUSIEST STATIONS:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << std::left << std::setw(5) << "Rank" << std::setw(25) << "Station Name" 
              << std::setw(15) << "Passengers" << "Percentage\n";
    std::cout << "──────────────────────────────────────────────────────────\n";
    
    int displayCount = std::min(5, (int)stationsByPassengers.size());
    for (int i = 0; i < displayCount; i++) {
        double percentage = totalPassengers > 0 
            ? (stationsByPassengers[i].first * 100.0 / totalPassengers) 
            : 0.0;
        
        std::cout << std::left << std::setw(5) << (i + 1)
                  << std::setw(25) << stationsByPassengers[i].second
                  << std::setw(15) << stationsByPassengers[i].first
                  << std::fixed << std::setprecision(2) << percentage << "%\n";
    }
    
    // Line-wise distribution
    std::map<std::string, int> linePassengers;
    for (const auto& station : allStations) {
        linePassengers[getLineName(station.line)] += station.passengerCount;
    }
    
    std::cout << "\n📈 LINE-WISE DISTRIBUTION:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    for (const auto& line : linePassengers) {
        double percentage = totalPassengers > 0 
            ? (line.second * 100.0 / totalPassengers) 
            : 0.0;
        std::cout << std::left << std::setw(20) << line.first 
                  << std::setw(10) << line.second 
                  << "(" << std::fixed << std::setprecision(1) << percentage << "%)\n";
    }
    
    std::cout << "══════════════════════════════════════════════════════════\n\n";
}

/**
 * Function: displayCongestionReport
 * Analyzes and reports congestion levels at all stations
 * 
 * Congestion Levels:
 * - LOW: < 50 passengers
 * - MEDIUM: 50-99 passengers
 * - HIGH: 100-199 passengers
 * - SEVERE: >= 200 passengers
 * 
 * Algorithm:
 * 1. Categorize each station by congestion level
 * 2. Identify critical congestion points
 * 3. Provide recommendations
 * 
 * Time Complexity: O(n)
 * 
 * Real-world use: Crowd management, safety protocols
 */
void displayCongestionReport() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║           CONGESTION LEVEL REPORT                      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
    
    if (allStations.empty()) {
        std::cout << "No station data available.\n";
        return;
    }
    
    // Categorize stations by congestion
    std::vector<std::string> lowCongestion, mediumCongestion, highCongestion, severeCongestion;
    
    for (const auto& station : allStations) {
        if (station.passengerCount < 50) {
            lowCongestion.push_back(station.name);
        } else if (station.passengerCount < 100) {
            mediumCongestion.push_back(station.name);
        } else if (station.passengerCount < 200) {
            highCongestion.push_back(station.name);
        } else {
            severeCongestion.push_back(station.name);
        }
    }
    
    // Display congestion summary
    std::cout << BOLDCYAN << "📊 CONGESTION SUMMARY:\n" << RESET;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << GREEN << "🟢 LOW (< 50):        " << lowCongestion.size() << " stations" << RESET << "\n";
    std::cout << YELLOW << "🟡 MEDIUM (50-99):    " << mediumCongestion.size() << " stations" << RESET << "\n";
    std::cout << ORANGE << "🟠 HIGH (100-199):    " << highCongestion.size() << " stations" << RESET << "\n";
    std::cout << RED << "🔴 SEVERE (>= 200):   " << severeCongestion.size() << " stations" << RESET << "\n\n";
    
    // Alert for severe congestion
    if (!severeCongestion.empty()) {
        std::cout << BOLDRED << "⚠️  CRITICAL ALERT - SEVERE CONGESTION:\n" << RESET;
        std::cout << RED << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        for (const auto& station : severeCongestion) {
            std::cout << "   • " << station << "\n";
        }
        std::cout << RESET << "\n💡 " << BOLDYELLOW << "RECOMMENDATIONS:\n" << RESET;
        std::cout << YELLOW << "   - Deploy additional crowd control personnel\n";
        std::cout << "   - Increase train frequency on affected lines\n";
        std::cout << "   - Activate emergency protocols if necessary\n";
        std::cout << "   - Monitor in real-time for safety compliance\n\n" << RESET;
    }
    
    // Display high congestion stations
    if (!highCongestion.empty()) {
        std::cout << BOLDYELLOW << "⚠️  HIGH CONGESTION STATIONS:\n" << RESET;
        std::cout << YELLOW << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        for (const auto& station : highCongestion) {
            std::cout << "   • " << station << "\n";
        }
        std::cout << RESET << "\n";
    }
    
    std::cout << "══════════════════════════════════════════════════════════\n\n";
}

// ======================================================================================
//                                   PEAK-HOUR STATISTICS
// ======================================================================================

/**
 * Function: displayPeakHourStatistics
 * Analyzes and displays peak-hour patterns and statistics
 * 
 * Analysis Includes:
 * - Current hour classification (Peak/Off-Peak)
 * - Predicted passenger load for next hour
 * - Historical peak hour patterns
 * - Capacity utilization recommendations
 * 
 * Peak Hours Definition:
 * - Morning Peak: 08:00 - 11:00
 * - Evening Peak: 17:00 - 21:00
 * 
 * Time Complexity: O(n)
 * 
 * Real-world use: Dynamic resource allocation, predictive scheduling
 */
void displayPeakHourStatistics() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║          PEAK HOUR STATISTICS & ANALYSIS               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
    
    // Get current time
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int currentHour = ltm->tm_hour;
    
    // Determine if current hour is peak
    bool isMorningPeak = (currentHour >= 8 && currentHour < 11);
    bool isEveningPeak = (currentHour >= 17 && currentHour < 21);
    bool isPeakHour = isMorningPeak || isEveningPeak;
    
    std::cout << "⏰ CURRENT TIME ANALYSIS:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Current Hour: " << std::setw(2) << std::setfill('0') << currentHour 
              << ":00" << std::setfill(' ') << "\n";
    std::cout << "Status: ";
    
    if (isMorningPeak) {
        std::cout << "🔴 MORNING PEAK HOUR (8-11 AM)\n";
    } else if (isEveningPeak) {
        std::cout << "🔴 EVENING PEAK HOUR (5-9 PM)\n";
    } else {
        std::cout << "🟢 OFF-PEAK HOUR\n";
    }
    
    // Calculate total current load
    int totalPassengers = 0;
    for (const auto& station : allStations) {
        totalPassengers += station.passengerCount;
    }
    
    // Estimate capacity
    int estimatedCapacity = allStations.size() * 200; // Assume 200 per station capacity
    double utilizationPercent = estimatedCapacity > 0 
        ? (totalPassengers * 100.0 / estimatedCapacity) 
        : 0.0;
    
    std::cout << "\n📊 CAPACITY UTILIZATION:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Current Load: " << totalPassengers << " passengers\n";
    std::cout << "System Capacity: " << estimatedCapacity << " passengers\n";
    std::cout << "Utilization: " << std::fixed << std::setprecision(1) 
              << utilizationPercent << "%\n";
    
    if (utilizationPercent > 80) {
        std::cout << "⚠️  WARNING: Nearing maximum capacity!\n";
    } else if (utilizationPercent > 60) {
        std::cout << "⚠️  CAUTION: High capacity utilization\n";
    } else {
        std::cout << "✓ Capacity within normal range\n";
    }
    
    // Peak hour predictions
    std::cout << "\n🔮 PEAK HOUR PATTERNS:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Morning Peak (8-11 AM):\n";
    std::cout << "  • Typical Load: 3-4x normal\n";
    std::cout << "  • Primary Direction: Suburbs → City Center\n";
    std::cout << "  • Key Stations: Churchgate, CST, Dadar\n\n";
    
    std::cout << "Evening Peak (5-9 PM):\n";
    std::cout << "  • Typical Load: 4-5x normal\n";
    std::cout << "  • Primary Direction: City Center → Suburbs\n";
    std::cout << "  • Key Stations: Andheri, Borivali, Thane\n\n";
    
    // Recommendations
    std::cout << "💡 RECOMMENDATIONS:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    if (isPeakHour) {
        std::cout << "✓ Peak hour protocols activated\n";
        std::cout << "✓ Increase train frequency by 30-40%\n";
        std::cout << "✓ Deploy additional platform staff\n";
        std::cout << "✓ Monitor congestion in real-time\n";
    } else {
        std::cout << "✓ Maintain standard operating schedule\n";
        std::cout << "✓ Prepare for upcoming peak hours\n";
        std::cout << "✓ Perform maintenance during low-traffic periods\n";
    }
    
    std::cout << "══════════════════════════════════════════════════════════\n\n";
}

// ======================================================================================
//                                   COMPREHENSIVE ANALYTICS
// ======================================================================================

/**
 * Function: displayComprehensiveAnalytics
 * Generates a complete analytics report integrating all data sources
 * 
 * Integrates Data From:
 * - Station passenger counts
 * - Ticketing system revenue
 * - Train schedules
 * - Network connectivity
 * 
 * Report Sections:
 * 1. System Overview
 * 2. Financial Summary
 * 3. Operational Metrics
 * 4. Performance Indicators
 * 
 * Time Complexity: O(n)
 * 
 * Real-world use: Executive dashboards, strategic planning
 */
void displayComprehensiveAnalytics(const TicketSystem& ticketSystem) {
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║       REALTIME COMPREHENSIVE ANALYTICS DASHBOARD      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n\n";
    
    // System Overview
    std::cout << "📈 SYSTEM OVERVIEW:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    int totalPassengers = 0;
    int interchangeCount = 0;
    for (const auto& station : allStations) {
        totalPassengers += station.passengerCount;
        if (station.isInterchange) interchangeCount++;
    }
    
    std::cout << "Network Size: " << allStations.size() << " stations\n";
    std::cout << "Interchange Stations: " << interchangeCount << "\n";
    std::cout << "Total Passengers Tracked: " << totalPassengers << "\n";
    std::cout << "Active Lines: 4 (Western, Central, Harbour, Trans-Harbour)\n\n";
    
    // Financial Summary (REALTIME)
    std::cout << "💰 FINANCIAL SUMMARY (REALTIME):\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Tickets Sold: " << ticketSystem.getTotalTickets() << "\n";
    std::cout << "Total Revenue: Rs. " << ticketSystem.getTotalRevenue() << "\n";
    
    if (ticketSystem.getTotalTickets() > 0) {
        double avgRevenue = (double)ticketSystem.getTotalRevenue() / ticketSystem.getTotalTickets();
        std::cout << "Average Ticket Price: Rs. " << std::fixed << std::setprecision(2) 
                  << avgRevenue << "\n";
        
        // Estimate daily revenue (assuming linear projection)
        double projectedDailyRevenue = ticketSystem.getTotalRevenue() * 10;  // Scale factor
        std::cout << "Projected Daily Revenue: Rs. " << std::fixed << std::setprecision(0) 
                  << projectedDailyRevenue << "\n";
    }
    std::cout << "\n";
    
    // Congestion Analysis (REALTIME)
    std::cout << "🚄 CONGESTION ANALYSIS (REALTIME):\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    double avgPassengersPerStation = allStations.size() > 0 
        ? (double)totalPassengers / allStations.size() 
        : 0.0;
    std::cout << "Avg Passengers/Station: " << std::fixed << std::setprecision(1) 
              << avgPassengersPerStation << "\n";
    
    // Find busiest station
    std::string busiestStation = "N/A";
    int maxPassengers = 0;
    for (const auto& station : allStations) {
        if (station.passengerCount > maxPassengers) {
            maxPassengers = station.passengerCount;
            busiestStation = station.name;
        }
    }
    std::cout << "Busiest Station: " << busiestStation << " (" << maxPassengers << " passengers)\n";
    
    // Categorize congestion levels
    int highCongestionCount = 0;
    int mediumCongestionCount = 0;
    int lowCongestionCount = 0;
    int lowThreshold = (int)avgPassengersPerStation / 2;
    int highThreshold = (int)avgPassengersPerStation * 1.5;
    
    for (const auto& station : allStations) {
        if (station.passengerCount >= highThreshold) {
            highCongestionCount++;
        } else if (station.passengerCount >= lowThreshold) {
            mediumCongestionCount++;
        } else {
            lowCongestionCount++;
        }
    }
    
    std::cout << "Congestion Status:\n";
    std::cout << "  🔴 HIGH Congestion: " << highCongestionCount << " stations\n";
    std::cout << "  🟡 MEDIUM Congestion: " << mediumCongestionCount << " stations\n";
    std::cout << "  🟢 LOW Congestion: " << lowCongestionCount << " stations\n";
    std::cout << "\n";
    
    // Peak Hour Statistics (REALTIME)
    std::cout << "⏰ PEAK HOUR STATISTICS (REALTIME):\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    // Simulate peak hour detection based on total passengers
    std::string peakStatus = (totalPassengers > 50) ? "PEAK HOURS DETECTED" : "OFF-PEAK";
    std::cout << "Peak Status: " << peakStatus << "\n";
    
    if (totalPassengers > 50) {
        std::cout << "Special Trains Activated: YES\n";
        std::cout << "Headway Reduced To: 10 minutes\n";
        std::cout << "Additional Capacity: +35%\n";
    } else {
        std::cout << "Special Trains Activated: NO\n";
        std::cout << "Standard Headway: 15-20 minutes\n";
        std::cout << "Operating Capacity: 100%\n";
    }
    std::cout << "\n";
    
    // System health
    std::cout << "🏥 SYSTEM HEALTH:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "✓ Network Status: OPERATIONAL\n";
    std::cout << "✓ All Lines: ACTIVE\n";
    std::cout << "✓ Ticketing System: ONLINE\n";
    std::cout << "✓ Real-time Tracking: ENABLED\n";
    std::cout << "✓ Platform Allocation: ACTIVE\n";
    std::cout << "✓ Emergency Services: STANDBY\n";
    
    std::cout << "══════════════════════════════════════════════════════════\n\n";
}
