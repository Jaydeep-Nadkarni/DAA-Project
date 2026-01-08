/**
 * ======================================================================================
 * IMPLEMENTATION: csv_manager.cpp
 * DESCRIPTION: Handles CSV file operations for data persistence
 * ======================================================================================
 */

#include "../include/csv_manager.h"
#include "../include/globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

const std::string CSVManager::STATION_FILE = "data/stations.csv";
const std::string CSVManager::TICKET_FILE = "data/tickets.csv";
const std::string CSVManager::ROUTE_FILE = "data/routes.csv";
const std::string CSVManager::USER_FILE = "data/users.csv";

void CSVManager::initializeDataDirectory() {
    if (!fs::exists("data")) {
        fs::create_directory("data");
    }
}

void CSVManager::saveStations(const std::vector<Station>& stations) {
    std::ofstream file(STATION_FILE);
    if (!file.is_open()) return;

    file << "id,name,line,platforms,passengerCount,isInterchange\n";
    for (const auto& s : stations) {
        file << s.id << "," << s.name << "," << (int)s.line << "," << s.platforms << "," 
             << s.passengerCount << "," << (s.isInterchange ? 1 : 0) << "\n";
    }
    file.close();
}

bool CSVManager::loadStations(std::vector<Station>& stations) {
    std::ifstream file(STATION_FILE);
    if (!file.is_open()) return false;

    stations.clear();
    std::string line, word;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int id, lineType, platforms, passengerCount, interchange;
        std::string name;

        std::getline(ss, word, ','); id = std::stoi(word);
        std::getline(ss, name, ',');
        std::getline(ss, word, ','); lineType = std::stoi(word);
        std::getline(ss, word, ','); platforms = std::stoi(word);
        std::getline(ss, word, ','); passengerCount = std::stoi(word);
        std::getline(ss, word, ','); interchange = std::stoi(word);

        Station s(id, name, (LineType)lineType, platforms);
        s.passengerCount = passengerCount;
        s.isInterchange = (interchange == 1);
        stations.push_back(s);
    }
    file.close();
    return true;
}

void CSVManager::saveTickets(const std::vector<Passenger>& tickets) {
    std::ofstream file(TICKET_FILE);
    if (!file.is_open()) return;

    file << "id,name,age,type,sourceId,destId,ticketPrice,entryTime\n";
    for (const auto& t : tickets) {
        file << t.id << "," << t.name << "," << t.age << "," << (int)t.type << "," 
             << t.sourceId << "," << t.destId << "," << t.ticketPrice << "," << t.entryTime << "\n";
    }
    file.close();
}

void CSVManager::appendTicket(const Passenger& t) {
    bool fileExists = fs::exists(TICKET_FILE);
    std::ofstream file(TICKET_FILE, std::ios::app);
    if (!file.is_open()) return;

    if (!fileExists) {
        file << "id,name,age,type,sourceId,destId,ticketPrice,entryTime\n";
    }
    file << t.id << "," << t.name << "," << t.age << "," << (int)t.type << "," 
         << t.sourceId << "," << t.destId << "," << t.ticketPrice << "," << t.entryTime << "\n";
    file.close();
}

bool CSVManager::loadTickets(std::vector<Passenger>& tickets) {
    std::ifstream file(TICKET_FILE);
    if (!file.is_open()) return false;

    tickets.clear();
    std::string line, word;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Passenger t;
        int type;

        std::getline(ss, word, ','); t.id = std::stoi(word);
        std::getline(ss, t.name, ',');
        std::getline(ss, word, ','); t.age = std::stoi(word);
        std::getline(ss, word, ','); type = std::stoi(word); t.type = (PassengerType)type;
        std::getline(ss, word, ','); t.sourceId = std::stoi(word);
        std::getline(ss, word, ','); t.destId = std::stoi(word);
        std::getline(ss, word, ','); t.ticketPrice = std::stoi(word);
        std::getline(ss, word, ','); t.entryTime = std::stol(word);

        tickets.push_back(t);
    }
    file.close();
    return true;
}

void CSVManager::saveRoutes(RailwayNetwork* network) {
    std::ofstream file(ROUTE_FILE);
    if (!file.is_open()) return;

    file << "u,v,weight,distance,line\n";
    for (int u = 0; u < (int)adj.size(); ++u) {
        for (const auto& edge : adj[u]) {
            if (u < edge.to) { // Avoid duplicates for bidirectional tracks
                file << u << "," << edge.to << "," << edge.weight << "," 
                     << edge.distance << "," << (int)edge.line << "\n";
            }
        }
    }
    file.close();
}

bool CSVManager::loadRoutes(RailwayNetwork* network) {
    std::ifstream file(ROUTE_FILE);
    if (!file.is_open()) return false;

    std::string line, word;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int u, v, w, dist, lineType;

        std::getline(ss, word, ','); u = std::stoi(word);
        std::getline(ss, word, ','); v = std::stoi(word);
        std::getline(ss, word, ','); w = std::stoi(word);
        std::getline(ss, word, ','); dist = std::stoi(word);
        std::getline(ss, word, ','); lineType = std::stoi(word);

        network->addTrack(u, v, w, dist, (LineType)lineType);
    }
    file.close();
    return true;
}
