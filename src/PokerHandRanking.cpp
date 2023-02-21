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
    bool pair = false;
    bool twoPair = false;
    bool threeAKind = false;
    bool fourAKind = false;
    bool straight = false;
    bool flush = false;
    bool straightFlush = false;
    bool royalFlush = false;
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
    pair = false;
    twoPair = false;
    threeAKind = false;
    fourAKind = false;
    straight = false;
    flush = false;
    straightFlush = false;
    royalFlush = false;
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
        cout << "Both players have a hand of the same rank, checking "
                "tie-breakers.";
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
    return -1;
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
            // both pairs are the same, resolve the tie by looking for best
            // high-card
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
    Deck deck;
    Hand hand;

    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());
    hand.addCard(deck.dealTopCard());

    hand.setRank();

    PokerGame game;
    game.addPlayerHand();
    game.addPlayerHand();
    game.showPlayerHands();
    game.determineHandWinner();

    return 0;
}
