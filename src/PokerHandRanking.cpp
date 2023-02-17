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

  // overriding < operator to allow for sort() function calls
  bool operator<(const card &card) const { return (cardValues[value] < cardValues[card.value]); }
  // overriding > as well for good measure
  bool operator>(const card &card) const { return (cardValues[value] > cardValues[card.value]); }
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

vector<card> Hand::getCards(){
    return cards;
}

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
    if (cardValues[cards[i + 1].value] - cardValues[cards[i].value] != 1) {
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
  void breakTie(HandRanking tiedRank);  
  void highCardTieBreaker();
  void onePairTieBreaker();
  void twoPairsTieBreaker();
  void threeOfAKindTieBreaker();
  void straightTieBreaker();
  void flushTieBreaker();
  void fullHouseTieBreaker();
  void fourOfAKindTieBreaker();  
  void straightFlushTieBreaker();
  void royalFlushTieBreaker();

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
    breakTie(tiedRank);
  }
}

void PokerGame::breakTie(HandRanking tiedRank) {
  switch (tiedRank)
    {
    case HandRanking::HighCard:
      highCardTieBreaker();
      break;
    case HandRanking::OnePair:
      onePairTieBreaker();
      break;
    case HandRanking::TwoPairs:
      twoPairsTieBreaker();
      break;
    case HandRanking::ThreeOfAKind:
      threeOfAKindTieBreaker();
      break;
    case HandRanking::Straight:
      straightTieBreaker();
      break;
    case HandRanking::Flush:
      flushTieBreaker();
      break;
    case HandRanking::FullHouse:
      fullHouseTieBreaker();
      break;
    case HandRanking::FourOfAKind:
      fourOfAKindTieBreaker();
      break;
    case HandRanking::StraightFlush:
      straightFlushTieBreaker();
      break;
    case HandRanking::RoyalFlush:
      royalFlushTieBreaker();
      break;
    default:
      //This should not happen
      std::cout << "Invalid hand rank for tie breaking!";
      break;
    }
}

/// @brief Break Ties between two hands of rank 'High Card'
///
/// This will check both hands which are sorted in ascending order. 
/// By iterating from the end toward the beginning, we must find the first 
/// instance where the cards do not match and the hand with the higher card 
/// is the winner.
void PokerGame::highCardTieBreaker(){
    vector<card> player1 = playerHands[0].getCards();
    vector<card> player2 = playerHands[1].getCards();
    for(int i = handSize-1; i >= 0; i--){
        if(player1[i].value == player2[i].value){
            continue;
        }
        else if(player1[i].value > player2[i].value){
            std::cout << "Player 1 wins the tie breaker.";
            break;
        }
        else{
            std::cout << "Player 2 wins the tie breaker.";
            break;
        }
        if(i == 0){
            std::cout << "It's a tie! The hands are of equal rank.";
        }
    }
    
}

void PokerGame::onePairTieBreaker(){

}

void PokerGame::twoPairsTieBreaker(){

}

void PokerGame::threeOfAKindTieBreaker(){

}

void PokerGame::straightTieBreaker(){

}

void PokerGame::flushTieBreaker(){

}

void PokerGame::fullHouseTieBreaker(){

}

void PokerGame::fourOfAKindTieBreaker(){

}

void PokerGame::straightFlushTieBreaker(){

}

void PokerGame::royalFlushTieBreaker(){

}

int main() {
  // Deck d;
  // Hand h;
  // h.addCard(d.dealTopCard());
  // h.addCard(d.dealTopCard());
  // h.addCard(d.dealTopCard());
  // h.addCard(d.dealTopCard());
  // h.addCard(d.dealTopCard());

  // h.showHand();
  // h.setRank();

  // cout << h.getRank();
  PokerGame game;
  game.addPlayerHand();
  game.addPlayerHand();
  game.showPlayerHands();
  game.determineHandWinner();

  return 0;
}
