#ifndef DSAFORGE_SINGLY_LINKED_LIST_HPP
#define DSAFORGE_SINGLY_LINKED_LIST_HPP

#include <cstddef>
#include <iterator>
#include <utility>

namespace dsaforge {

template <typename T>
class SinglyLinkedList {

private:

    // Node structure
    struct Node {
        T data;
        Node* next;

        template <typename U>
        explicit Node(U&& value)
            : data(std::forward<U>(value)), next(nullptr) {}
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;

    void copyFrom(const SinglyLinkedList& other) {
        for (const Node* current = other.head_;
             current != nullptr;
             current = current->next) {
            pushBack(current->data);
        }
    }

public:

    // =====================================
    // MUTABLE ITERATOR
    // =====================================

    class Iterator {
    private:
        Node* current_;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(Node* node = nullptr)
            : current_(node) {}

        reference operator*() const {
            return current_->data;
        }

        pointer operator->() const {
            return &current_->data;
        }

        Iterator& operator++() {
            if (current_)
                current_ = current_->next;

            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };


    // =====================================
    // CONSTANT ITERATOR
    // =====================================

    class ConstIterator {
    private:
        const Node* current_;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        explicit ConstIterator(const Node* node = nullptr)
            : current_(node) {}

        reference operator*() const {
            return current_->data;
        }

        pointer operator->() const {
            return &current_->data;
        }

        ConstIterator& operator++() {
            if (current_)
                current_ = current_->next;

            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const ConstIterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }
    };


    // =====================================
    // ITERATOR ACCESS
    // =====================================

    Iterator begin() {
        return Iterator(head_);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    ConstIterator begin() const {
        return ConstIterator(head_);
    }

    ConstIterator end() const {
        return ConstIterator(nullptr);
    }

    ConstIterator cbegin() const {
        return ConstIterator(head_);
    }

    ConstIterator cend() const {
        return ConstIterator(nullptr);
    }


    // =====================================
    // CONSTRUCTORS
    // =====================================

    SinglyLinkedList() = default;

    // Copy constructor
    SinglyLinkedList(const SinglyLinkedList& other) {
        try {
            copyFrom(other);
        }
        catch (...) {
            clear();
            throw;
        }
    }

    // Move constructor
    SinglyLinkedList(SinglyLinkedList&& other) noexcept
        : head_(std::exchange(other.head_, nullptr)),
          tail_(std::exchange(other.tail_, nullptr)),
          size_(std::exchange(other.size_, 0)) {}


    // =====================================
    // ASSIGNMENT OPERATORS
    // =====================================

    // Copy assignment
    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this != &other) {
            SinglyLinkedList temp(other);
            swap(temp);
        }

        return *this;
    }

    // Move assignment
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();

            head_ = std::exchange(other.head_, nullptr);
            tail_ = std::exchange(other.tail_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }

        return *this;
    }


    // =====================================
    // DESTRUCTOR
    // =====================================

    ~SinglyLinkedList() {
        clear();
    }


    // =====================================
    // INSERT AT BACK
    // =====================================

    void pushBack(const T& value) {
        emplaceBack(value);
    }

    void pushBack(T&& value) {
        emplaceBack(std::move(value));
    }

    template <typename... Args>
    T& emplaceBack(Args&&... args) {
        Node* node = new Node(
            T(std::forward<Args>(args)...)
        );

        if (head_ == nullptr) {
            head_ = tail_ = node;
        }
        else {
            tail_->next = node;
            tail_ = node;
        }

        ++size_;

        return node->data;
    }


    // =====================================
    // DELETE HEAD
    // =====================================

    bool deleteHead() {
        if (head_ == nullptr)
            return false;

        Node* temp = head_;

        head_ = head_->next;

        delete temp;
        --size_;

        if (head_ == nullptr)
            tail_ = nullptr;

        return true;
    }


    // =====================================
    // DELETE TAIL
    // =====================================

    bool deleteTail() {
        if (head_ == nullptr)
            return false;

        if (head_ == tail_) {
            delete head_;

            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;

            return true;
        }

        Node* previous = head_;

        while (previous->next != tail_) {
            previous = previous->next;
        }

        delete tail_;

        tail_ = previous;
        tail_->next = nullptr;

        --size_;

        return true;
    }


    // =====================================
    // DELETE BY VALUE
    // =====================================

    bool remove(const T& value) {
        if (head_ == nullptr)
            return false;

        if (head_->data == value)
            return deleteHead();

        Node* previous = head_;
        Node* current = head_->next;

        while (current != nullptr) {

            if (current->data == value) {

                previous->next = current->next;

                if (current == tail_)
                    tail_ = previous;

                delete current;
                --size_;

                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }


    // =====================================
    // FIND
    // =====================================

    T* find(const T& value) {
        for (Node* current = head_;
             current != nullptr;
             current = current->next) {

            if (current->data == value)
                return &current->data;
        }

        return nullptr;
    }

    const T* find(const T& value) const {
        for (const Node* current = head_;
             current != nullptr;
             current = current->next) {

            if (current->data == value)
                return &current->data;
        }

        return nullptr;
    }


    // =====================================
    // COUNT AND EMPTY CHECK
    // =====================================

    bool isEmpty() const noexcept {
        return size_ == 0;
    }

    std::size_t count() const noexcept {
        return size_;
    }


    // =====================================
    // CLEAR
    // =====================================

    void clear() noexcept {
        while (head_ != nullptr) {
            Node* temp = head_;

            head_ = head_->next;

            delete temp;
        }

        tail_ = nullptr;
        size_ = 0;
    }


    // =====================================
    // SWAP
    // =====================================

    void swap(SinglyLinkedList& other) noexcept {
        using std::swap;

        swap(head_, other.head_);
        swap(tail_, other.tail_);
        swap(size_, other.size_);
    }

};


// Non-member swap
template <typename T>
void swap(
    SinglyLinkedList<T>& lhs,
    SinglyLinkedList<T>& rhs
) noexcept {
    lhs.swap(rhs);
}

} // namespace dsaforge

#endif
