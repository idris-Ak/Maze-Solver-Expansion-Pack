#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"  // Include the Node.h header


class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void append(int x,int y);
    void prepend(int x,int y);
    void deleteNode(Node* node);
    bool isEmpty();
    Node* getLastNode(); 

private:
    Node* head;
};

#endif
