#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>

#include <time.h>
#include <vector>
#include <limits>

namespace c_libs
{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h> 
    #include <sys/socket.h>
    #include <netinet/in.h>
}

#define PORT_NUMBER 9518

#define FIRST_MOVE_SERVER 1
#define FIRST_MOVE_CLIENT 2

#define VALID_LOCATION 0
#define INVALID_LOCATION 1
#define ALREADY_OCCUPIED 2

#define MOVE_CONFIRMED 'y'
#define MOVE_NOT_CONFIRMED 'n'

#define print std::cout
#define newline std::endl
#define input std::cin

char playerCharacter = '-'; // Will be set depending on who goes first.
int sockfd, acceptfd;

const char * gameOver = "E";


using namespace std;
using namespace c_libs;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

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

    print<<newline<<newline;
}

char checkResult(vector <vector<char>> &board)
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

int checkIfMoveIsValid(vector <vector<char>> &board, int move)
{
    if(!(move>0 && move<10))
    {
        return INVALID_LOCATION;
    }
    else
    {
        int rowNum = 0, colNum = 0;
        if(move % 3 != 0)
            rowNum = int(move/3);
        else
            rowNum = int(move/3) - 1;
        colNum = move - 3*rowNum - 1;

        if(board[rowNum][colNum] != '-')
        {
            return ALREADY_OCCUPIED;
        }
    }

    
    return VALID_LOCATION;
}

char confirmMove(vector <vector<char>> &board, int move)
{
    int rowNum = 0, colNum = 0;
    if(move % 3 != 0)
        rowNum = int(move/3);
    else
        rowNum = int(move/3) - 1;
    colNum = move - 3*rowNum - 1;
    char decision = 'n';
    char originalValue = board[rowNum][colNum];
    board[rowNum][colNum] = playerCharacter;
    print<<"This will be the board after modification: ";
    displayBoard(board);
    print<<"Press y to confirm move or n to try again: ";
    input>>decision;

    while(decision != 'y' and decision != 'n' and decision != 'Y' and decision != 'N')
    {
        print<<"Invalid decision. Please type y or n:  ";
        input>>decision;
    }

    if(decision == 'Y' or decision == 'y')
        return MOVE_CONFIRMED;
    else if (decision == 'N' or decision == 'n')
    {
        board[rowNum][colNum] = originalValue;
        return MOVE_NOT_CONFIRMED;
    }
}

int readValidMove(vector <vector<char>> &board, int &move)
{
    print<<"Enter your move: ";
    input>>move;

    while(input.fail())
    {
        // cin failed, retry
        input.clear();
        input.ignore(std::numeric_limits<streamsize>::max(), '\n');
        print<<"Not an integer. Please enter a value in the range 1 to 9:  ";
        input>>move;
    }

    int resultOfMove = checkIfMoveIsValid(board, move);

    while(resultOfMove != VALID_LOCATION)
    {
        input.clear();
        input.ignore(std::numeric_limits<streamsize>::max(), '\n');;

        if(resultOfMove == INVALID_LOCATION)
        {
            print<<"Please enter a value in the range of 1 to 9: ";
        }
        else if(resultOfMove == ALREADY_OCCUPIED)
        {
            print<<"This location has already been occupied. Please choose another position:  ";
        }
        
        input>>move;
        resultOfMove = checkIfMoveIsValid(board, move);
    }
}

void readMove(vector <vector<char>> &board)
{
    int move;
    readValidMove(board, move);

    // check if the entered move is ok by displaying the modified board.
    
    char confirmation = confirmMove(board, move);

    while(confirmation != MOVE_CONFIRMED)
    {
        print<<"Enter move: ";
        readValidMove(board, move);
        confirmation = confirmMove(board, move);
    }
        
}

void sendMove(vector <vector<char>> &board)
{
    readMove(board);
    char* message = new char[9];
    int k=0;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            message[k++] = board[i][j];
        }
    }

    if(write(acceptfd, message, 9) < 0)
        error("WAS NOT ABLE TO SEND MOVE TO CLIENT.");
    
    delete message;
}

void receiveMove(vector <vector<char>> &board)
{
    char buffer[9];
    if(c_libs::read(acceptfd,buffer,9) < 0)
        error("ERROR RECEIVING MOVE FROM CLIENT");

    // update the board
    int rowNum = 0, colNum = 0;

    for(int i=0; i<9; i++)
    {    
        rowNum = int(i/3);
        colNum = i - 3*rowNum;
        board[rowNum][colNum] = buffer[i];
    }
}

void startGame(vector <vector<char>> &board)
{
    int firstMove = int((rand()%2 + 1));
    
    print<<newline<<newline;
    
    displayBoard({{'1','2','3'}, {'4','5','6'}, {'7','8','9'}});
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    
    if(sockfd < 0)
        error("ERROR opening socket.");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT_NUMBER);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    
    print<<"Waiting for player to join..."<<newline;

    while(1)
    {
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        acceptfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if(acceptfd < 0)
            error("ERROR ON ACCEPT");

        print<<"Connected to player!"<<newline;

        const char* message;

        if(firstMove == FIRST_MOVE_SERVER)
        {
            print<<"You are playing first! Your character is X."<< newline;
            playerCharacter = 'X';
            message = "X";
        }
        else
        {
            print<<"You are playing second! Your character is O."<< newline;
            playerCharacter = 'O';
            message = "O";
        }
        print<<"To play, you must enter the position number (1-9) of the location where you want to place your piece."<< newline
            <<"The numbering is as shown in the following diagram: "<<newline<<newline;

        // send information of first move to the client.
    
        char buffer[9];

        if(write(acceptfd, message, 1) < 0)
            error("ERROR IN SENDING FIRST MOVE TO CLIENT.");
        
        if(firstMove == FIRST_MOVE_SERVER)
        {
            while(checkResult(board) == 'N') // no result yet
            {
                receiveMove(board);
                sendMove(board);
            }
        }
        else
        {
            while(checkResult(board) == 'N') // no result yet
            {
                sendMove(board);
                receiveMove(board);
            }
        }

        char result = checkResult(board);
        if(result == playerCharacter)
            print<<"Congrats! You won!! "<<newline<<newline;
        else if(result == 'D')
            print<<"Draw match. "<<newline<<newline;
        else
            print<<"You lost :( "<<newline<<newline;
        
        print<<"Waiting for response from client..."<<newline;

        if(c_libs::read(acceptfd,buffer,1) < 0)
            error("ERROR RECEIVING CLIENT REMATCH CONFIRMATION");
        
        if(buffer[0] == 'y')
            print<<"Opponent wants to play another game!"<<newline;
        else
            print<<"Opponent is done playing for now!"<<newline;

        print<<"Would you like to play again? Press y or n to confirm: ";

        char decision;
        input>>decision;

        while(decision != 'y' and decision != 'n' and decision != 'Y' and decision != 'N')
        {
            input.clear();
            input.ignore(std::numeric_limits<streamsize>::max(), '\n');
            print<<"Invalid decision. Please type y or n:  ";
            input>>decision;
        }

        if(buffer[0] == 'n')
        {
            // client dont want to play, close the tcp connection and exit the program.
            close(acceptfd);
            close(sockfd);
            print<<"Searching for new opponent..."<<newline;
        }
        else
        {
            // client wants to play
            if(decision == 'n' or decision == 'N')
            {
                // tell the client that the server does not want to play another game and then exit
                write(acceptfd, gameOver, 1);
                close(acceptfd);
                close(sockfd);
                exit(0);
            }
        }
    }
}

#endif