
#ifndef DSAFORGE_DOUBLY_LINKED_LIST_HPP
#define DSAFORGE_DOUBLY_LINKED_LIST_HPP

#include <cstddef>
#include <iterator>
#include <utility>
#include <type_traits>

namespace dsaforge {

template <typename T>
class DoublyLinkedList {

private:

    struct Node {
        T data;
        Node* prev;
        Node* next;

        template <typename U>
        explicit Node(U&& value)
            : data(std::forward<U>(value)),
              prev(nullptr),
              next(nullptr) {}
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;

    void copyFrom(const DoublyLinkedList& other) {
        for (const Node* current = other.head_;
             current != nullptr;
             current = current->next) {
            pushBack(current->data);
        }
    }

public:

    // =====================================
    // BIDIRECTIONAL ITERATORS
    // =====================================

    template <bool IsConst>
    class BasicIterator {

    private:

        template <bool>
        friend class BasicIterator;

        using NodePointer =
            std::conditional_t<IsConst, const Node*, Node*>;

        using TailPointer =
            std::conditional_t<IsConst, Node* const*, Node**>;

        NodePointer current_;
        TailPointer tail_;

    public:

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        using pointer =
            std::conditional_t<IsConst, const T*, T*>;

        using reference =
            std::conditional_t<IsConst, const T&, T&>;

        explicit BasicIterator(
            NodePointer current = nullptr,
            TailPointer tail = nullptr
        )
            : current_(current), tail_(tail) {}

        // Mutable iterator -> Const iterator conversion
        template <
            bool OtherConst,
            std::enable_if_t<IsConst && !OtherConst, int> = 0
        >
        BasicIterator(const BasicIterator<OtherConst>& other)
            : current_(other.current_),
              tail_(other.tail_) {}

        reference operator*() const {
            return current_->data;
        }

        pointer operator->() const {
            return &current_->data;
        }

        // Prefix increment
        BasicIterator& operator++() {
            if (current_)
                current_ = current_->next;

            return *this;
        }

        // Postfix increment
        BasicIterator operator++(int) {
            BasicIterator temp = *this;
            ++(*this);
            return temp;
        }

        // Prefix decrement
        BasicIterator& operator--() {
            if (current_)
                current_ = current_->prev;
            else if (tail_)
                current_ = *tail_;

            return *this;
        }

        // Postfix decrement
        BasicIterator operator--(int) {
            BasicIterator temp = *this;
            --(*this);
            return temp;
        }

        // Mutable/const iterator comparison
        template <bool OtherConst>
        bool operator==(
            const BasicIterator<OtherConst>& other
        ) const {
            return current_ == other.current_;
        }

        template <bool OtherConst>
        bool operator!=(
            const BasicIterator<OtherConst>& other
        ) const {
            return !(*this == other);
        }
    };

    using Iterator = BasicIterator<false>;
    using ConstIterator = BasicIterator<true>;

    // =====================================
    // ITERATOR ACCESS
    // =====================================

    Iterator begin() {
        return Iterator(head_, &tail_);
    }

    Iterator end() {
        return Iterator(nullptr, &tail_);
    }

    ConstIterator begin() const {
        return ConstIterator(head_, &tail_);
    }

    ConstIterator end() const {
        return ConstIterator(nullptr, &tail_);
    }

    ConstIterator cbegin() const {
        return ConstIterator(head_, &tail_);
    }

    ConstIterator cend() const {
        return ConstIterator(nullptr, &tail_);
    }

    // =====================================
    // CONSTRUCTORS
    // =====================================

    DoublyLinkedList() = default;

    DoublyLinkedList(const DoublyLinkedList& other) {
        try {
            copyFrom(other);
        }
        catch (...) {
            clear();
            throw;
        }
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head_(std::exchange(other.head_, nullptr)),
          tail_(std::exchange(other.tail_, nullptr)),
          size_(std::exchange(other.size_, 0)) {}

    // =====================================
    // ASSIGNMENT OPERATORS
    // =====================================

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList temp(other);
            swap(temp);
        }

        return *this;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
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

    ~DoublyLinkedList() {
        clear();
    }

    // =====================================
    // INSERT AT FRONT
    // =====================================

    void pushFront(const T& value) {
        emplaceFront(value);
    }

    void pushFront(T&& value) {
        emplaceFront(std::move(value));
    }

    template <typename... Args>
    T& emplaceFront(Args&&... args) {
        Node* node = new Node(
            T(std::forward<Args>(args)...)
        );

        node->next = head_;

        if (head_)
            head_->prev = node;
        else
            tail_ = node;

        head_ = node;
        ++size_;

        return node->data;
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

        node->prev = tail_;

        if (tail_)
            tail_->next = node;
        else
            head_ = node;

        tail_ = node;
        ++size_;

        return node->data;
    }

    // =====================================
    // DELETE FRONT
    // =====================================

    bool popFront() {
        if (!head_)
            return false;

        Node* temp = head_;

        head_ = head_->next;

        if (head_)
            head_->prev = nullptr;
        else
            tail_ = nullptr;

        delete temp;
        --size_;

        return true;
    }

    // =====================================
    // DELETE BACK
    // =====================================

    bool popBack() {
        if (!tail_)
            return false;

        Node* temp = tail_;

        tail_ = tail_->prev;

        if (tail_)
            tail_->next = nullptr;
        else
            head_ = nullptr;

        delete temp;
        --size_;

        return true;
    }

    // =====================================
    // DELETE BY VALUE
    // =====================================

    bool remove(const T& value) {
        Node* current = head_;

        while (current) {

            if (current->data == value) {

                if (current == head_)
                    return popFront();

                if (current == tail_)
                    return popBack();

                current->prev->next = current->next;
                current->next->prev = current->prev;

                delete current;
                --size_;

                return true;
            }

            current = current->next;
        }

        return false;
    }

    // =====================================
    // FIND
    // =====================================

    T* find(const T& value) {
        for (Node* current = head_;
             current;
             current = current->next) {

            if (current->data == value)
                return &current->data;
        }

        return nullptr;
    }

    const T* find(const T& value) const {
        for (const Node* current = head_;
             current;
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
        while (head_) {
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

    void swap(DoublyLinkedList& other) noexcept {
        using std::swap;

        swap(head_, other.head_);
        swap(tail_, other.tail_);
        swap(size_, other.size_);
    }

};

// =====================================
// NON-MEMBER SWAP
// =====================================

template <typename T>
void swap(
    DoublyLinkedList<T>& lhs,
    DoublyLinkedList<T>& rhs
) noexcept {
    lhs.swap(rhs);
}

} // namespace dsaforge

#endif
