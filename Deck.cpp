#include "Deck.hpp"
#include <algorithm>
#include <assert.h>
#include <chrono>

using std::vector;
using std::string;
using std::ostream;
using std::pair;

Deck::Deck(vector<string> colors, vector<NameValuePair> nameValues, pair<size_t, size_t> marriageValues):
	mDefaultDeck(initDefaultDeck(colors, nameValues)),
	mActiveDeck(),
	mRNG(seedRNG()),
	mMarriageValues(marriageValues) {
		assert(mDefaultDeck.size() % 2 == 0);
		shuffle();
	}

void Deck::shuffle() {
	mActiveDeck = mDefaultDeck;
    std::shuffle(mActiveDeck.begin(), mActiveDeck.end(), mRNG);
}

Card Deck::draw() {
	if (mActiveDeck.empty()) {
		std::cerr << "Deck alle :(" ;
		return Card("Error", "EroroRORroR", -1);
	}
	const Card result = mActiveDeck.back();
	mActiveDeck.pop_back();
	return result;
}

Card Deck::trumpcard() const {
	return mActiveDeck.front();
}

string Deck::trump() const {
	return mActiveDeck.front().color();
}

bool Deck::empty() const {
	return mActiveDeck.empty();
}

size_t Deck::seedRNG() {
	auto now = std::chrono::steady_clock::now();
	return now.time_since_epoch().count();
}

vector<Card> Deck::initDefaultDeck(vector<string> colors, vector<NameValuePair> nameValues) const {
	vector<Card> result;

	for (const auto c : colors) {
		for (auto n : nameValues) {
			result.emplace_back(Card(c, n.first, n.second));
		}
	}
	return result;
}

ostream& Deck::printDeck(ostream& os, const vector<Card>& deck) const {
	for (const auto card : deck) {
		card.printWithValue(os);
		os << '\n';
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Deck deck) {
	return deck.printDeck(os, deck.mActiveDeck);
}