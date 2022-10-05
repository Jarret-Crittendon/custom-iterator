#include <memory>
#include <iterator>

template <typename> class Jarray;

template <typename T>
auto swap(Jarray<T>&, Jarray<T>&) -> void;

template <typename T>
class Jarray {
    class Iterator;
    auto swap(Jarray&, Jarray&) -> void;
public:
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator = Iterator;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    Jarray():
        start(nullptr), first_free(nullptr), cap(nullptr) { }

    auto push_back(const T&) -> void;

    auto size() const -> size_type {
        return first_free - start;
    }

    auto empty() const -> bool {
        return size() == 0;
    }

    auto capacity() const -> size_type {
        return cap - start;
    }

    auto begin() -> iterator {
        return Iterator(start);
    }

    auto end() -> iterator {
        return Iterator(first_free);
    }

    ~Jarray();


private:
    auto check_alloc() -> void {
        if (size() == capacity()) {
            reallocate();
        }
    }
    auto reallocate() -> void;
    auto free() -> void;


    T* start;
    T* first_free;
    T* cap;


    std::allocator<T> alloc;
    std::allocator_traits<decltype(alloc)> alloc_t;
};

template <typename T>
auto Jarray<T>::push_back(const T& value) -> void {
    check_alloc();
    alloc_t.construct(alloc, first_free++, value);
}

template <typename T>
auto Jarray<T>::reallocate() -> void {
    size_type new_size = size() ? 2 * size() : 1;
    auto new_start = alloc_t.allocate(alloc, new_size);

    auto new_free = std::uninitialized_copy_n(start, first_free - start, new_start);
    //auto new_free = std::uninitialized_copy(start, first_free, new_start);

    auto new_cap = new_start + new_size;

    // delete old data
    free();

    // have object point to new memory
    start = new_start;
    first_free = new_free;
    cap = new_cap;
}

template <typename T>
auto swap(Jarray<T>& lhs, Jarray<T>& rhs) -> void {
    using std::swap;
    std::cout << "SWAP!\n";
    swap(lhs.start, rhs.start);
    swap(lhs.first_free, rhs.first_free);
    swap(lhs.cap, rhs.cap);
}

template <typename T>
auto Jarray<T>::free() -> void {
    // delete old data
    while (first_free != start) {
        alloc_t.destroy(alloc, --first_free);
    }

    alloc_t.deallocate(alloc, start, capacity());
}

template <typename T>
Jarray<T>::~Jarray() {
    free();
    start = nullptr;
    first_free = nullptr;
    cap = nullptr;
}

template <typename T>
class Jarray<T>::Iterator {
public:
    // Assign iterator property by tagging it
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    // All iterators must be constructible,
    // copy-constructible, copy-assignable, destructible and swappable.

    explicit Iterator(pointer ptr): m_ptr(ptr) { }

    auto operator*() const -> reference {
        return *m_ptr;
    }

    auto operator->() const -> pointer {
        return m_ptr;
    }

    auto operator[](int n) -> reference {
        return *(m_ptr + n);
    }

    auto operator+=(int n) -> Iterator& {
        m_ptr = m_ptr + n;
        return *this;
    }

    auto operator-=(int n) -> Iterator& {
        m_ptr = m_ptr - n;
        return *this;
    }    


    auto operator++() -> Iterator& {
        m_ptr++;
        return *this;
    }

    auto operator++(int) -> Iterator {
        auto _copy = *this;
        m_ptr++;
        return _copy;
    }

    auto operator--() -> Iterator& {
        m_ptr--;
        return *this;
    }

    auto operator--(int) -> Iterator {
        auto _copy = *this;
        m_ptr--;
        return _copy;
    }

    friend auto operator<(const Iterator& lhs, const Iterator& rhs)  -> bool {
        return lhs.m_ptr < rhs.m_ptr;
    }

    friend auto operator>=(const Iterator& lhs, const Iterator& rhs)  -> bool {
        return !(lhs.m_ptr < rhs.m_ptr);
    }

    friend auto operator>(const Iterator& lhs, const Iterator& rhs)  -> bool {
        return  lhs.m_ptr > rhs.m_ptr;
    }

    friend auto operator<=(const Iterator& lhs, const Iterator& rhs)  -> bool {
        return !(lhs.m_ptr > rhs.m_ptr);
    }

    friend auto operator== (const Iterator& lhs, const Iterator& rhs) -> bool {
         return lhs.m_ptr == rhs.m_ptr;
    }
    friend auto operator!= (const Iterator& lhs, const Iterator& rhs) -> bool {
         return !(lhs.m_ptr == rhs.m_ptr);
    }

    friend auto operator-(const Iterator& lhs, const Iterator& rhs) -> difference_type {
        return lhs.m_ptr - rhs.m_ptr;
    }


    // Ordinarily, we define the arithmetic and relational operators
    // as nonmember functions in order to allow conversions for
    // either the left- or right-hand operand

    // Classes that define both an arithmetic operator and the related compound
    // assignment ordinarily ought to implement the arithmetic operator by using
    // the compound assignment.

    auto operator-(const int& a) const -> Iterator {
        Iterator temp(m_ptr);
        temp -= a;
        return temp;
    }

    auto operator+(const int& a) -> Iterator {
        Iterator temp(m_ptr);
        temp += a;
        return temp;
    }

private:
    pointer m_ptr;
};