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
    // boolean flags for hand type determination
    bool pair, twoPair, threeAKind, fourAKind = false;
    bool straight, flush, straightFlush, royalFlush = false;
    // arrays to store counts of card values
    int handCounts[13] = {0};  // 2 3 4 5 6 7 8 9 T J Q K A
    int suitCounts[4] = {0};   // Club Diamond Heart Spade

   protected:
};

void Hand::discardHand() {
    // clear hand and reset all calculated values to 0 and false
    cards.clear();
    handCounts[13] = {0};
    suitCounts[4] = {0};
    pair, twoPair, threeAKind, fourAKind = false;
    straight, flush, straightFlush, royalFlush = false;
}

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
    for (card c : cards) {
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
    for (int i; i < 8; i++) {
        if (handCounts[i] == 1 && handCounts[i + 1] == 1 &&
            handCounts[i + 2] == 1 && handCounts[i + 3] == 1 &&
            handCounts[i + 4] == 1) {
            straight = true;
        }
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

vector<card> Hand::getCards() { return cards; }

int main() {
    Deck deck;
    Hand hand;

    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());

    hand.setRank();

    return 0;
}