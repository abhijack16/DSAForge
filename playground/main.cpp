#include <iostream>
#include <dsaforge/dsaforge.hpp>

int main() {

    dsaforge::SinglyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    for (const auto& x : list) {
        std::cout << x << " ";
    }

    return 0;
}