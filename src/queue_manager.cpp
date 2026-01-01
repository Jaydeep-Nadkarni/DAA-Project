/**
 * ======================================================================================
 * IMPLEMENTATION: queue_manager.cpp
 * DESCRIPTION: Implementation of PlatformQueue (Circular Queue for Load Balancing)
 * 
 * DATA STRUCTURE: Circular Queue
 * - Fixed-size array-based implementation
 * - Efficient space utilization through circular indexing
 * - O(1) enqueue and dequeue operations
 * - Used for platform management and train scheduling
 * ======================================================================================
 */

#include "../include/queue_manager.h"
#include <iostream>

// ======================================================================================
//                                   PLATFORM QUEUE IMPLEMENTATION
// ======================================================================================

/**
 * Constructor: PlatformQueue
 * Initializes a circular queue for platform load balancing
 * 
 * Parameters:
 *   cap - Maximum capacity of the platform queue (default: 5)
 * 
 * Implementation Details:
 *   - Dynamic memory allocation for array
 *   - front and rear initialized to -1 (empty state)
 *   - size tracks current number of elements
 * 
 * Use Case: Managing trains waiting for platform assignment
 */
PlatformQueue::PlatformQueue(int cap) {
    capacity = cap;
    arr = new int[capacity];
    front = -1;
    rear = -1;
    size = 0;
}

/**
 * Function: isFull
 * Checks if the platform queue is at maximum capacity
 * 
 * Returns: true if queue is full, false otherwise
 * Time Complexity: O(1)
 */
bool PlatformQueue::isFull() {
    return size == capacity;
}

/**
 * Function: isEmpty
 * Checks if the platform queue is empty
 * 
 * Returns: true if queue is empty, false otherwise
 * Time Complexity: O(1)
 */
bool PlatformQueue::isEmpty() {
    return size == 0;
}

/**
 * Function: enqueue
 * Adds a train to the platform queue (circular queue insertion)
 * 
 * Parameters:
 *   trainId - Unique identifier of the train
 * 
 * Algorithm:
 *   1. Check if queue is full
 *   2. Initialize front pointer if queue was empty
 *   3. Use modulo arithmetic for circular indexing: rear = (rear + 1) % capacity
 *   4. Insert train ID and increment size
 * 
 * Time Complexity: O(1)
 * 
 * Real-world scenario: Train arrives at station and waits for platform allocation
 */
void PlatformQueue::enqueue(int trainId) {
    if (isFull()) {
        std::cout << "Platform Queue Full! Train " << trainId << " must wait.\n";
        return;
    }
    if (front == -1) front = 0;  // First element initialization
    rear = (rear + 1) % capacity;  // Circular increment
    arr[rear] = trainId;
    size++;
    std::cout << "Train " << trainId << " assigned to platform buffer.\n";
}

/**
 * Function: dequeue
 * Removes and returns a train from the platform queue (circular queue deletion)
 * 
 * Returns: Train ID if successful, -1 if queue is empty
 * 
 * Algorithm:
 *   1. Check if queue is empty
 *   2. Retrieve element at front
 *   3. If only one element, reset front and rear to -1
 *   4. Otherwise, use circular increment: front = (front + 1) % capacity
 *   5. Decrement size
 * 
 * Time Complexity: O(1)
 * 
 * Real-world scenario: Platform becomes available, allocate to waiting train
 */
int PlatformQueue::dequeue() {
    if (isEmpty()) return -1;
    int item = arr[front];
    if (front == rear) {
        // Last element - reset queue
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % capacity;  // Circular increment
    }
    size--;
    return item;
}
