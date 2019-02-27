#include "Game.hpp"
#include <list>
#include <limits>
#include <iomanip>

// ä = \x84 \b
// ö = \x94 \b
// ü = \x81 \b
// ß = \xE1 \b

using std::cin;
using std::cout;
using std::setw;
using std::left;
using std::right;
using std::string;
using std::array;
using std::vector;
using std::pair;
using std::list;

Game::Game():
	mDeck({"Herz", "Karo", "Pik", "Kreuz"}, {{"Ass", 11}, {"Zehner", 10}, {"K\x94 \bnig", 4}, {"Dame", 3}, {"Unter", 2}, {"Neuner", 0}}, std::make_pair(3, 4)) {
	outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(outHandle, &initialInfo);
}

void Game::run() {
	initPlayers(inputNames());
	for (size_t i = 0; i < mCardsPerHand; ++i) {
		drawCards();
	}
	roundsLoop();
}

void Game::clearLines(size_t lines, size_t chars) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(outHandle, &info);
	const auto currentPosition = info.dwCursorPosition;
	const auto initialPosition = initialInfo.dwCursorPosition;

	COORD newPosition = {0, 0};
	newPosition.Y = currentPosition.Y - lines;

	SetConsoleCursorPosition(outHandle, newPosition);
	for (size_t i = 0; i < lines; ++i) {
		cout << string(chars, ' ') << '\n';
	}
	SetConsoleCursorPosition(outHandle, newPosition);
}

vector<string> Game::inputNames() {
	vector<string> result;
	size_t lines = 0;
	for (size_t i = 0; i < NUMBER_OF_PLAYERS; ++i) {
		cout << "Name f\x81 \br Spieler " << i + 1 << ": ";
		string name;
		cin >> name;
		result.emplace_back(name);
		++lines;
	}
	clearLines(lines);
	return result;
}

void Game::initPlayers(vector<string> names) {
	for (const auto& n : names) {
		mPlayers.emplace_back(Player(n));
	}
}

void Game::drawCards() {
	if (mDeck.empty()) {
		cout << "Deck empty\t";
		return;
	}
	for (auto& p : mPlayers) {
		p.hand().emplace_back(mDeck.draw());
	}
}

void Game::confirmPrivacy() {
	size_t lines = 0;
	cout << "Dr\x81 \bcke ENTER um die Hand von " << mPlayers[mActivePlayerIndex].name() << " anzuzeigen\n";
	cin.sync();
	cin.ignore();
	lines += 2;
	clearLines(lines);
}

vector<pair<size_t, Card>> Game::checkMarriages(size_t playerIndex) {
	const auto marriageValues = mDeck.marriageValues();
	auto& currentPlayer = mPlayers[playerIndex];
	currentPlayer.pairs().clear();
	list<size_t> queenIndices;
	list<size_t> kingIndices;

	size_t i = 0;
	for (const auto& c : currentPlayer.hand()) {
		size_t value = c.value();
		if (value == marriageValues.first) {
			queenIndices.push_back(i);
		} else if (value == marriageValues.second) {
			kingIndices.push_back(i);
		}
		++i;
	}

	vector<pair<size_t, Card>> result;
	for (auto queenIndex : queenIndices) {
		const string color = currentPlayer.hand()[queenIndex].color();
		for (auto kingIndex : kingIndices) {
			if (color == currentPlayer.hand()[kingIndex].color()) {
				size_t value;
				string kingName;
				string queenName;
				if (color == mDeck.trump()) {
					value = 40;
					kingName = "Vierziger - K\x94 \bnig";
					queenName = "Vierziger - Dame";
				} else {
					value = 20;
					kingName = "Zwanziger - K\x94 \bnig";
					queenName = "Zwanziger - Dame";
				}
				result.emplace_back(std::make_pair(queenIndex, Card(color, queenName, value)));
				result.emplace_back(std::make_pair(kingIndex, Card(color, kingName, value)));
			}
		}
	}
	return result;
}

void Game::pickAggressingCard() {
	size_t lines = 0;
	size_t i = 0;
	cout << "Trumpfkarte: " << mDeck.trumpcard() << '\n';
	cout << "Deine Punkte: " << mPlayers[mActivePlayerIndex].score << '\n';
	lines += 2;
	auto& hand = mPlayers[mActivePlayerIndex].hand();
	for (const auto& c : hand) {
		cout << "[" << i << "] " << c << '\n';
		++i;
	}
	const auto marriages = checkMarriages(mActivePlayerIndex);
	for (const auto& m : marriages) {
		Card c = m.second;
		cout << "[" << i << "] " << c << '\n';
		++i;
	}
	lines += i;

	vector<bool> validIndices;
	for (const auto& c : hand) {
		bool fittingColor = (c.color() == mDeck.trump());
		validIndices.emplace_back(fittingColor);
	}
	size_t cardIndex;
	cout << "W\x84 \bhle eine Karte\n";
	++lines;
	while (true) {
		cin >> cardIndex;
		++lines;
		const bool indexInRange = cardIndex < hand.size() + marriages.size();
		if (!indexInRange) {
			cout << "Ung\x81 \bltiger Index eingegeben!\n";
			cin >> cardIndex;
			lines += 2;
			continue;
		}
		break;
	}
	Card playedCard;
	if (cardIndex < hand.size()) {
		playedCard = hand.at(cardIndex);
		hand.erase(hand.begin() + cardIndex);
		mEvalCardsQueue.emplace_back(playedCard);
	} else /* hand.size() <= cardIndex < hand.size() + marriages.size() */ {
		cardIndex -= hand.size();
		size_t handIndex = marriages.at(cardIndex).first;
		playedCard = hand.at(handIndex);
		hand.erase(hand.begin() + handIndex);
		Card marriageCard = marriages.at(cardIndex).second;
		Card laidCard = {playedCard.color(), marriageCard.name(), playedCard.value()};
		mPlayers[mActivePlayerIndex].score += marriageCard.value();
		mEvalCardsQueue.emplace_back(laidCard);
	}
	
	clearLines(lines);
}

void Game::pickDefendingCard() {
	size_t lines = 0;
	size_t i = 0;
	cout << "Trumpfkarte: " << mDeck.trumpcard() << '\n';
	cout << "Deine Punkte: " << mPlayers[mActivePlayerIndex].score << '\n';
	lines += 2;
	auto& hand = mPlayers[mActivePlayerIndex].hand();
	for (const auto& c : hand) {
		cout << "[" << i << "] " << c << '\n';
		++i;
	}
	lines += i;

	vector<bool> validIndices;
	for (const auto& c : hand) {
		bool fittingColor = (c.color() == mDeck.trump());
		fittingColor 	 |= (c.color() == mEvalCardsQueue.front().color());
		validIndices.emplace_back(fittingColor);
	}
	size_t cardIndex;
	cout << "W\x84 \bhle eine Karte\n";
	++lines;
	while (true) {
		cin >> cardIndex;
		++lines;
		
		const bool atLeastOneCardPlayable = [validIndices] () -> bool {
			for (const auto valid : validIndices) {
				if (valid) return true;
			}
			return false;
		} ();
		if (!atLeastOneCardPlayable) {
			break;
		}
		const bool indexInRange = cardIndex < hand.size();
		if (!indexInRange) {
			cout << "Ung\x81 \bltiger Index eingegeben!\n";
			++lines;
			continue;
		}
		const bool cardPlayable = validIndices[cardIndex];
		if (!cardPlayable) {
			cout << "Passt nicht zur gelegten Karte!\n";
			++lines;
			continue;
		}
		break;
	}
	mEvalCardsQueue.emplace_back(Card(hand.at(cardIndex)));
	hand.erase(hand.begin() + cardIndex);
	clearLines(lines);
}

void Game::evaluateCards() {
	if (mEvalCardsQueue.empty()) {
		return;
	}
	const Card firstCard = mEvalCardsQueue.front();
	size_t bestIndex = mAggressingPlayerIndex;
	size_t bestValue = firstCard.value();
	bool isBestTrump = firstCard.color() == mDeck.trump();

	// Determine winning card
	RingIndex playerIndex = mAggressingPlayerIndex;
	for (const Card& c : mEvalCardsQueue) { 
		size_t curValue = c.value();
		bool curTrump = c.color() == mDeck.trump();

		if (((curValue > bestValue) && (isBestTrump == curTrump))
				|| (!isBestTrump && curTrump)) {
			bestIndex = playerIndex;
			bestValue = curValue;
			isBestTrump = curTrump;
		}
		playerIndex.next();
	}
	size_t totalValue = 0;
	while (!mEvalCardsQueue.empty()) {
		mPlayers[bestIndex].wonCards().emplace_back(mEvalCardsQueue.back());
		totalValue += mEvalCardsQueue.back().value();
		mEvalCardsQueue.pop_back();
	}
	mPlayers[bestIndex].score += totalValue;
	mLastExchangeWonIndex = bestIndex;
	
	size_t lines = 0;
	cout
		<< mPlayers[mLastExchangeWonIndex].name()
		<< " sticht. ENTER zum Fortfahren\n";
	cin.sync();
	cin.ignore();
	lines += 2;
	clearLines(lines);
}

bool Game::checkScores() {
	if (mPlayers[mLastExchangeWonIndex].score >= 66) {
		distributePoints(mLastExchangeWonIndex);
		return true;
	}
	const bool allHandsEmpty = [&] () {
		for (auto& p : mPlayers) {
			if (!p.hand().empty()) {
				return false;
			}
		}
		return true;
	} ();
	if (allHandsEmpty) {
		distributePoints(mLastExchangeWonIndex);
		return true;
	} else {
		return false;
	}
}

void Game::distributePoints(size_t winnerIndex) {
	size_t points = 4;
	size_t i = 0;
	for (auto& p : mPlayers) {
		if (i == winnerIndex) {
			continue;
		}
		if (p.score >= 33) {
			points = 2;
		} else if (p.score == 0) {
			points = 6;
		}
		++i;
	}
	mPlayers[winnerIndex].points += points;
}

void Game::showPointsHeader() const {
	cout
		<< left
		<< setw(10)
		<< "Punkte:";
	
	for (const auto& p : mPlayers) {
		string name = p.name();
		size_t width = std::max(name.size(), static_cast<size_t>(10));
		cout 
			<< right
			<< setw(width)
			<< name;
	}
	cout << '\n';
}

void Game::showPoints() const {
	cout
		<< left
		<< setw(10)
		<< " ";
	
	for (const auto& p : mPlayers) {
		size_t points = p.points;
		cout 
			<< right
			<< setw(10)
			<< points;
	}
	cout << '\n';
}

void Game::roundsLoop() {
	mAggressingPlayerIndex = mActivePlayerIndex = mLastExchangeWonIndex;
	size_t lines = 0;
	for (size_t i = 0; i < mActivePlayerIndex.size(); ++i) {
		confirmPrivacy();
		if (mActivePlayerIndex == mAggressingPlayerIndex) {
			pickAggressingCard();
			if (checkScores()) {
				mEvalCardsQueue.clear();
				break;
			}
		} else {
			pickDefendingCard();
		}
		cout
			<< mPlayers[mActivePlayerIndex].name()
			<<" hat "
			<< mEvalCardsQueue[i]
			<< " gespielt\n";
		++lines;
		mActivePlayerIndex.next();
	}
	evaluateCards();
	cout << mPlayers[mLastExchangeWonIndex].name() << " hat gestochen\n";
	++lines;
	clearLines(lines);

	if (checkScores()) {
		if (0 == mNumberOfMatches) {
			showPointsHeader();
		}
		++mNumberOfMatches;
		showPoints();
		queryNewGame();
	} else {
		drawCards();
		roundsLoop();
	}
}

void Game::queryNewGame() {
	size_t lines = 0;
	cout << "Neues Spiel? [J/N]\n";
	++lines;
	while(true) {
		string in;
		cin >> in;
		++lines;
		if (in == "J" || in == "j" || in == "Ja" || in == "ja") {
			clearLines(lines);
			reset();
			roundsLoop();
			break;
		} else if (in == "N" || in == "n" || in == "Nein" || in == "nein") {
			exit(0);
		} else {
			cout << "Ung\x81 \bltige Eingabe!\n";
			++lines;
		}
	}
}

void Game::reset() {
	mBeginningPlayerIndex.next();
	mLastExchangeWonIndex = mBeginningPlayerIndex;
	for (auto& p : mPlayers) {
		p.score = 0;
		p.hand().clear();
		p.wonCards().clear();
	}
	mDeck.shuffle();
	for (size_t i = 0; i < mCardsPerHand; ++i) {
		drawCards();
	}
}