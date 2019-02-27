#pragma once
#include <string>
#include <iostream>

class Card {
public:
	Card() {}
	Card(std::string color, std::string name, size_t value):
		mColor(color),
		mName(name),
		mValue(value) {}
		
	Card(const Card& other):
		mColor(other.mColor),
		mName(other.mName),
		mValue(other.mValue)
		{}

	friend std::ostream& operator<<(std::ostream& os, const Card card);
	void printWithValue(std::ostream& os) const {
		os << mColor << " " << mName << ": " << mValue;
	}

	std::string color() const { return mColor; }
	std::string name() const { return mName; }
	size_t value() const { return mValue; }

private:
	std::string mColor;
	std::string mName;
	size_t mValue;
};

inline std::ostream& operator<<(std::ostream& os, const Card card) {
	card.printWithValue(os);
	return os;
}