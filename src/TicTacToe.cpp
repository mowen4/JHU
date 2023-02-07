#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Game
{
    public:
        Game();
        void clearBoard();
        void showBoard();
        bool isGameOver();
        void promptUserforMove(char XorO);
        void makeComputerMove(char XorO);

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
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            board[i][j] = ' ';
        }
    }
}

void Game::promptUserforMove(char playerMarker)
{
    std::cout << "Enter row and column: ";
    bool validEntryReceived = false;
    while(!validEntryReceived){
        int row, col;
        cin >> row >> col;
        if(1 <= row && row <=3 && 1 <= col && col <= 3){
            if(board[row-1][col-1] == ' '){
                validEntryReceived = true;
                addXorO(playerMarker, row, col);
            }
            else{
                cout << "That space is already occupied, please choose another: ";
            }
        }
        else{
                cout << "Valid row and column choices are 1, 2, or 3. Please try again: ";
        }
    }
}


void Game::makeComputerMove(char computerMarker){
    //Add available spaces to vector. Available spaces will be numbered according to the grid below
    //0 1 2
    //3 4 5
    //6 7 8
    vector<int> availableSpaces;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){            
            if(board[i][j] == ' '){
                cout << (i*3+j); //debug, print out available spaces being considered
                availableSpaces.push_back(i*3+j);
            }
        }
    }
    //todo: make sure available spaces isn't empty?

    //choose a space from the available spaces at random
    int randomIndex = rand() % availableSpaces.size();
    cout << "\n" << randomIndex << "\n";
    int chosenSpace = availableSpaces[randomIndex];
    cout << "\n" << chosenSpace << "\n";
    //example chosen space and its row, column:  5/3 = 1 (second row).  5 % 3 = 2 (third column)
    int row = (chosenSpace / 3) + 1; //integer division by 3 gets the one-indexed row
    int column = (chosenSpace % 3) + 1; //chosen space mod 3 to get the one-indexed column  
        
    addXorO(computerMarker, row, column);   

}

void Game::showBoard()
{
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
    bool playerTurn = true;
    while (!g.isGameOver())
    {
        if (playerTurn)
        {
            g.promptUserforMove('X');
        }
        else
        {
            g.makeComputerMove('O');
        }
        //TODO: Only swap the turn if a valid move was played (e.g. can't be a move into a spot already played or a coordinate outside the grid space)
        playerTurn = !playerTurn;
        g.showBoard();
    }
    return 0;
}