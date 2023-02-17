#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <vector>

using namespace std;

// TODO: wouldn't compile with map set as a const after I started indexing in
// the map to try to rank the hand. Not sure what I was doing wrong but I feel
// like const makes sense for this but don't want to check in a build that won't
// compile so putting this TODO here.
map<char, int> cardValues = {
    {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};

const char Suits[4] = {'S', 'H', 'D', 'C'};

const char Values[13] = {'2', '3', '4', '5', '6', '7', '8',
                         '9', 'T', 'J', 'Q', 'K', 'A'};

struct card {
  char value;
  char suit;
  int rank;

  // overriding < operator to allow for sort() function calls
  bool operator<(const card &card) const { return (rank < card.rank); }
  // overridng comparison to ints for easier comparisons in tie breakers
  bool operator<(int compareValue) const { return (rank < compareValue); }
  // overriding > as well for easier comparison in tie breakers
  bool operator>(const card &card) const { return (rank > card.rank); }
  bool operator>(int compareValue) const { return (rank > compareValue); }
  // for simplifying comparing if cards have equal value
  bool operator==(const card &card) const { return (rank == card.rank); }
  bool operator==(int compareValue) const { return (rank == compareValue); }
};

class Deck {
public:
  Deck();
  ~Deck();
  card dealTopCard();

private:
  deque<card> deck;
};

Deck::Deck() {
  vector<card> temp;
  for (char s : Suits) {
    for (char v : Values) {
      card c;
      c.suit = s;
      c.value = v;
      c.rank = cardValues[c.value];
      deck.push_back(c);
    }
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(deck.begin(), deck.end(), g);
}

Deck::~Deck() {}

card Deck::dealTopCard() {
  card c = deck.front();
  deck.pop_front();
  return c;
}

// Enum to store the possible hand rankings
enum HandRanking {
  HighCard,
  OnePair,
  TwoPairs,
  ThreeOfAKind,
  Straight,
  Flush,
  FullHouse,
  FourOfAKind,
  StraightFlush,
  RoyalFlush
};

// overriding the << operator so that we can output the HandRanking enum text
// instead of the int value
std::ostream &operator<<(std::ostream &os, const HandRanking rank) {
  switch (rank) {
  case HandRanking::HighCard:
    os << "High Card";
    break;
  case HandRanking::OnePair:
    os << "One Pair";
    break;
  case HandRanking::TwoPairs:
    os << "Two Pairs";
    break;
  case HandRanking::ThreeOfAKind:
    os << "Three Of a Kind";
    break;
  case HandRanking::Straight:
    os << "Straight";
    break;
  case HandRanking::Flush:
    os << "Flush";
    break;
  case HandRanking::FullHouse:
    os << "Full House";
    break;
  case HandRanking::FourOfAKind:
    os << "Four Of a Kind";
    break;
  case HandRanking::StraightFlush:
    os << "Straight Flush";
    break;
  case HandRanking::RoyalFlush:
    os << "Royal Flush";
    break;
  default:
    os << "Invalid option.";
    break;
  }
  return os;
}

class Hand {
public:
  void addCard(card c);
  void showHand();
  bool isHandFull();
  void discardHand();
  void sortHand();
  HandRanking getRank();
  void setRank();
  vector<card> getCards();

private:
  vector<card> cards;
  HandRanking rank;

  bool isRoyalFlush();
  bool isStraightFlush();
  bool isFourOfAKind();
  bool isFullHouse();
  bool isFlush();
  bool isStraight();
  bool isThreeOfAKind();
  bool isTwoPairs();
  bool isOnePair();

protected:
};

void Hand::discardHand() { cards.clear(); }

void Hand::sortHand() { sort(cards.begin(), cards.end()); }

void Hand::addCard(card c) { cards.push_back(c); }

void Hand::showHand() {
  for (card c : cards) {
    std::cout << c.value << c.suit << " ";
  }
  std::cout << std::endl;
}

bool Hand::isHandFull() {
  if (cards.size() >= 5) {
    return true;
  } else {
    return false;
  }
}

HandRanking Hand::getRank() { return rank; }

void Hand::setRank() {
  if (isRoyalFlush()) {
    rank = HandRanking::RoyalFlush;
  } else if (isStraightFlush()) {
    rank = HandRanking::StraightFlush;
  } else if (isFourOfAKind()) {
    rank = HandRanking::FourOfAKind;
  } else if (isFullHouse()) {
    rank = HandRanking::FullHouse;
  } else if (isStraight()) {
    rank = HandRanking::Straight;
  } else if (isThreeOfAKind()) {
    rank = HandRanking::ThreeOfAKind;
  } else if (isTwoPairs()) {
    rank = HandRanking::TwoPairs;
  } else if (isOnePair()) {
    rank = HandRanking::OnePair;
  } else {
    rank = HandRanking::HighCard;
  }
}

vector<card> Hand::getCards() { return cards; }

bool Hand::isRoyalFlush() {
  if (isFlush() && isStraight() && cards[4].value == 'A') {
    return true;
  }
  return false;
}

bool Hand::isStraightFlush() {
  if (isFlush() && isStraight()) {
    return true;
  }
  return false;
}

bool Hand::isFourOfAKind() {
  // check all sets of four consecutive cards for four of a kind
  for (int i = 0; i <= cards.size() - 4; i++) {
    if (cards[i].value == cards[i + 1].value &&
        cards[i].value == cards[i + 2].value &&
        cards[i].value == cards[i + 3].value) {
      return true;
    }
  }
  return false;
}

bool Hand::isFullHouse() {
  // checks for sorted hand with three of a kind then a pair
  if (cards[0].value == cards[1].value && cards[1].value == cards[2].value &&
      cards[3].value == cards[4].value) {
    return true;
  }
  // checks for sorted hand with a pair then three of a kind
  if (cards[0].value == cards[1].value && cards[2].value == cards[3].value &&
      cards[3].value == cards[4].value) {
    return true;
  }
  return false;
}

bool Hand::isFlush() {
  if (cards[0].suit == cards[1].suit && cards[0].suit == cards[2].suit &&
      cards[0].suit == cards[3].suit && cards[0].suit == cards[4].suit) {
    return true;
  }
  return false;
}

bool Hand::isStraight() {
  for (int i = 0; i < cards.size() - 1; i++) {
    // doing subtraction so lookup the value in the map since you can't
    // subtract 7 from 'A'
    if (cards[i + 1].rank - cards[i].rank != 1) {
      return false;
    }
  }
  return true;
}

bool Hand::isThreeOfAKind() {
  // check all sets of 3 consecutive cards for three of a kind
  for (int i = 0; i <= cards.size() - 3; i++) {
    if (cards[i].value == cards[i + 1].value &&
        cards[i].value == cards[i + 2].value) {
      return true;
    }
  }
  return false;
}

bool Hand::isTwoPairs() {
  int numberOfPairs = 0;
  for (int i = 0; i < cards.size() - 1; i++) {
    if (cards[i].value == cards[i + 1].value) {
      numberOfPairs++;
      i++; // already found pair for this index so skip ahead by one
    }
  }
  return numberOfPairs == 2;
}

bool Hand::isOnePair() {
  int numberOfPairs = 0;
  for (int i = 0; i < cards.size() - 1; i++) {
    if (cards[i].value == cards[i + 1].value) {
      numberOfPairs++;
      i++; // already found pair for this index so skip ahead by one
    }
  }
  return numberOfPairs == 1;
}

// Stubbing out some method names that a poker game might need
// TODO: Use this class to run a game of poker with two players...?
class PokerGame {
public:
  vector<Hand> playerHands;
  void addPlayerHand();
  void showPlayerHands();
  void determineHandWinner();

private:
  Deck deck;
  int numberOfPlayers = 0;
  int handSize = 5;
  void breakTie(HandRanking tiedRank, vector<card> player1,
                vector<card> player2);
  int highCardTieBreaker(vector<card> player1, vector<card> player2);
  int onePairTieBreaker(vector<card> player1, vector<card> player2);
  int twoPairsTieBreaker(vector<card> player1, vector<card> player2);
  int threeOfAKindTieBreaker(vector<card> player1, vector<card> player2);
  int straightTieBreaker(vector<card> player1, vector<card> player2);
  int flushTieBreaker(vector<card> player1, vector<card> player2);
  int fullHouseTieBreaker(vector<card> player1, vector<card> player2);
  int fourOfAKindTieBreaker(vector<card> player1, vector<card> player2);
  int straightFlushTieBreaker(vector<card> player1, vector<card> player2);
  int royalFlushTieBreaker(vector<card> player1, vector<card> player2);

protected:
};

void PokerGame::addPlayerHand() {
  Hand hand;
  for (int i = 0; i < handSize; i++) {
    hand.addCard(deck.dealTopCard());
  }
  hand.sortHand();
  playerHands.push_back(hand);
  numberOfPlayers++;
}

void PokerGame::showPlayerHands() {
  for (int i = 0; i < numberOfPlayers; i++) {
    std::cout << "Showing hand for player " << (i + 1) << "\n";
    playerHands[i].showHand();
  }
}

void PokerGame::determineHandWinner() {
  for (int i = 0; i < numberOfPlayers; i++) {
    playerHands[i].setRank();
    cout << "Player " << (i + 1)
         << "has hand of type: " << playerHands[i].getRank();
    std::cout << std::endl;
  }
  // TODO: Generalize determining a winner to a process that supports more
  // than 2 players
  if ((int)playerHands[0].getRank() > (int)playerHands[1].getRank()) {
    cout << "Player 1 is the winner!";
    std::cout << std::endl;
  } else if ((int)playerHands[0].getRank() < (int)playerHands[1].getRank()) {
    cout << "Player 2 is the winner!";
    std::cout << std::endl;
  } else {
    cout << "Both players have a hand of the same rank, checking tie-breakers.";
    std::cout << std::endl;
    HandRanking tiedRank = playerHands[0].getRank();
    vector<card> player1 = playerHands[0].getCards();
    vector<card> player2 = playerHands[1].getCards();
    breakTie(tiedRank, player1, player2);
  }
}

void PokerGame::breakTie(HandRanking tiedRank, vector<card> player1,
                         vector<card> player2) {
  int winner = -1;
  switch (tiedRank) {
  case HandRanking::HighCard:
    winner = highCardTieBreaker(player1, player2);
    break;
  case HandRanking::OnePair:
    winner = onePairTieBreaker(player1, player2);
    break;
  case HandRanking::TwoPairs:
    winner = twoPairsTieBreaker(player1, player2);
    break;
  case HandRanking::ThreeOfAKind:
    winner = threeOfAKindTieBreaker(player1, player2);
    break;
  case HandRanking::Straight:
    winner = straightTieBreaker(player1, player2);
    break;
  case HandRanking::Flush:
    winner = flushTieBreaker(player1, player2);
    break;
  case HandRanking::FullHouse:
    winner = fullHouseTieBreaker(player1, player2);
    break;
  case HandRanking::FourOfAKind:
    winner = fourOfAKindTieBreaker(player1, player2);
    break;
  case HandRanking::StraightFlush:
    winner = straightFlushTieBreaker(player1, player2);
    break;
  case HandRanking::RoyalFlush:
    winner = royalFlushTieBreaker(player1, player2);
    break;
  default:
    // This should not happen
    std::cout << "Invalid hand rank for tie breaking!";
    break;
  }

  if (winner == 1) {
    std::cout << "Player 1 wins the tie breaker.";
  } else if (winner == 2) {
    std::cout << "Player 2 wins the tie breaker.";
  } else {
    std::cout << "It's a tie! The hands are of equal rank.";
  }
}

/// @brief Break Ties between two hands of rank 'High Card'
///
/// This will check both hands which are sorted in ascending order.
/// By iterating from the end toward the beginning, we must find the first
/// instance where the cards do not match and the hand with the higher card
/// is the winner.
int PokerGame::highCardTieBreaker(vector<card> player1, vector<card> player2) {
  for (int i = handSize - 1; i >= 0; i--) {
    if (player1[i] > player2[i]) {
      return 1;
    } else if (player1[i] < player2[i]) {
      return 2;
    }
    if (i == 0) {
      return -1;
    }
  }
}

/// @brief Break ties between two hands that contain one pair
///
/// The code will first see if one pair is higher than the other. If they are
/// the same pair value, high card rules will be used as backup tie breakers.
int PokerGame::onePairTieBreaker(vector<card> player1, vector<card> player2) {
  int pairValuePlayer1 = -1;
  int pairValuePlayer2 = -1;
  for (int i = 1; i < handSize; i++) {
    if (player1[i] == player1[i - 1]) {
      pairValuePlayer1 = player1[i].rank;
    }
    if (player2[i] == player2[i - 1]) {
      pairValuePlayer2 = player2[i].rank;
    }
  }
  if (pairValuePlayer1 > pairValuePlayer2) {
    return 1;
  } else if (pairValuePlayer1 < pairValuePlayer2) {
    return 2;
  } else {
    // the pairs are the same, resolve the tie by looking for best high-card
    for (int i = handSize - 1; i >= 0; i--) {
      if (player1[i] > player2[i]) {
        return 1;
      } else if (player1[i] < player2[i]) {
        return 2;
      }
    }
  }
  return -1;
}

int PokerGame::twoPairsTieBreaker(vector<card> player1, vector<card> player2) {
  // Find the highest pair and the highest non-paired card in each hand
  int player1Pair1 = -1;
  int player1Pair2 = -1;
  int player2Pair1 = -1;
  int player2Pair2 = -1;
  for (int i = 0; i < handSize - 1; i++) {
    if (player1[i] == player1[i + 1] && player1[i] > player1Pair1) {
      player1Pair2 = player1Pair1;
      player1Pair1 = player1[i].rank;
    } else if (player1[i] == player1[i + 1] && player1[i] > player1Pair2) {
      player1Pair2 = player1[i].rank;
    }
    if (player2[i] == player2[i + 1] && player2[i] > player2Pair1) {
      player2Pair2 = player2Pair1;
      player2Pair1 = player2[i].rank;
    } else if (player2[i] == player2[i + 1] && player2[i] > player2Pair2) {
      player2Pair2 = player2[i].rank;
    }
  }

  if (player1Pair1 > player2Pair1) {
    return 1;
  } else if (player2Pair1 > player1Pair1) {
    return 2;
  } else {
    // in this case the highest pair is equal
    if (player1Pair2 > player2Pair2) {
      return 1;
    } else if (player2Pair2 > player1Pair2) {
      return 2;
    } else {
      // both pairs are the same, resolve the tie by looking for best high-card
      for (int i = handSize - 1; i >= 0; i--) {
        if (player1[i] > player2[i]) {
          return 1;
        } else if (player1[i] < player2[i]) {
          return 2;
        }
      }
    }
  }

  return -1;
}

int PokerGame::threeOfAKindTieBreaker(vector<card> player1,
                                      vector<card> player2) {
  return -1;
}

int PokerGame::straightTieBreaker(vector<card> player1, vector<card> player2) {
  return -1;
}

int PokerGame::flushTieBreaker(vector<card> player1, vector<card> player2) {
  return -1;
}

int PokerGame::fullHouseTieBreaker(vector<card> player1, vector<card> player2) {
  return -1;
}

int PokerGame::fourOfAKindTieBreaker(vector<card> player1,
                                     vector<card> player2) {
  return -1;
}

int PokerGame::straightFlushTieBreaker(vector<card> player1,
                                       vector<card> player2) {
  return -1;
}

int PokerGame::royalFlushTieBreaker(vector<card> player1,
                                    vector<card> player2) {
  return -1;
}

int main() {
  PokerGame game;
  game.addPlayerHand();
  game.addPlayerHand();
  game.showPlayerHands();
  game.determineHandWinner();

  return 0;
}
