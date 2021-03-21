#include "TicTacToe.h"

int main()
{
    srand(time(0)); // seed set to time(-) for rand()

    vector <vector<char>> board = {
        {'-','-','-'},
        {'-','-','-'},
        {'-','-','-'}
    }; // create the board

    // setup server, wait for client to join the game

    // client has connected, run rand() to see who plays first
    startGame(board);
}