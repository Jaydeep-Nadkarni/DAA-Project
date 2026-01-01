/**
 * ======================================================================================
 * HEADER: queue_manager.h
 * DESCRIPTION: Custom data structure implementations (Node, Stack, Queue, List)
 *              and Platform Queue management
 * ======================================================================================
 */

#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include <iostream>

// ======================================================================================
//                                   CUSTOM DATA STRUCTURES
// ======================================================================================

/**
 * Template Class: MyNode
 * Generic node for Linked List, Stack, and Queue.
 */
template <typename T>
class MyNode {
public:
    T data;
    MyNode* next;
    MyNode* prev; // For Doubly Linked List

    MyNode(T val) : data(val), next(NULL), prev(NULL) {}
};

/**
 * Template Class: MyStack
 * Custom implementation of Stack using Linked List.
 * Operations: push, pop, top, empty.
 * Time Complexity: O(1) for all operations.
 */
template <typename T>
class MyStack {
    MyNode<T>* topNode;
    int count;

public:
    MyStack() : topNode(NULL), count(0) {}

    void push(T val) {
        MyNode<T>* newNode = new MyNode<T>(val);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    void pop() {
        if (topNode) {
            MyNode<T>* temp = topNode;
            topNode = topNode->next;
            delete temp;
            count--;
        }
    }

    T top() {
        if (topNode) return topNode->data;
        return T(); // Return default if empty
    }

    bool empty() { return topNode == NULL; }
    int size() { return count; }
};

/**
 * Template Class: MyQueue
 * Custom implementation of Queue using Linked List.
 * Operations: push, pop, front, empty.
 * Time Complexity: O(1) for all operations.
 */
template <typename T>
class MyQueue {
    MyNode<T>* frontNode;
    MyNode<T>* rearNode;
    int count;

public:
    MyQueue() : frontNode(NULL), rearNode(NULL), count(0) {}

    void push(T val) {
        MyNode<T>* newNode = new MyNode<T>(val);
        if (rearNode == NULL) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    void pop() {
        if (frontNode == NULL) return;
        MyNode<T>* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == NULL) rearNode = NULL;
        delete temp;
        count--;
    }

    T front() {
        if (frontNode) return frontNode->data;
        return T();
    }

    bool empty() { return frontNode == NULL; }
    int size() { return count; }
};

/**
 * Template Class: MyList
 * Custom implementation of Doubly Linked List.
 */
template <typename T>
class MyList {
public:
    MyNode<T>* head;
    MyNode<T>* tail;
    int count;

    MyList() : head(NULL), tail(NULL), count(0) {}

    void push_back(T val) {
        MyNode<T>* newNode = new MyNode<T>(val);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void display() {
        MyNode<T>* temp = head;
        while (temp) {
            // Assuming T has operator<< overloaded or is primitive
            // std::cout << temp->data << " "; 
            temp = temp->next;
        }
    }
};

// ======================================================================================
//                                   PLATFORM QUEUE
// ======================================================================================

/**
 * Circular Queue for Platform Load Balancing
 */
class PlatformQueue {
    int *arr;
    int front, rear, size, capacity;

public:
    PlatformQueue(int cap = 5);
    ~PlatformQueue() { delete[] arr; }

    bool isFull();
    bool isEmpty();
    void enqueue(int trainId);
    int dequeue();
};

#endif // QUEUE_MANAGER_H
