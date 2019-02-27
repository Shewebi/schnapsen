#include "RingIndex.hpp"

RingIndex operator+(RingIndex index, size_t x) {
    index.mIndex = (index.mIndex + x) % index.mSize;
}
RingIndex operator-(RingIndex index, size_t x) {
    index.mIndex = (index.mIndex - x) % index.mSize;
}
bool operator==(RingIndex index, size_t x) {
    return index.mIndex == x;
}
bool operator==(size_t x, RingIndex index) {
    return index == x;
}
bool operator==(RingIndex lhs, RingIndex rhs) {
    return lhs.mIndex == rhs.mIndex && lhs.mSize == rhs.mSize;
}