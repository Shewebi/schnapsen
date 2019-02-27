//#pragma once
#include "Card.hpp"

#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <random>

typedef std::pair<std::string, size_t> NameValuePair;

class Deck {
public:
	Deck() {}
	Deck(std::vector<std::string> colors, std::vector<NameValuePair> nameValues, std::pair<size_t, size_t> marriageValues);
	Card draw();
	Card trumpcard() const;
	std::string trump() const;
	bool empty() const;
	void shuffle();
	std::pair<size_t, size_t> marriageValues() const {
		return mMarriageValues;
	}
	friend std::ostream& operator<<(std::ostream& os, const Deck deck);

private:
	size_t seedRNG();
	std::vector<Card> initDefaultDeck(std::vector<std::string> colors, std::vector<NameValuePair> nameValues) const;
	std::ostream& printDeck(std::ostream& os, const std::vector<Card>& deck) const;

	std::vector<Card> mActiveDeck;
	std::vector<Card> mDefaultDeck;
	std::mt19937_64 mRNG;
	const std::pair<size_t, size_t> mMarriageValues;
};
