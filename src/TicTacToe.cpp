#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

/// @brief The Game class holds the code from playing a tic-tac-toe game
///
/// This class holds data members for storing the tic tac toe board state, current move number, and remaining available moves
/// It holds functions for allowing a human player to make a move as well as for a computer to make a move
/// The game is played via terminal input and the board is printed to the terminal as well
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
        //each space in board will hold either an X or O once a move is played
        char board[3][3] = {' '};               
        vector<int> availableSpaces = {0,1,2,3,4,5,6,7,8};
        void addXorO(char XorO, int i, int j);
        void updateAvailableSpaces(int row, int col);
};

/// @brief constructs a new game with an empty board and all spaces available
///
/// The constructor for Game. Calls clearBoard to set the game board to the correct initial state.
Game::Game()
{
    clearBoard();
}

/// @brief Resets the game board 
///
/// Sets all spaces in board[][] to an empty space and sets availableSpaces to include all spaces on the board
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


/// @brief removes the selected space from the list of availableSpaces
///
///removes a square from the availableSpaces vector based on the selected move (row, col)
///integers in the availableSpaces vector coorespond to the grid below 
///this allows us to track available spaces and precludes the need to scan the board before each computer move
///0 1 2
///3 4 5
///6 7 8        
void Game::updateAvailableSpaces(int row, int col){
    int selectedSpace = (row-1)*3 + (col-1);
    auto item = std::find(availableSpaces.begin(), availableSpaces.end(), selectedSpace);    
    availableSpaces.erase(item);
}

/// @brief Gets user input and plays the user's move
///
/// This function will prompt the user for their next game move. The move should be the row followed by a space and then the column. 
/// If the space is already occupied, the user is prompted for a different move
/// If the selected row or column are outside the playable area, the user is prompted for a different move
/// Once a valid move is entered, the game board and available spaces are updated. 
/// @param playerMarker This is the marker used by the player, either 'X' or 'O'

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

/// @brief Makes a move for the computer, chosen at random from the available spaces
///
/// This function selects a move for the computer at random from the list of available spaces. The game board and available spaces are updated.
/// @param computerMarker the marker used by the computer, either 'X' or 'O'

void Game::makeComputerMove(char computerMarker){    
    //choose a space from the available spaces at random
    int randomIndex = rand() % availableSpaces.size();   
    int chosenSpace = availableSpaces[randomIndex];
   
    //example chosen space and its row, column:  5/3 = 1 (second row).  5 % 3 = 2 (third column)
    int row = (chosenSpace / 3) + 1; //integer division by 3 gets the one-indexed row
    int col = (chosenSpace % 3) + 1; //chosen space mod 3 to get the one-indexed column  

    //place the space on the board    
    addXorO(computerMarker, row, col);   
    //update the list of available spaces to remove the move just played
    updateAvailableSpaces(row, col);

}

/// @brief displays the board in the terminal
///
/// Displays the game board in the terminal. Each column is separated by a vertical bar and the rows are separated by dashes
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

/// @brief  Adds the passed in character to the board at the specified coordinates
///
/// This function places a character onto the game board following a player or computer move. 
/// This function does not validate the move, the move is assumed to have been validated prior to this function being called
/// @param character The player marker to be placed, will be either an 'X' or an 'O'
/// @param i The selected row where the marker will be placed
/// @param j The selected column where the marker will be placed
void Game::addXorO(char character, int i, int j)
{
    board[--i][--j] = character;
}

/// @brief checks if the game is in a terminal state
/// 
/// Checks the board for three markers of the same type in a row in horizontal, vertical or diagonal directions
/// @return Returns true if the game is over, otherwise returns false
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
        cout << "Tie Game" << endl;
        return true;
    }

    return false;
}

/// @brief This class assists in running the Game
///
/// This class gets called by the int main() function and assists with running the game and keeping track of whose turn it is. 
class GameDriver: protected Game
{
    public:
        void run();
};

/// @brief Runs the game and makes calls for each player to make a move. 
///
/// Asks each player to make a move based on whose turn it is. Updates the playerTurn variable after each move is completed.
void GameDriver::run()
{
    showBoard();
    bool playerTurn = true;
    while (!isGameOver())
    {
        if (playerTurn)
        {
            promptUserforMove('X');
        }
        else
        {
            makeComputerMove('O');
        }        
        playerTurn = !playerTurn;
        showBoard();
    }
}

/// @brief  program entry point
///
/// This main function is the driver for the tic tac toe board game. 
/// @return  returns a zero on successful completion of the program
int main()
{
    GameDriver gd;
    gd.run();

    return 0;
}