#ifndef NODE_H
#define NODE_H


class Node {
public:

    Node(int xValue, int yValue);
    int xValue;
    int yValue;
    Node* next;
};

#endif
