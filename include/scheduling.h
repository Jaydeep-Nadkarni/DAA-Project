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

        if (left < heap.size() && heap[left] < heap[smallest]) smallest = left;
        if (right < heap.size() && heap[right] < heap[smallest]) smallest = right;

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

    bool empty() { return heap.empty(); }
    int size() { return heap.size(); }
    
    // Helper to get underlying vector for display (not standard heap op but useful for UI)
    std::vector<T> getVector() { return heap; }
};

// ======================================================================================
//                                   SCHEDULER CLASS
// ======================================================================================

/**
 * Train Scheduler using MinHeap
 */
class Scheduler {
    MinHeap<Train> trainSchedule;

public:
    void scheduleTrain(int id, std::string name, int time, int startStationId);
    void showUpcomingTrains();
    void optimizeFrequency(bool isPeakHour);
};

#endif // SCHEDULING_H
