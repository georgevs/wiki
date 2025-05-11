#pragma once

#include <iterator>
#include <ranges>
#include <utility>  // pair

using namespace std;

namespace rle_view {

// To formally model a view (ie work with ranges::copy() and be composible)
// `rle` must also be default constructible, and inherit from view_interface
// `rle::iterator` must model input_iterator

template <forward_iterator InIter>
class rle : ranges::view_interface<rle<InIter>> {
public:
    // rle() = default;
    rle(InIter first, InIter last) : _first(first), _last(last) {}
    auto begin() { return iterator(_first, _last); }
    auto end() { return default_sentinel_t{}; }

    class iterator {
    public:
        using difference_type = ptrdiff_t;
        // using iterator_category = input_iterator_tag;
        // using iterator_concept = std::input_iterator_tag;
        using value_type = pair<int, iter_value_t<InIter>>;

        // iterator() = default;
        // iterator(const iterator&) = default;
        // iterator& operator = (const iterator&) = default;
        iterator(InIter first, InIter last) : _first(first), _last(last) {}

        friend bool operator == (default_sentinel_t s, const iterator& it) { return it == s; }
        // friend bool operator == (const iterator& it, default_sentinel_t s) { return it == s; }

        bool operator == (default_sentinel_t) { return _first == _last; }
        // bool operator != (default_sentinel_t s) { return !(*this == s); }

        iterator& operator++() {
            // assert(_first != _last);
            auto val = *_first;
            for (++_first; _first != _last && val == *_first; ++_first) {}
            return *this;
        }
        iterator operator++(int) {
            auto temp = *this;
            ++temp;
            return temp;
        }
        value_type operator*() const {
            // assert(_first != _last);
            auto first = _first;
            auto val = *first;
            int count = 1;
            for (++first; first != _last && val == *first; ++first) {
                ++count;
            }
            return {count, val};
        }
    private:
        InIter _first, _last;
    };
private:
    InIter _first, _last;
};

} // namespace rle_view
