/**
 * ======================================================================================
 * IMPLEMENTATION: queue_manager.cpp
 * DESCRIPTION: Implementation of PlatformQueue (Circular Queue)
 * ======================================================================================
 */

#include "../include/queue_manager.h"
#include <iostream>

// ======================================================================================
//                                   PLATFORM QUEUE IMPLEMENTATION
// ======================================================================================

PlatformQueue::PlatformQueue(int cap) {
    capacity = cap;
    arr = new int[capacity];
    front = -1;
    rear = -1;
    size = 0;
}

bool PlatformQueue::isFull() {
    return size == capacity;
}

bool PlatformQueue::isEmpty() {
    return size == 0;
}

void PlatformQueue::enqueue(int trainId) {
    if (isFull()) {
        std::cout << "Platform Queue Full! Train " << trainId << " must wait.\n";
        return;
    }
    if (front == -1) front = 0;
    rear = (rear + 1) % capacity;
    arr[rear] = trainId;
    size++;
    std::cout << "Train " << trainId << " assigned to platform buffer.\n";
}

int PlatformQueue::dequeue() {
    if (isEmpty()) return -1;
    int item = arr[front];
    if (front == rear) {
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % capacity;
    }
    size--;
    return item;
}
