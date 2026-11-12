#include <iostream>
#include "dsaforge/dsaforge.hpp"

int main() {
    dsaforge::SinglyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    const auto& constList = list;

    for (auto it = constList.cbegin(); it != constList.cend(); ++it) {
        std::cout << *it << " ";
    }

    std::cout << '\n';

    for (const auto& value : constList) {
        std::cout << value << " ";
    }

    std::cout << '\n';
}
