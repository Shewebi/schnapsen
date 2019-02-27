#include "Card.hpp"
#include <vector>
#include <string>

class Player {
public:
	Player(std::string name):
    	mName(name)
		{}
	
	std::string name() const { return mName; }
	std::vector<Card>& hand() { return mHand; }
	std::vector<Card>& pairs() { return mPairs; }
	std::vector<Card>& wonCards() { return mWonCards; }
	size_t score = 0;
	size_t points = 0;
private:
	const std::string mName;
	std::vector<Card> mHand;
	std::vector<Card> mPairs;
	std::vector<Card> mWonCards;
};