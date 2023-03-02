#include "jsoncpp.cpp"
#include "json/json.h"
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <vector>

using namespace std;
using namespace Json;

struct Card {
  char value;
  char suit;
  int rank;
  static map<char, int> cardValues;
  static const char Suits[];
  static const char Values[];

  Card();
  Card(string cardCode);

  //override < > operators to allow for sort function calls to know how to sort
  bool operator<(const Card &card) const { return (rank < card.rank); }
  bool operator>(const Card &card) const { return (rank > card.rank); }  
  
  // overridng comparison to ints for easier comparisons in tie breakers
  bool operator<(int compareValue) const { return (rank < compareValue); }
  bool operator>(int compareValue) const { return (rank > compareValue); }

  // for simplifying comparing if cards have equal value
  bool operator==(const Card &card) const { return (rank == card.rank); }
  bool operator==(int compareValue) const { return (rank == compareValue); }
};

/// @brief parameterless constructor
Card::Card() {}

/// @brief constructs a card from a two character code
/// @param code
Card::Card(string code) {
  value = code.at(0);
  suit = code.at(1);
  rank = cardValues[value];
}

map<char, int> Card::cardValues = {
    {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};
const char Card::Suits[] = {'S', 'H', 'D', 'C'};
const char Card::Values[] = {'2', '3', '4', '5', '6', '7', '8',
                         '9', 'T', 'J', 'Q', 'K', 'A'};

/// @brief Class representing a deck of cards
///
/// A class that represents a deck of 52 standard playing cards. Has function
/// a function for dealing out cards. Deck is shuffled at creation time
class Deck {
public:
  Deck();
  ~Deck();
  Card dealTopCard();

private:
  deque<Card> deck;
};

// Deck constructor. Adds all of the cards to the deck and then shuffles them.
Deck::Deck() {
  vector<Card> temp;
  for (char s : Card::Suits) {
    for (char v : Card::Values) {
      Card c;
      c.suit = s;
      c.value = v;
      c.rank = Card::cardValues[c.value];
      deck.push_back(c);
    }
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(deck.begin(), deck.end(), g);
}

Deck::~Deck() {}

/// @brief Deals the top card from the deck
/// @return card
Card Deck::dealTopCard() {
  Card c = deck.front();
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

/// @brief Overriding the ostream << operator
/// Overiding this to make displaying Hand Rankings easier with cout
/// @param os
/// @param rank
/// @return
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

/// @brief Class to hold a poker hand
///
/// The Hand class holds a collection of Card objects in the vector 'cards.' The
/// functions support functionality for putting cards into the hand, sorting the
/// hand, and displaying the hand. There are also properties for ma
class Hand {
public:
  Hand(int maxHandSize);
  void addCard(Card c);
  void showHand() const;
  bool isHandFull();
  void discardHand();
  void sortHand();
  void setRank();
  HandRanking getRank() const;
  vector<Card> getCards() const;

  // array to store counts of card values in the hand
  int handCounts[13] = {0}; // 2 3 4 5 6 7 8 9 T J Q K A
  // array to store counts of suits within the hand
  int suitCounts[4] = {0}; // Club Diamond Heart Spade
private:
  vector<Card> cards;
  HandRanking rank;
  int maximumHandSize;

  // boolean flags for hand type determination
  bool pair = false;
  bool twoPair = false;
  bool threeAKind = false;
  bool straight = false;
  bool flush = false;
  bool fourAKind = false;
  bool straightFlush = false;
  bool royalFlush = false;
};

Hand::Hand(int maxSize = 5) { maximumHandSize = maxSize; }

/// @brief Discard the hand
///
/// Sets the hand and its properties back to the default, empty state
void Hand::discardHand() {
  // clear hand and reset all calculated values to 0 and false
  cards.clear();
  handCounts[13] = {0};
  suitCounts[4] = {0};
  pair = false;
  twoPair = false;
  threeAKind = false;
  straight = false;
  flush = false;
  fourAKind = false;
  straightFlush = false;
  royalFlush = false;
}

/// @brief Sort the hand ascending
void Hand::sortHand() { sort(cards.begin(), cards.end()); }

/// @brief Add a card to the cards vector
/// @param c The card to be added
void Hand::addCard(Card c) {
  if (!isHandFull()) {
    cards.push_back(c);
  }
}

/// @brief Displays the hand in the terminal
/// Display format example: Three of Hearts is represented as 3H
void Hand::showHand() const {
  for (Card c : cards) {
    std::cout << c.value << c.suit << " ";
  }
  std::cout << std::endl;
}

/// @brief Returns true if the cards vector has reached the maximum hand size
/// @return
bool Hand::isHandFull() {
  if (cards.size() >= maximumHandSize) {
    return true;
  } else {
    return false;
  }
}

/// @brief Returns the rank property
/// @return
HandRanking Hand::getRank() const { return rank; }

/// @brief Returns the cards vector
/// @return
vector<Card> Hand::getCards() const { return cards; }

/// @brief function to set the rank for the hand
///
/// Sets the rank (e.g. flush, two pair). Performs a single pass through the
/// cards vector to set the values in the handCounts and suitCounts arrays then
/// uses the values in those arrays to determine the hand rank
void Hand::setRank() {
  for (Card c : cards) {
    handCounts[c.rank - 2]++;
    switch (c.suit) {
    case 'C':
      suitCounts[0]++;
      break;
    case 'D':
      suitCounts[1]++;
      break;
    case 'H':
      suitCounts[2]++;
      break;
    case 'S':
      suitCounts[3]++;
      break;
    default:
      break;
    }
  }

  if (count(handCounts, handCounts + 13, 2) == 1) {
    pair = true;
  }
  if (count(handCounts, handCounts + 13, 2) == 2) {
    twoPair = true;
  }
  if (count(handCounts, handCounts + 13, 3) == 1) {
    threeAKind = true;
  }
  if (count(handCounts, handCounts + 13, 4) == 1) {
    fourAKind = true;
  }
  if (count(suitCounts, suitCounts + 4, 5) == 1) {
    flush = true;
  }

  bool allCardsUnique = true;
  for(int i = 0; i < 13; i++){
    if(handCounts[i] > 1){
        allCardsUnique = false;
        break;
    }
  }
  if(allCardsUnique){
    //if cards are unique and difference between biggest and lowest is 4
    // or the wrap around case for A,2,3,4,5
    if((cards[4].rank - cards[0].rank) == 4 
      || (cards[4].rank == 14
      && cards[0].rank == 2
      && cards[1].rank == 3
      && cards[2].rank == 4
      && cards[3].rank == 5)){
        straight = true;
      }    
  } else{
    straight = false;
  }
  if (straight && flush) {
    straightFlush = true;
  }
  if (straightFlush && handCounts[12] == 1) {
    royalFlush = true;
  }

  if (royalFlush) {
    rank = HandRanking::RoyalFlush;
  } else if (straight && flush) {
    rank = HandRanking::StraightFlush;
  } else if (fourAKind) {
    rank = HandRanking::FourOfAKind;
  } else if (pair && threeAKind) {
    rank = HandRanking::FullHouse;
  } else if (flush){
    rank = HandRanking::Flush;
  } else if (straight) {
    rank = HandRanking::Straight;
  } else if (threeAKind) {
    rank = HandRanking::ThreeOfAKind;
  } else if (twoPair) {
    rank = HandRanking::TwoPairs;
  } else if (pair) {
    rank = HandRanking::OnePair;
  } else {
    rank = HandRanking::HighCard;
  }
}

/// @brief Class for facilitating the running of a poker game
///
/// Holds a set of cards (player hands) for each player in the game. Has
/// functions for determining the winner and display the hands. Can break a tie
/// if two hands have the same rank
class PokerGame {
public:
  vector<Hand> playerHands;
  void addPlayerHand();
  void addPlayerHand(vector<Card> cards);
  void showPlayerHands();
  void determineHandWinner();

private:
  Deck deck;
  int numberOfPlayers = 0;
  int handSize = 5;

  void breakTie(HandRanking tiedRank, Hand player1, Hand player2);
  int highCardTieBreaker(Hand player1, Hand player2);
  int onePairTieBreaker(Hand player1, Hand player2);
  int twoPairsTieBreaker(Hand player1, Hand player2);
  int threeOfAKindTieBreaker(Hand player1, Hand player2);
  int straightTieBreaker(Hand player1, Hand player2);
  int flushTieBreaker(Hand player1, Hand player2);
  int fullHouseTieBreaker(Hand player1, Hand player2);
  int fourOfAKindTieBreaker(Hand player1, Hand player2);
  int straightFlushTieBreaker(Hand player1, Hand player2);

protected:
};

/// @brief Adds a new hand from the deck to the playerHands array
void PokerGame::addPlayerHand() {
  Hand hand;
  for (int i = 0; i < handSize; i++) {
    hand.addCard(deck.dealTopCard());
  }
  hand.sortHand();
  playerHands.push_back(hand);
  numberOfPlayers++;
}

/// @brief Adds a specified hand to the playerHands array
void PokerGame::addPlayerHand(vector<Card> cards) {
  Hand hand;
  for (int i = 0; i < handSize; i++) {
    hand.addCard(cards[i]);
  }
  hand.sortHand();
  playerHands.push_back(hand);
  numberOfPlayers++;
}

/// @brief Displays all cards in all player hands.
void PokerGame::showPlayerHands() {
  for (int i = 0; i < numberOfPlayers; i++) {
    std::cout << "Showing hand for player " << (i + 1) << "\n";
    playerHands[i].showHand();
  }
}

/// @brief Determines which player has the best hand
void PokerGame::determineHandWinner() {
  for (int i = 0; i < numberOfPlayers; i++) {
    playerHands[i].setRank();
    cout << "Player " << (i + 1)
         << " has hand of type: " << playerHands[i].getRank();
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
    cout << "Both players have a hand of the same rank, checking "
            "tie-breakers.";
    std::cout << std::endl;
    HandRanking tiedRank = playerHands[0].getRank();
    Hand player1 = playerHands[0];
    Hand player2 = playerHands[1];
    breakTie(tiedRank, player1, player2);
  }
}

/// @brief Determines the winner if two player hands are of equal rank
/// @param tiedRank The rank of the hands of the tied players
/// @param player1
/// @param player2
void PokerGame::breakTie(HandRanking tiedRank, Hand player1, Hand player2) {
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
    winner = -1; // If both are royal flushes its a tie.
    break;
  default:
    // This should not happen
    std::cout << "Invalid hand rank for tie breaking!\n";
    break;
  }

  if (winner == 1) {
    std::cout << "Player 1 wins the tie breaker!\n";
  } else if (winner == 2) {
    std::cout << "Player 2 wins the tie breaker!\n";
  } else {
    std::cout << "It's a tie! The hands are of equal rank.\n";
  }
}

/// @brief Break Ties between two hands of rank 'High Card'
///
/// This will check both hands which are sorted in ascending order.
/// By iterating from the end toward the beginning, we must find the first
/// instance where the cards do not match and the hand with the higher card
/// is the winner.
int PokerGame::highCardTieBreaker(Hand player1, Hand player2) {
  auto cards1 = player1.getCards();
  auto cards2 = player2.getCards();
  for (int i = handSize - 1; i >= 0; i--) {
    if (cards1[i] > cards2[i]) {
      return 1;
    } else if (cards1[i] < cards2[i]) {
      return 2;
    }
    if (i == 0) {
      return -1;
    }
  }
  return -1;
}

/// @brief Break ties between two hands of rank 'one pair'
///
/// The code will first see if one pair is higher than the other. If they are
/// the same pair value, high card rules will be used as backup tie breakers.
int PokerGame::onePairTieBreaker(Hand player1, Hand player2) {
  auto cards1 = player1.getCards();
  auto cards2 = player2.getCards();
  int pairValuePlayer1 = -1;
  int pairValuePlayer2 = -1;
  for (int i = 1; i < handSize; i++) {
    if (cards1[i] == cards1[i - 1]) {
      pairValuePlayer1 = cards1[i].rank;
    }
    if (cards2[i] == cards2[i - 1]) {
      pairValuePlayer2 = cards2[i].rank;
    }
  }
  if (pairValuePlayer1 > pairValuePlayer2) {
    return 1;
  } else if (pairValuePlayer1 < pairValuePlayer2) {
    return 2;
  } else {
    // the pairs are the same, resolve the tie by looking for best high-card
    for (int i = handSize - 1; i >= 0; i--) {
      if (cards1[i] > cards2[i]) {
        return 1;
      } else if (cards1[i] < cards2[i]) {
        return 2;
      }
    }
  }
  return -1;
}

/// @brief Breaks the tie for two hands of rank 'two pairs'
/// @param player1
/// @param player2
/// @return
int PokerGame::twoPairsTieBreaker(Hand player1, Hand player2) {
  // Find the highest pair and the highest non-paired card in each hand
  auto cards1 = player1.getCards();
  auto cards2 = player2.getCards();
  int player1Pair1 = -1;
  int player1Pair2 = -1;
  int player2Pair1 = -1;
  int player2Pair2 = -1;
  for (int i = 0; i < handSize - 1; i++) {
    if (cards1[i] == cards1[i + 1] 
      && cards1[i] > player1Pair1) {
      player1Pair2 = player1Pair1;
      player1Pair1 = cards1[i].rank;
    } else if (cards1[i] == cards1[i + 1] 
      && cards1[i] > player1Pair2) {
      player1Pair2 = cards1[i].rank;
    }
    if (cards2[i] == cards2[i + 1] 
      && cards2[i] > player2Pair1) {
      player2Pair2 = player2Pair1;
      player2Pair1 = cards2[i].rank;
    } else if (cards2[i] == cards2[i + 1] 
      && cards2[i] > player2Pair2) {
      player2Pair2 = cards2[i].rank;
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
      // both pairs are the same, resolve the tie by looking for best
      // high-card
      for (int i = handSize - 1; i >= 0; i--) {
        if (cards1[i] > cards2[i]) {
          return 1;
        } else if (cards1[i] < cards2[i]) {
          return 2;
        }
      }
    }
  }

  return -1;
}

/// @brief Breaks ties for two hands of rank 'three of a kind'
/// @param player1
/// @param player2
/// @return
int PokerGame::threeOfAKindTieBreaker(Hand player1, Hand player2) {
  // Find the entry in the handCounts array with a value of 3
  int *address1 = find(begin(player1.handCounts), end(player1.handCounts), 3);
  int *address2 = find(begin(player2.handCounts), end(player2.handCounts), 3);
  // subtract beginning-of-array address to get the index
  int index1 = address1 - begin(player1.handCounts);
  int index2 = address2 - begin(player2.handCounts);
  if (index1 > index2) {
    return 1;
  }
  if (index2 > index1) {
    return 2;
  } 
  //if the three of a kinds are equal, high card determines the winner. 
  return highCardTieBreaker(player1, player2);
}

/// @brief Breaks ties for two hands of rank 'straight'
/// @param player1
/// @param player2
/// @return
int PokerGame::straightTieBreaker(Hand player1, Hand player2) {
  auto cards1 = player1.getCards();
  auto cards2 = player2.getCards();
  //begin checking the 2nd highest ranked card in the hand
  //to handle the issue of "low ace straights"
  for (int i = handSize - 2; i >= 0; i--) {
    if (cards1[i] > cards2[i]) {
      return 1;
    } else if (cards1[i] < cards2[i]) {
      return 2;
    }
    if (i == 0) {
      return -1;
    }
  }
  return -1;
}

/// @brief Breaks ties for two hands of rank 'flush'
/// @param player1
/// @param player2
/// @return
int PokerGame::flushTieBreaker(Hand player1, Hand player2) {

  int i = 12;

  for (i; i >= 0; i--) {

    if (player1.handCounts[i] > player2.handCounts[i]) {
      return 1;
    } else if (player1.handCounts[i] < player2.handCounts[i]) {
      return 2;
    }
  }

  return -1;
}

/// @brief Breaks ties for two hands of rank 'full house'
/// @param player1
/// @param player2
/// @return
int PokerGame::fullHouseTieBreaker(Hand player1, Hand player2) {
  int tieBreaker = threeOfAKindTieBreaker(player1, player2);
  if(tieBreaker != -1){
    return tieBreaker; //tie broken by 3 of a kind
  }
  // Find the entry in the handCounts array with a value of 2 (the pair)
  int *address1 = find(begin(player1.handCounts), end(player1.handCounts), 2);
  int *address2 = find(begin(player2.handCounts), end(player2.handCounts), 2);
  // subtract beginning-of-array address to get the index
  int index1 = address1 - begin(player1.handCounts);
  int index2 = address2 - begin(player2.handCounts);
  if (index1 > index2) {
    return 1;
  }
  if (index2 > index1) {
    return 2;
  }
  //both the three of a kind and the pair are a tie so the hands are tied.  
  return -1;
}

/// @brief Breaks ties for two hands of rank 'four of a kind'
/// @param player1
/// @param player2
/// @return
int PokerGame::fourOfAKindTieBreaker(Hand player1, Hand player2) {
  // Find the entry in the handCounts array with a value of 3
  int *address1 = find(begin(player1.handCounts), end(player1.handCounts), 4);
  int *address2 = find(begin(player2.handCounts), end(player2.handCounts), 4);
  // subtract beginning-of-array address to get the index
  int index1 = address1 - begin(player1.handCounts);
  int index2 = address2 - begin(player2.handCounts);
  if (index1 > index2) {
    return 1;
  }
  if (index2 > index1) {
    return 2;
  }
  
  return highCardTieBreaker(player1, player2);
}

/// @brief Breaks ties for two hands of rank 'stright flush'
/// @param player1
/// @param player2
/// @return
int PokerGame::straightFlushTieBreaker(Hand player1, Hand player2) {
  return highCardTieBreaker(player1, player2);
}

/// @brief Runs tests on a collection of hands stored in json format
///
/// Hands are stored in a file in json format. This class will read
/// in each pair of hands anddetermine the winner and print it to the screen
class JsonPokerTests {
public:
  JsonPokerTests(string jsonFile);
  void ProcessTestsInJsonFile();

private:
  string fileLocation;
  Json::Value jsonData;
};

JsonPokerTests::JsonPokerTests(string fileName) {
  fileLocation = fileName;
  std::ifstream file(fileLocation);
  file >> jsonData;  
}

void JsonPokerTests::ProcessTestsInJsonFile() {  
  for (Json::ValueIterator itr = jsonData.begin(); itr != jsonData.end();
       itr++) {
    std::string propertyName = itr.name();
    
    cout << "\nInitiating tests of the following type: " << propertyName
         << "\n";
    Json::Value currentCategory = jsonData[propertyName];

    // iterate through all the hands in the current category (e.g. all the
    // straight flush tests)
    for (const auto &handPair : currentCategory) {
      vector<Card> cardsP1;
      vector<Card> cardsP2;
      for (const auto &val : handPair["p1"]) {
        std::string cardCode = val.asString();
        Card card(cardCode);
        cardsP1.push_back(card);
      }
      for (const auto &val : handPair["p2"]) {
        std::string cardCode = val.asString();
        Card card(cardCode);
        cardsP2.push_back(card);
      }
      PokerGame game;
      game.addPlayerHand(cardsP1);
      game.addPlayerHand(cardsP2);
      game.showPlayerHands();
      game.determineHandWinner();
      std::cout << std::endl;
    }
  }
}

/// @brief int main is the program entry point
///
/// Runs the code for comparing poker hands.
/// @return
int main() {

  JsonPokerTests jsonTester("PokerHandTests.json");  
  jsonTester.ProcessTestsInJsonFile();  

  return 0;
}
