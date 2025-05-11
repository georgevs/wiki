#pragma once

#include <iterator>
#include <utility>  // pair

using namespace std;

namespace rle_gen {

template <forward_iterator InIter>
class rle {
public:
    rle(InIter first, InIter last) : _first(first), _last(last) {}
    auto begin() { return iterator(_first, _last); }
    auto end() { return default_sentinel_t{}; }
    class iterator {
    public:
        iterator(InIter first, InIter last) : _first(first), _last(last) {}
        bool operator != (default_sentinel_t) { return _first != _last; }
        iterator& operator++() {
            // assert(_first != _last);
            auto val = *_first;
            for (++_first; _first != _last && val == *_first; ++_first) {}
            return *this;
        }
        auto operator*() {
            // assert(_first != _last);
            auto first = _first;
            auto val = *first;
            int count = 1;
            for (++first; first != _last && val == *first; ++first) {
                ++count;
            }
            return pair{count, val}; 
        }
    private:
        InIter _first, _last;
    };
private:
    InIter _first, _last;
};

} // namespace rle_gen
