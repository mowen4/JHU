#include <iostream>
#include <iterator>
#include <algorithm>
#include <random>
#include <vector>
#include <map>
#include <deque>

using namespace std;

const map<char, int> cardValues = {{'2', 2}, {'3', 3}, {'4', 4}, {'5', 5},
    {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12},
    {'K', 13}, {'A', 14}};

const char Suits[4] = {'S', 'H', 'D', 'C'};
const char Values[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 
    'K', 'A'};

struct card
{
    char value;
    char suit;
};

class Deck
{
public:
    Deck();
    ~Deck();
    card dealTopCard();

private:
    deque<card> deck;
};

Deck::Deck()
{
    vector<card> temp;
    for (char s : Suits)
    {
        for (char v : Values)
        {
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

Deck::~Deck()
{
}

card Deck::dealTopCard()
{
    card c = deck.front();
    deck.pop_front();
    return c;
}

class Hand
{
public:
    void addCard(card c);
    void isHandFull();

private:
    vector<card> cards;

protected:
};

// Enum to store the possible hand rankings
enum HandRanking
{
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

int main()
{
    Deck d;
    card c = d.dealTopCard();
    std::cout << c.value << c.suit << std::endl;
    c = d.dealTopCard();
    std::cout << c.value << c.suit << std::endl;
    c = d.dealTopCard();
    std::cout << c.value << c.suit << std::endl;
    c = d.dealTopCard();
    std::cout << c.value << c.suit << std::endl;

    return 0;
}
