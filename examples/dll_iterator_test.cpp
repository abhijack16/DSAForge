#include <cassert>
#include <iostream>
#include "dsaforge/dsaforge.hpp"

int main() {

    dsaforge::DoublyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    // Forward iteration
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }

    std::cout << "\n";

    // Backward iteration
    auto it = list.end();

    while (it != list.begin()) {
        --it;
        std::cout << *it << " ";
    }

    std::cout << "\n";

    // Const iteration
    const auto& constList = list;

    for (auto cit = constList.cbegin();
         cit != constList.cend(); ++cit) {
        std::cout << *cit << " ";
    }

    std::cout << "\n";

    // Range-based iteration
    for (const auto& value : list) {
        std::cout << value << " ";
    }

    std::cout << "\n";

    assert(list.count() == 3);
    assert(*list.find(20) == 20);

    return 0;
}
