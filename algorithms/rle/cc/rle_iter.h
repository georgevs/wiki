#pragma once

#include <iterator>
#include <utility>  // pair

using namespace std;

namespace rle_iter {

template <forward_iterator InIter,
            output_iterator<pair<int, iter_value_t<InIter>>> OutIter>
OutIter rle(InIter first, InIter last, OutIter dest) {
    if (first != last) {
        auto x = *first;
        int count = 1;
        for (++first; first != last; ++first) {
            auto y = *first;
            if (x == y) {
                ++count;
            } else {
                *dest = pair{count, x};
                x = move(y);
                count = 1;
            }
        }
        *dest = pair{count, x};
    }
    return dest;
}

} // namespace
