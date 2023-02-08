#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

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
        void updateAvailableSpaces(int row, int col);

    private:
        int moveNumber = 1;
        //each space in board will hold either an X or O once a move is played
        char board[3][3] = {' '};               
        vector<int> availableSpaces = {0,1,2,3,4,5,6,7,8};
        void addXorO(char XorO, int i, int j);
};

Game::Game()
{
    clearBoard();
}

//resets the game
void Game::clearBoard()
{
    //resets the board
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            board[i][j] = ' ';
        }
    }
    //refill list of available spaces
    availableSpaces = {0,1,2,3,4,5,6,7,8};
}

//removes a square from the availableSpaces vector based on the selected move (row, col)
//integers in the availableSpaces vector coorespond to the grid below 
//this allows us to track available spaces and precludes the need to scan the board before each computer move
//0 1 2
//3 4 5
//6 7 8        
void Game::updateAvailableSpaces(int row, int col){
    int selectedSpace = (row-1)*3 + (col-1);
    auto item = std::find(availableSpaces.begin(), availableSpaces.end(), selectedSpace);    
    availableSpaces.erase(item);
}

//gets user input and plays the user's move
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
                updateAvailableSpaces(row, col);
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

//makes a move for the computer, chosen at random from the available spaces
void Game::makeComputerMove(char computerMarker){    
    //choose a space from the available spaces at random
    int randomIndex = rand() % availableSpaces.size();
    cout << "\n" << randomIndex << "\n";
    int chosenSpace = availableSpaces[randomIndex];
    cout << "\n" << chosenSpace << "\n";
    //example chosen space and its row, column:  5/3 = 1 (second row).  5 % 3 = 2 (third column)
    int row = (chosenSpace / 3) + 1; //integer division by 3 gets the one-indexed row
    int col = (chosenSpace % 3) + 1; //chosen space mod 3 to get the one-indexed column  

    //place the space on the board    
    addXorO(computerMarker, row, col);   
    //update the list of available spaces to remove the move just played
    updateAvailableSpaces(row, col);

}

//displays the board in the terminal
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

//adds the passed in character to the board at the specified coordinates
void Game::addXorO(char character, int i, int j)
{
    board[--i][--j] = character;
}

//checks if the game is in a terminal state and returns true if it is, otherwise false
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

//program entry point
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
            g.makeComputerMove('O');
        }        
        xturn = !xturn;
        g.showBoard();
    }
    return 0;
}