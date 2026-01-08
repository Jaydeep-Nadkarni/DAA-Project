/**
 * ======================================================================================
 * HEADER: scheduling.h
 * DESCRIPTION: Train scheduling system with Train struct, MinHeap, and Scheduler
 * ======================================================================================
 */

#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <string>
#include <vector>
#include "station.h"

// ======================================================================================
//                                   TRAIN STRUCTURE
// ======================================================================================

struct Train {
    int trainId;
    std::string name;
    int capacity;
    int currentLoad;
    int nextStationId;
    int arrivalTime; // Minutes from midnight
    TrainStatus status;

    // Operator for MinHeap
    bool operator>(const Train& other) const {
        return arrivalTime > other.arrivalTime;
    }
    bool operator<(const Train& other) const {
        return arrivalTime < other.arrivalTime;
    }
};

// ======================================================================================
//                                   MIN HEAP TEMPLATE
// ======================================================================================

/**
 * Template Class: MinHeap
 * Custom implementation of Min Heap for Priority Queue.
 */
template <typename T>
class MinHeap {
    std::vector<T> heap;

    void heapifyUp(int index) {
        if (index == 0) return;
        int parent = (index - 1) / 2;
        if (heap[parent] > heap[index]) {
            std::swap(heap[parent], heap[index]);
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < (int)heap.size() && heap[left] < heap[smallest]) smallest = left;
        if (right < (int)heap.size() && heap[right] < heap[smallest]) smallest = right;

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    void push(T val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    T top() {
        if (!heap.empty()) return heap[0];
        return T();
    }

    bool empty() const { return heap.empty(); }
    int size() const { return heap.size(); }
    
    // Helper to get underlying vector for display (not standard heap op but useful for UI)
    std::vector<T> getVector() { return heap; }
};

// ======================================================================================
//                                   SCHEDULER CLASS
// ======================================================================================

/**
 * Train Scheduler using Custom MinHeap
 * 
 * Features:
 * - Priority-based scheduling (earliest arrival time first)
 * - Automatic sorting via MinHeap (O(log n) insertion)
 * - Peak hour optimization with dynamic frequency adjustment
 * - Real-time schedule display with time formatting
 * - Train status tracking (ON_TIME, DELAYED, CANCELLED)
 * 
 * Data Structure: MinHeap<Train>
 * - Maintains trains sorted by arrivalTime
 * - Efficient O(log n) operations for add/remove
 * - Custom operator< in Train struct defines priority
 */
class Scheduler {
    MinHeap<Train> trainSchedule;  // Priority queue using custom MinHeap

public:
    // Core Scheduling Operations
    void scheduleTrain(int id, std::string name, int time, int startStationId);
    void showUpcomingTrains();          // Display schedule in chronological order
    void showTrainsAtStation(int stationId);  // Show trains arriving at a specific station
    void optimizeFrequency(bool isPeakHour);  // Dynamic frequency management
    
    // Getters for monitoring
    int getTotalScheduledTrains() const { return trainSchedule.size(); }
    bool hasScheduledTrains() const { return !trainSchedule.empty(); }
};

#endif // SCHEDULING_H
