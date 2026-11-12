#include <iostream>
#include "dsaforge/dsaforge.hpp"

int main() {
    dsaforge::SinglyLinkedList<int> singly;
    singly.pushBack(10);
    singly.pushBack(20);
    singly.pushBack(30);

    std::cout << "Singly linked list count: " << singly.count() << '\n';
    std::cout << "Contains 20: " << (singly.find(20) != nullptr) << '\n';

    dsaforge::DoublyLinkedList<int> doubly;
    doubly.pushFront(5);
    doubly.pushBack(10);
    doubly.pushBack(15);

    std::cout << "Doubly linked list count: " << doubly.count() << '\n';
    doubly.remove(10);
    std::cout << "After removing 10: " << doubly.count() << '\n';
}
