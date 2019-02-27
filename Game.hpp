#include "Player.hpp"
#include "Deck.hpp"
#include "RingIndex.hpp"
#define NOMINMAX 1
#include <windows.h>
#include <tuple>
#include <vector>
#include <string>
#include <iostream>
static constexpr size_t NUMBER_OF_PLAYERS = 2;

class Game {
public:
	Game();
	void run();
	
private:
	void clearLines(size_t lines, size_t chars = 80);
	std::vector<std::string> inputNames();
	void initPlayers(std::vector<std::string> names);
	void drawCards();
	void confirmPrivacy();
	std::vector<std::pair<size_t, Card>> checkMarriages(size_t playerIndex);
	void pickAggressingCard();
	void pickDefendingCard();
	void evaluateCards();
	bool checkScores();
	void distributePoints(size_t winnerIndex);
	void showPointsHeader() const;
	void showPoints() const;
	void roundsLoop();
	void queryNewGame();
	void reset();

	std::vector<Player> mPlayers;
	std::vector<Card> mDesk;
	RingIndex mAggressingPlayerIndex   = {0, NUMBER_OF_PLAYERS};
	RingIndex mActivePlayerIndex       = {0, NUMBER_OF_PLAYERS};
	RingIndex mLastExchangeWonIndex    = {0, NUMBER_OF_PLAYERS};
	RingIndex mBeginningPlayerIndex    = {0, NUMBER_OF_PLAYERS};
	std::vector<Card> mEvalCardsQueue;
	Deck mDeck;
	size_t mNumberOfMatches = 0;
	size_t mCardsPerHand = 4;

	HANDLE outHandle;
	CONSOLE_SCREEN_BUFFER_INFO initialInfo;
};

/**
 * 1. Input player names
 * 2. Pick random player to start
 * 
 * ## GAMELOOP ##
 * while (!deck.empty()) {
 * 	3. Confirm that hand of aggressing player can be displayed
 * 	4. Display hand of aggressing player
 * 	5. Player selects card to play by inputting the corresponding index
 * 
 * 	6. Confirm that hand of defending player can be displayed
 * 	7. Display hand of defending player
 * 	8. Player selects card to defend with by inputting the corresponding index
 * 
 * 	9. Evaluate played cards
 * 	10. Player who won the exchange gets points
 * 	11. Evaluate both players' points, if one player is >= 66, they win
 * }
 * 
 * 12. Either play again or exit
 */