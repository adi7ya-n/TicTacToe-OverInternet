#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define FIRST_MOVE_SERVER 1
#define FIRST_MOVE_CLIENT 2

#define print std::cout
#define newline std::endl

using namespace std;

void displayBoard(vector <vector<char>> board)
{
    print<<newline<<newline<<newline;

	for (int i=0; i<3; i++)
	{
		print<<"  ";
		for(int j=0; j<3; j++)
		{
			print<<"  " <<  board[i][j]<<"  ";
			if(j!= 2)
				print<<"|";
		}
		
		//print the underscores
		if(i!= 2)	
		print<<newline<<"  -----+-----+-----";
		print<<newline;
    }

    print<<newline;
}

char checkResult(vector <vector<char>> board)
{
    // Returns 'X' if X wins and 'O' if O wins, 'D' = draw, 'N' - no result
    // go over all rows, columns, diagonals

    // check if the board is full, ie find the first occurence of 0

    bool fullBoard = true; // assume board has no 0's left.

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(board[i][j] == '0')
                fullBoard = false; // 0 has been found, set fullBoard = false;
        }
    }
    
    // rows

    for(int i=0; i<3; i++)
    {
        char firstRowElement = board[i][0];

        if(firstRowElement == '-')
            break;
        int j;
        for(j=1; j<3; j++)
        {
            if(firstRowElement != board[i][j])
                break;
        }

        if(j ==3)
            // found winner
            return firstRowElement;
    }

    // columns

    for(int i=0; i<3; i++)
    {
        char firstColumnElement = board[0][i];

        if(firstColumnElement == '-')
            break;
        int j;
        for(j=1; j<3; j++)
        {
            if(firstColumnElement != board[j][i])
                break;
        }

        if(j == 3)
            // found winner
            return firstColumnElement;
    }

    // diags

    char firstDiagonalElement = board[0][0];

    if(firstDiagonalElement != '-')
    {
        int i;
        for (i=1; i<3; i++)
        {
            if(firstDiagonalElement != board[i][i])
                break;
        }

        if(i == 3)
            return firstDiagonalElement;
    }

    char secondDiagonalElement = board[0][2];

    if(secondDiagonalElement != '-')
    {
        int i;
        for(i=1; i<3; i++)
        {
            if(secondDiagonalElement != board[i][3-i-1])
                break;   
        }

        if(i == 3)
            return secondDiagonalElement;
    }

    if(fullBoard == true) // no more 0's left, entire board has been filled.
        return 'D';    

    return 'N';
}

void sendTurn(int turn);

void startGame(vector <vector<char>> &board)
{
    int firstMove = int((rand()%2 + 1));

    print<<newline<<newline;

    if(firstMove == FIRST_MOVE_SERVER)
        print<<"You are playing first! Your character is X."<< newline;
    else
        print<<"You are playing second! Your character is O."<< newline;

    print<<"To play, you must enter the position number (1-9) of the location where you want to place your piece."<< newline
         <<"The numbering is as shown in the following diagram: "<<newline<<newline;

    displayBoard({{'1','2','3'}, {'4','5','6'}, {'7','8','9'}});

}

#endif