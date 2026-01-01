/**
 * ======================================================================================
 * IMPLEMENTATION: analytics.cpp
 * DESCRIPTION: Implementation of StationBST class
 * ======================================================================================
 */

#include "../include/analytics.h"
#include <iostream>
#include <iomanip>

// ======================================================================================
//                                   STATION BST NODE IMPLEMENTATION
// ======================================================================================

StationBST::Node::Node(std::string n, int i) : name(n), id(i), left(NULL), right(NULL) {}

// ======================================================================================
//                                   STATION BST IMPLEMENTATION
// ======================================================================================

StationBST::StationBST() : root(NULL) {}

StationBST::Node* StationBST::insert(Node* node, std::string name, int id) {
    if (node == NULL) return new Node(name, id);
    if (name < node->name) node->left = insert(node->left, name, id);
    else if (name > node->name) node->right = insert(node->right, name, id);
    return node;
}

StationBST::Node* StationBST::search(Node* node, std::string name) {
    if (node == NULL || node->name == name) return node;
    if (name < node->name) return search(node->left, name);
    return search(node->right, name);
}

void StationBST::inorder(Node* node) {
    if (node != NULL) {
        inorder(node->left);
        std::cout << "  - " << std::left << std::setw(20) << node->name 
                  << " (ID: " << node->id << ")" << std::endl;
        inorder(node->right);
    }
}

void StationBST::addStation(std::string name, int id) {
    root = insert(root, name, id);
}

int StationBST::getStationId(std::string name) {
    Node* res = search(root, name);
    if (res) return res->id;
    return -1;
}

void StationBST::listStations() {
    std::cout << "\n--- Station Directory (Sorted Alphabetically via BST) ---\n";
    inorder(root);
    std::cout << "---------------------------------------------------------\n";
}
