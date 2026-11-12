#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "dsaforge/dsaforge.hpp"

using dsaforge::SinglyLinkedList;
using dsaforge::DoublyLinkedList;

// ==========================================
// HELPER: VERIFY LIST CONTENTS
// ==========================================

template <typename List>
void checkForward(const List& list, const std::vector<int>& expected) {
    assert(list.count() == expected.size());

    std::size_t index = 0;

    for (const auto& value : list) {
        assert(index < expected.size());
        assert(value == expected[index]);
        ++index;
    }

    assert(index == expected.size());
}

// ==========================================
// SINGLY LINKED LIST TESTS
// ==========================================

void testSLLBasicOperations() {
    SinglyLinkedList<int> list;

    assert(list.isEmpty());
    assert(list.count() == 0);

    assert(!list.deleteHead());
    assert(!list.deleteTail());
    assert(!list.remove(10));
    assert(list.find(10) == nullptr);

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    assert(!list.isEmpty());
    checkForward(list, {10, 20, 30});

    assert(list.find(20) != nullptr);
    assert(*list.find(20) == 20);
    assert(list.find(100) == nullptr);

    *list.find(20) = 25;
    checkForward(list, {10, 25, 30});

    assert(list.remove(25));
    assert(!list.remove(25));
    checkForward(list, {10, 30});

    assert(list.deleteHead());
    checkForward(list, {30});

    assert(list.deleteTail());
    assert(list.isEmpty());
    assert(list.count() == 0);
}

void testSLLDeletionCases() {
    SinglyLinkedList<int> list;

    // Delete only node using deleteHead.
    list.pushBack(10);
    assert(list.deleteHead());
    assert(list.isEmpty());

    // Delete only node using deleteTail.
    list.pushBack(20);
    assert(list.deleteTail());
    assert(list.isEmpty());

    // Delete only node using remove.
    list.pushBack(30);
    assert(list.remove(30));
    assert(list.isEmpty());

    // Delete head.
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    assert(list.remove(1));
    checkForward(list, {2, 3});

    // Delete tail.
    assert(list.remove(3));
    checkForward(list, {2});

    // Delete middle.
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(5);

    assert(list.remove(4));
    checkForward(list, {2, 3, 5});

    // Missing value.
    assert(!list.remove(100));
    checkForward(list, {2, 3, 5});
}

void testSLLDuplicates() {
    SinglyLinkedList<int> list;

    list.pushBack(5);
    list.pushBack(10);
    list.pushBack(5);
    list.pushBack(5);

    assert(list.count() == 4);

    // remove() should remove only the first match.
    assert(list.remove(5));
    checkForward(list, {10, 5, 5});

    assert(list.remove(5));
    checkForward(list, {10, 5});

    assert(list.remove(5));
    checkForward(list, {10});

    assert(!list.remove(5));
}

void testSLLCopyAndMove() {
    SinglyLinkedList<int> original;

    original.pushBack(10);
    original.pushBack(20);
    original.pushBack(30);

    // Copy constructor.
    SinglyLinkedList<int> copied(original);

    checkForward(copied, {10, 20, 30});

    copied.deleteHead();

    checkForward(original, {10, 20, 30});
    checkForward(copied, {20, 30});

    // Copy assignment.
    SinglyLinkedList<int> assigned;
    assigned.pushBack(99);
    assigned = original;

    checkForward(assigned, {10, 20, 30});

    // Self copy assignment.
    assigned = assigned;
    checkForward(assigned, {10, 20, 30});

    // Move constructor.
    SinglyLinkedList<int> moved(std::move(copied));

    checkForward(moved, {20, 30});
    assert(copied.isEmpty());

    // Move assignment.
    SinglyLinkedList<int> moveAssigned;
    moveAssigned.pushBack(100);
    moveAssigned = std::move(moved);

    checkForward(moveAssigned, {20, 30});
    assert(moved.isEmpty());

    // Self move assignment.
    moveAssigned = std::move(moveAssigned);
    checkForward(moveAssigned, {20, 30});
}

void testSLLIterators() {
    SinglyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    // Mutable range-based iteration.
    for (int& value : list) {
        value += 1;
    }

    checkForward(list, {11, 21, 31});

    // Iterator increment.
    auto it = list.begin();

    assert(*it == 11);

    auto previous = it++;
    assert(*previous == 11);
    assert(*it == 21);

    ++it;
    assert(*it == 31);

    ++it;
    assert(it == list.end());

    // Const iteration.
    const auto& constList = list;

    std::vector<int> values;

    for (const int& value : constList) {
        values.push_back(value);
    }

    assert((values == std::vector<int>{11, 21, 31}));

    auto cit = constList.cbegin();
    assert(*cit == 11);

    ++cit;
    assert(*cit == 21);
}

void testSLLClearAndReuse() {
    SinglyLinkedList<int> list;

    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    list.clear();

    assert(list.isEmpty());
    assert(list.count() == 0);

    // Clear an already empty list.
    list.clear();
    assert(list.isEmpty());

    // Reuse after clear.
    list.pushBack(100);
    checkForward(list, {100});

    assert(list.deleteTail());
    assert(list.isEmpty());
}

void testSLLStress() {
    SinglyLinkedList<int> list;

    constexpr int N = 10000;

    for (int i = 0; i < N; ++i) {
        list.pushBack(i);
    }

    assert(list.count() == N);

    for (int i = 0; i < N; ++i) {
        assert(list.deleteHead());
    }

    assert(list.isEmpty());

    // Repeated tail deletions.
    for (int i = 0; i < N; ++i) {
        list.pushBack(i);
    }

    for (int i = N - 1; i >= 0; --i) {
        assert(list.deleteTail());
    }

    assert(list.isEmpty());
}

// ==========================================
// DOUBLY LINKED LIST TESTS
// ==========================================

void testDLLBasicOperations() {
    DoublyLinkedList<int> list;

    assert(list.isEmpty());
    assert(list.count() == 0);

    assert(!list.popFront());
    assert(!list.popBack());
    assert(!list.remove(10));
    assert(list.find(10) == nullptr);

    list.pushBack(20);
    list.pushFront(10);
    list.pushBack(30);

    checkForward(list, {10, 20, 30});

    assert(list.find(20) != nullptr);
    assert(*list.find(20) == 20);
    assert(list.find(100) == nullptr);

    *list.find(20) = 25;
    checkForward(list, {10, 25, 30});

    assert(list.remove(25));
    checkForward(list, {10, 30});

    assert(list.popFront());
    checkForward(list, {30});

    assert(list.popBack());
    assert(list.isEmpty());
}

void testDLLDeletionCases() {
    DoublyLinkedList<int> list;

    // Delete only node using popFront.
    list.pushBack(10);
    assert(list.popFront());
    assert(list.isEmpty());

    // Delete only node using popBack.
    list.pushBack(20);
    assert(list.popBack());
    assert(list.isEmpty());

    // Delete only node using remove.
    list.pushBack(30);
    assert(list.remove(30));
    assert(list.isEmpty());

    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(5);

    // Head.
    assert(list.remove(1));
    checkForward(list, {2, 3, 4, 5});

    // Tail.
    assert(list.remove(5));
    checkForward(list, {2, 3, 4});

    // Middle.
    assert(list.remove(3));
    checkForward(list, {2, 4});

    // Missing value.
    assert(!list.remove(100));
    checkForward(list, {2, 4});
}

void testDLLDuplicates() {
    DoublyLinkedList<int> list;

    list.pushBack(5);
    list.pushBack(10);
    list.pushBack(5);
    list.pushBack(5);

    assert(list.count() == 4);

    assert(list.remove(5));
    checkForward(list, {10, 5, 5});

    assert(list.remove(5));
    checkForward(list, {10, 5});

    assert(list.remove(5));
    checkForward(list, {10});

    assert(!list.remove(5));
}

void testDLLCopyAndMove() {
    DoublyLinkedList<int> original;

    original.pushBack(10);
    original.pushBack(20);
    original.pushBack(30);

    // Copy constructor.
    DoublyLinkedList<int> copied(original);

    checkForward(copied, {10, 20, 30});

    copied.popFront();

    checkForward(original, {10, 20, 30});
    checkForward(copied, {20, 30});

    // Copy assignment.
    DoublyLinkedList<int> assigned;
    assigned.pushBack(99);
    assigned = original;

    checkForward(assigned, {10, 20, 30});

    // Self copy assignment.
    assigned = assigned;
    checkForward(assigned, {10, 20, 30});

    // Move constructor.
    DoublyLinkedList<int> moved(std::move(copied));

    checkForward(moved, {20, 30});
    assert(copied.isEmpty());

    // Move assignment.
    DoublyLinkedList<int> moveAssigned;
    moveAssigned.pushBack(100);
    moveAssigned = std::move(moved);

    checkForward(moveAssigned, {20, 30});
    assert(moved.isEmpty());

    // Self move assignment.
    moveAssigned = std::move(moveAssigned);
    checkForward(moveAssigned, {20, 30});
}

void testDLLIterators() {
    DoublyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    // Mutable forward iteration.
    for (int& value : list) {
        value += 1;
    }

    checkForward(list, {11, 21, 31});

    // Forward iterator operations.
    auto it = list.begin();

    assert(*it == 11);

    auto previous = it++;
    assert(*previous == 11);
    assert(*it == 21);

    ++it;
    assert(*it == 31);

    ++it;
    assert(it == list.end());

    // Backward iteration using --end().
    std::vector<int> reversed;

    auto reverseIt = list.end();

    while (reverseIt != list.begin()) {
        --reverseIt;
        reversed.push_back(*reverseIt);
    }

    assert((reversed == std::vector<int>{31, 21, 11}));

    // Const iteration.
    const auto& constList = list;

    std::vector<int> values;

    for (const int& value : constList) {
        values.push_back(value);
    }

    assert((values == std::vector<int>{11, 21, 31}));

    auto cit = constList.cend();
    --cit;
    assert(*cit == 31);

    --cit;
    assert(*cit == 21);
}

void testDLLClearAndReuse() {
    DoublyLinkedList<int> list;

    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    list.clear();

    assert(list.isEmpty());
    assert(list.count() == 0);

    list.clear();
    assert(list.isEmpty());

    list.pushFront(100);
    list.pushBack(200);

    checkForward(list, {100, 200});

    assert(list.popFront());
    assert(list.popBack());

    assert(list.isEmpty());
}

void testDLLStress() {
    DoublyLinkedList<int> list;

    constexpr int N = 10000;

    for (int i = 0; i < N; ++i) {
        list.pushBack(i);
    }

    assert(list.count() == N);

    for (int i = 0; i < N / 2; ++i) {
        assert(list.popFront());
        assert(list.popBack());
    }

    assert(list.isEmpty());

    // Stress insertion at both ends.
    for (int i = 0; i < N; ++i) {
        list.pushFront(i);
        list.pushBack(i);
    }

    assert(list.count() == 2 * N);

    list.clear();

    assert(list.isEmpty());
}

// ==========================================
// TYPE AND ITERATOR CATEGORY CHECKS
// ==========================================

void testIteratorTypes() {
    static_assert(
        std::is_same<
            std::iterator_traits<SinglyLinkedList<int>::Iterator>::iterator_category,
            std::forward_iterator_tag
        >::value,
        "SLL iterator must be a forward iterator"
    );

    static_assert(
        std::is_same<
            std::iterator_traits<DoublyLinkedList<int>::Iterator>::iterator_category,
            std::bidirectional_iterator_tag
        >::value,
        "DLL iterator must be a bidirectional iterator"
    );

    static_assert(
        std::is_same<
            decltype(*std::declval<SinglyLinkedList<int>::ConstIterator>()),
            const int&
        >::value,
        "SLL const iterator must return const reference"
    );

    static_assert(
        std::is_same<
            decltype(*std::declval<DoublyLinkedList<int>::ConstIterator>()),
            const int&
        >::value,
        "DLL const iterator must return const reference"
    );
}

// ==========================================
// MAIN
// ==========================================

int main() {
    testSLLBasicOperations();
    testSLLDeletionCases();
    testSLLDuplicates();
    testSLLCopyAndMove();
    testSLLIterators();
    testSLLClearAndReuse();
    testSLLStress();

    testDLLBasicOperations();
    testDLLDeletionCases();
    testDLLDuplicates();
    testDLLCopyAndMove();
    testDLLIterators();
    testDLLClearAndReuse();
    testDLLStress();

    testIteratorTypes();

    return 0;
}
