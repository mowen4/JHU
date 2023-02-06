#include <iostream>
#include <stdlib.h>
#include <array>

using namespace std;

class Game
{
public:
    Game();
    void clearBoard();
    void showBoard();
    bool isGameOver();
    void promptUserforMove(char XorO);

private:
    int moveNumber = 1;
    char board[3][3] = {' '};
    void addXorO(char XorO, int i, int j);
};

Game::Game()
{
    clearBoard();
}

void Game::clearBoard()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void Game::promptUserforMove(char playerMarker)
{
    std::cout << "Enter row and column: ";
    int row, col;
    array<int, 2> userMoves;
    cin >> row >> col;
    addXorO(playerMarker, row, col);
}

void Game::showBoard()
{
    // system("CLS");
    cout << "-------------" << endl;
    cout << "| " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << " |" << endl;
    cout << "-------------" << endl;
    cout << "| " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << " |" << endl;
    cout << "-------------" << endl;
    cout << "| " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << " |" << endl;
    cout << "-------------" << endl;
}

void Game::addXorO(char character, int i, int j)
{
    board[--i][--j] = character;
}

bool Game::isGameOver()
{
    int x, y;

    // check colums
    for (x = 0; x < 3; x++)
    {

        if (board[0][x] == board[1][x] && board[0][x] == board[2][x] && board[0][x] != ' ')
        {
            cout << board[0][x] << " has won!" << endl;
            return true;
        }
    }
    // check rows
    for (y = 0; y < 3; y++)
    {
        if (board[y][0] == board[y][1] && board[y][0] == board[y][2] && board[y][0] != ' ')
        {
            cout << board[y][0] << " has won!" << endl;
            return true;
        }
    }

    // check diagonals
    if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[1][1] != ' ')
    {
        cout << board[0][0] << " has won!" << endl;
        return true;
    }
    if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[1][1] != ' ')
    {
        cout << board[0][2] << " has won!" << endl;
        return true;
    }

    if (moveNumber++ == 10)
    {
        cout << "Stalemate" << endl;
        return true;
    }

    return false;
}

int main()
{
    Game g;
    g.showBoard();
    bool xturn = true;
    while (!g.isGameOver())
    {
        if (xturn)
        {
            g.promptUserforMove('X');
        }
        else
        {
            g.promptUserforMove('O');
        }
        xturn = !xturn;
        g.showBoard();
    }
    return 0;
}