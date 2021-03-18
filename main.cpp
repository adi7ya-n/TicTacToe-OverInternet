#include "TicTacToe.h"

int main()
{
    srand(time(0)); // seed set to time(0) for rand()

    std::vector <vector<char>> board = {
        {'0','0','0'},
        {'0','0','0'},
        {'0','0','0'}
    }; // create the board

    // setup server, wait for client to join the game

    // client has connected, run rand() to see who plays first

    int turn = turn();

    if(turn == FIRST_MOVE_SERVER)
    {
        // server plays first
        std::cout<<"You are going to play first!"<<std::endl;
        sendTurn(turn);
        startGame(turn);
    }
    else
    {
        std::cout<<"You are going to play second!"<<std::endl;
        sendTurn(turn);
        // wait for client to make the first move.
    }
        


    

}