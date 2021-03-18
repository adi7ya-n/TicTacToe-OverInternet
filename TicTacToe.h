#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define FIRST_MOVE_SERVER 1
#define FIRST_MOVE_CLIENT 2

int turn()
{
    // returns 1 or 2, 1 indicates -> server plays first, 2 -> client plays first
    return(int((rand()%2 + 1)));
}

void displayBoard(vector <vector<char>> board)
{
	for (int i=0; i<3; i++)
	{
		cout<<"  ";
		for(int j=0; j<3; j++)
		{
			cout<<"  " <<  board[i][j]<<"  ";
			if(j!= 2)
				cout<<"|";
		}
		
		//print the underscores
		if(i!= 2)	
		cout<<endl<<"  -----+-----+-----";
		cout<<endl;
    }
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

        if(firstRowElement == '0')
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

        if(firstColumnElement == '0')
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

    if(firstDiagonalElement != '0')
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

    if(secondDiagonalElement != '0')
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


#endif