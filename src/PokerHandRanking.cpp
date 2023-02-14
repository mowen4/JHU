#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <vector>

using namespace std;

const map<char, int> cardValues = {
    {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};

const char Suits[4] = {'S', 'H', 'D', 'C'};

const char Values[13] = {'2', '3', '4', '5', '6', '7', '8',
                         '9', 'T', 'J', 'Q', 'K', 'A'};

struct card {
    char value;
    char suit;
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

class Hand {
   public:
    void addCard(card c);
    void showHand();
    bool isHandFull();
    void discardHand();
   private:
    vector<card> cards;

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

int main() {
    Deck d;
    Hand h;
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());
    h.addCard(d.dealTopCard());

    h.showHand();


    return 0;
}
