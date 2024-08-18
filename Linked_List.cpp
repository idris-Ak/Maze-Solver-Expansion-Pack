#include "Linked_List.h"
#include <iostream>

LinkedList::LinkedList() {
    head = nullptr;
}

LinkedList::~LinkedList() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void LinkedList::append(int x, int y) {
    Node* newNode = new Node(x,y);
    if (!head) {
        head = newNode;
        return;
    }

    Node* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
}

void LinkedList::prepend(int x, int y) {
    Node* newNode = new Node(x, y);
    newNode->next = head;
    head = newNode;
}

void LinkedList::deleteNode(Node* node) {
    if (!head || !node) return;

    Node* current = head;
    Node* previous = nullptr;
    bool nodeFound = false;

    while (current && !nodeFound) {
        if (current == node) {
            if (previous) {
                previous->next = current->next;
            } else {
                head = current->next;
            }
            delete current;
            nodeFound = true;
        } else {
            previous = current;
            current = current->next;
        }
    }
}



Node* LinkedList::getLastNode() {
    if (!head) return nullptr;

    Node* current = head;
    while (current->next) {
        current = current->next;
    }

    return current;
}


bool LinkedList::isEmpty() {
    return head == nullptr;
}




