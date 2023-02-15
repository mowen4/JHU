#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <vector>

using namespace std;

//TODO: wouldn't compile with map set as a const after I started indexing in the map to try to rank the hand. Not sure what I was doing wrong but I feel like const makes sense for this but don't want to check in a build that won't compile so putting this TODO here. 
map<char, int> cardValues = {
    {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};

const char Suits[4] = {'S', 'H', 'D', 'C'};

const char Values[13] = {'2', '3', '4', '5', '6', '7', '8',
                         '9', 'T', 'J', 'Q', 'K', 'A'};

struct card {
    char value;
    char suit;

    //overriding < operator to allow for sort() function calls
    bool operator < (const card& card) const
    {
        return (value < card.value);
    }
    //overriding > as well for good measure    
    bool operator > (const card& card) const
    {
        return (value > card.value);
    }
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


std::ostream& operator<<(std::ostream& os, const HandRanking rank)
{
  switch(rank)
  {
    case HandRanking::HighCard:
        os << "HighCard";
        break;
    case HandRanking::OnePair:
        os << "OnePair";
        break;
    case HandRanking::TwoPairs:
        os << "TwoPairs";
        break;
    case HandRanking::ThreeOfAKind:
        os << "ThreeOfAKind";
        break;
    case HandRanking::Straight:
        os << "Straight";
        break;
    case HandRanking::Flush:
        os << "Flush";
        break;
    case HandRanking::FullHouse:
        os << "FullHouse";
        break;
    case HandRanking::FourOfAKind:
        os << "FourOfAKind";
        break;
    case HandRanking::StraightFlush:
        os << "StraightFlush";
        break;
    case HandRanking::RoyalFlush:
        os << "RoyalFlush";
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
    HandRanking getRank();
    void setRank();       
    
   private:
    vector<card> cards;
    HandRanking rank; 
    //bool cardSorter(card lhs, card rhs);
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

void Hand::discardHand() {
    cards.clear();
}

void Hand::addCard(card c){
    cards.push_back(c);
}

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

HandRanking Hand::getRank(){
    return rank;
}

void Hand::setRank(){
    if(isRoyalFlush()){
        rank = HandRanking::RoyalFlush;
    }
    else if(isStraightFlush()){
        rank = HandRanking::StraightFlush;
    }
    else if(isFourOfAKind()){
        rank = HandRanking::FourOfAKind;
    }
    else if(isFullHouse()){
        rank = HandRanking::FullHouse;
    }
    else if(isStraight()){
        rank = HandRanking::Straight;
    }
    else if(isThreeOfAKind()){
        rank = HandRanking::ThreeOfAKind;
    }
    else if(isTwoPairs()){
        rank = HandRanking::TwoPairs;
    }
    else if(isOnePair()){
        rank = HandRanking::OnePair;
    }
    else{
        rank = HandRanking::HighCard;
    }
}

bool Hand::isRoyalFlush(){
    //TODO: move this sort call out somewhere else
    sort(cards.begin(), cards.end());    
    if(isFlush()
      && isStraight()
      && cards[4].value == 'A'){
        return true;
    }    
    return false;    
}

bool Hand::isStraightFlush(){
    if(isFlush()
      && isStraight()){
        return true;
    }
    return false;
}

bool Hand::isFourOfAKind(){
    //check all sets of four consecutive cards for four of a kind
    for (int i = 0; i <= cards.size() - 4; i++) {
        if (cards[i].value == cards[i+1].value 
          && cards[i].value == cards[i+2].value
          && cards[i].value == cards[i+3].value) {
            return true;
        }
    }
    return false;
}

bool Hand::isFullHouse(){
    //checks for sorted hand with three of a kind then a pair
    if(cards[0].value == cards[1].value
      && cards[1].value == cards[2].value
      && cards[3].value == cards[4].value){
        return true;
    }
    //checks for sorted hand with a pair then three of a kind
    if(cards[0].value == cards[1].value
      && cards[2].value == cards[3].value
      && cards[3].value == cards[4].value){
        return true;
    }
    return false;
}

bool Hand::isFlush(){
    if(cards[0].suit == cards[1].suit
      && cards[0].suit == cards[2].suit
      && cards[0].suit == cards[3].suit
      && cards[0].suit == cards[4].suit
    ){
        return true;
    }
    return false;
}

bool Hand::isStraight(){
    for(int i = 0; i < cards.size() - 1; i++){        
        //doing subtraction so lookup the value in the map since you can't 
        //subtract 7 from 'A' 
        if(cardValues[cards[i+1].value] - cardValues[cards[i].value] != 1){
            return false;
        }
    }
    return true;
}

bool Hand::isThreeOfAKind(){
    //check all sets of 3 consecutive cards for three of a kind
    for (int i = 0; i <= cards.size() - 3; i++) {
        if (cards[i].value == cards[i+1].value 
          && cards[i].value == cards[i+2].value) {
            return true;
        }
    }
    return false;
}

bool Hand::isTwoPairs(){
    int numberOfPairs = 0;
    for(int i = 0; i < cards.size() - 1; i++){
        if(cards[i].value == cards[i+1].value){
            numberOfPairs++;
            i++; //already found pair for this index so skip ahead by one
        }
    }
    return numberOfPairs == 2;
}

bool Hand::isOnePair(){
    int numberOfPairs = 0;
    for(int i = 0; i < cards.size() - 1; i++){
        if(cards[i].value == cards[i+1].value){
            numberOfPairs++;
            i++; //already found pair for this index so skip ahead by one
        }
    }
    return numberOfPairs == 1;
}


//Stubbing out some method names that a poker game might need
//TODO: Use this class to run a game of poker with two players...? 
class PokerGame{
    public:                
        PlayHand();
        DetermineHandWinner();        
   private:
        int numberOfPlayers = 2; 
   protected:
};

int main() {
    Deck d;
    Hand h;
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());

    h.showHand();

    h.setRank();
    cout << h.getRank();

    return 0;
}
