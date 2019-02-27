#include <cstddef>

class RingIndex {
public:
	RingIndex(size_t index, size_t size):
		mIndex(index),
		mSize(size)
		{}
	void previous() {
		mIndex = (mIndex - 1) % mSize;
	}
	void next() {
		mIndex = (mIndex + 1) % mSize;
	}
	size_t size() {
		return mSize;
	}
	RingIndex operator=(size_t newIndex) {
		mIndex = newIndex % mSize;
	}
	operator size_t() const {
		return mIndex;
	}
	friend bool operator==(RingIndex, size_t);
	friend bool operator==(size_t, RingIndex);
	friend bool operator==(RingIndex, RingIndex);
	friend RingIndex operator+(RingIndex, size_t);
	friend RingIndex operator-(RingIndex, size_t);
private:
	size_t mIndex = 0;
	size_t mSize = 0;
};