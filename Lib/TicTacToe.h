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
    #include <netdb.h>
}

#define PORT_NUMBER 9518

#define FIRST_MOVE_SERVER 1
#define FIRST_MOVE_CLIENT 2

#define CLIENT_GOES_SECOND 2
#define CLIENT_GOES_FIRST 1

#define PLAYER_1 1
#define PLAYER_2 2

#define VALID_LOCATION 0
#define INVALID_LOCATION 1
#define ALREADY_OCCUPIED 2

#define MOVE_CONFIRMED 'y'
#define MOVE_NOT_CONFIRMED 'n'

#define GAME_OVER 1
#define NO_RESULT 0

#define print std::cout
#define newline std::endl
#define input std::cin

char playerCharacter = '-'; // Will be set depending on who goes first.
char opponentCharacter = '-';
char gameResult = 'N';
int sockfd, acceptfd;

const char * gameOver = "E";
const char* message;

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

    displayBoard(board);
    bool fullBoard = true; // assume board has no 0's left.

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(board[i][j] == '-')
                fullBoard = false; // 0 has been found, set fullBoard = false;
        }
    }
    
    // rows

    for(int i=0; i<3; i++)
    {
        char firstRowElement = board[i][0];

        if(firstRowElement == '-')
        {
            if(i != 2)
            {
                continue;
            }    
            else
                break;
        }    
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

    // print<<"No winner along rows"<<newline;

    // columns

    for(int i=0; i<3; i++)
    {
        char firstColumnElement = board[0][i];

        if(firstColumnElement == '-')
        {
            if(i != 2)
            {
                continue;
            }    
            else
                break;
        }
        int j;
        for(j=1; j<3; j++)
        {
            // print<<"first col element: "<<firstColumnElement<<" compared with "<<board[j][i]<<newline;
            if(firstColumnElement != board[j][i])
                break;
        }

        if(j == 3)
            // found winner
            return firstColumnElement;
    }
    // print<<"No winner along cols"<<newline;
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
    // print<<"No winner along first diagonal  "<<newline;
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
    // print<<"No winner along 2ns diagonal  "<<newline;

    if(fullBoard == true) // no more - left, entire board has been filled.
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

int sendMove(vector <vector<char>> &board, int fd)
{
    print<<"Inside sendmove"<<newline;
    readMove(board);
    char* message = new char[10];
    int k=1;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            message[k++] = board[i][j];
        }
    }
    char result = checkResult(board);

    // print<<"Player character: "<<playerCharacter<<" Opponent character: "<<opponentCharacter<<newline;

    // print<<"Game result before sending the data: "<<result<<newline;

    if(result == 'D')
        message[0] = 'D';
    else if(result == playerCharacter)
        message[0] = playerCharacter;
    else if(result == opponentCharacter)
        message[0] = opponentCharacter;
    else
        message[0] = 'N';

    if(write(fd, message, 10) < 0)
        error("WAS NOT ABLE TO SEND MOVE TO CLIENT.");
    
    delete message;

    gameResult = result;
    
    if(gameResult == 'N')
        return NO_RESULT;
    else
        return GAME_OVER;
}

int receiveMove(vector <vector<char>> &board, int fd)
{
    // cout<<"Inside receivemove"<<newline;
    char buffer[10];
    if(c_libs::read(fd,buffer,10) < 0)
        error("ERROR RECEIVING MOVE FROM CLIENT");

    // update the board
    int rowNum = 0, colNum = 0;

    for(int i=1; i<10; i++)
    {    
        rowNum = int((i-1)/3);
        colNum = i - 1 - 3*rowNum;
        print<<rowNum<<" "<<colNum<<newline;

        board[rowNum][colNum] = buffer[i];
    }
    
    // print<<"Game result: "<<buffer[0]<<newline;

    if(buffer[0] == playerCharacter)
    { 
        displayBoard(board);
        print<<"You won!"<<newline;
    }
    else if(buffer[0] == opponentCharacter)
    {
        displayBoard(board);
        print<<"You lost! :("<<newline;
    }
    else if(buffer[0] == 'D')
    {
        displayBoard(board);
        print<<"Draw match!"<<newline;
    }
    else
    {
        print<<newline<<newline<<"Opponent has made their move. Here is the updated board!"<<newline;
        displayBoard(board);
    }

    gameResult = buffer[0];  

    if(buffer[0] == 'N')
        return NO_RESULT;
    else
        return GAME_OVER;
}

void displayRules(int firstMove)
{
    if(firstMove == PLAYER_1)
    {
        print<<"You are playing first! Your character is X."<< newline;
        playerCharacter = 'X';
        opponentCharacter = 'O';
        message = "2";
    }
    else
    {
        print<<"You are playing second! Your character is O."<< newline;
        playerCharacter = 'O';
        opponentCharacter = 'X';
        message = "1";
    }
    print<<"To play, you must enter the position number (1-9) of the location where you want to place your piece."<< newline
        <<"The numbering is as shown in the following diagram: "<<newline<<newline;

    displayBoard({{'1','2','3'}, {'4','5','6'}, {'7','8','9'}});
}

void startGame(vector <vector<char>> &board)
{
    // vector <vector<char>> testBoard = {
    //     {'O', '-', 'X'},
    //     {'O', '-', 'X'},
    //     {'-', '-', 'X'}
    // };

    //print<<checkResult(testBoard);

    int firstMove = int((rand()%2 + 1));
    
    print<<newline<<newline;
    
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

        displayRules(firstMove);
        // send information of first move to the client.
    
        char buffer[9];

        if(write(acceptfd, message, 1) < 0)
            error("ERROR IN SENDING FIRST MOVE TO CLIENT.");

        
        print<<"My first move is: "<<firstMove<<newline;
        
        if(firstMove == FIRST_MOVE_CLIENT)
        {
            /*
            while(checkResult(board) == 'N') // no result yet
            {
                receiveMove(board, acceptfd);
                sendMove(board, acceptfd);
            }
            */
            while(1)
            {
                int receiveResult = receiveMove(board, acceptfd);
                // print<<"Receive result : "<<receiveResult<<newline;
                if(receiveResult == GAME_OVER)
                    break;
                int sendResult = sendMove(board, acceptfd);
                // print<<"Send result: "<<sendResult<<newline;
                if(sendResult == GAME_OVER)
                    break;
            }
        }
        else
        {
            /*
            while(checkResult(board) == 'N') // no result yet
            {
                sendMove(board, acceptfd);
                receiveMove(board, acceptfd);
            }
            */
            while(1)
            {
                int sendResult = sendMove(board, acceptfd);
                // print<<"Send result: "<<sendResult<<newline;
                if(sendResult == GAME_OVER)
                    break;
                int receiveResult = receiveMove(board, acceptfd);
                // print<<"Receive result : "<<receiveResult<<newline;
                if(receiveResult == GAME_OVER)
                    break;
            }

        }

        // char result = checkResult(board);
        // if(result == playerCharacter)
        //     print<<"Congrats! You won!! "<<newline<<newline;
        // else if(result == 'D')
        //     print<<"Draw match. "<<newline<<newline;
        // else
        //     print<<"You lost :( "<<newline<<newline;

        if(gameResult == playerCharacter)
            print<<"Congrats!, You won :D"<<newline;
        else
            print<<"You lost :("<<newline;
        
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

void joinGame(vector <vector<char>> &board)
{
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buffer[9];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
        error("ERROR OPENING SOCKET ON CLIENT");
    
    server = gethostbyname("localhost");

    if(server == NULL)
        error("COULD NOT FIND THE HOST SERVER");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT_NUMBER);

    while(1)
    {
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR connecting to Host");

        if(c_libs::read(sockfd, buffer, 1) < 0)
            error("ERROR reading the first move from the host");
        
        int firstMove = int(buffer[0] - '0');


        bzero(buffer, 9);

        print<<"Connected to server!"<<newline;

        displayRules(firstMove);

        print<<"My first move is: "<<firstMove<<newline;

        // if client gets 2, then receive the move

        if(firstMove == CLIENT_GOES_FIRST)
        {
            while (1)
            {
                int sendResult = sendMove(board, sockfd);
                // print<<"Send result: "<<sendResult<<newline;
                if(sendResult == GAME_OVER)
                    break;
                int receiveResult = receiveMove(board, sockfd);

                // print<<"Receive result : "<<receiveResult<<newline;
                if(receiveResult == GAME_OVER)
                    break;
            }
        }
        else
        {
            while (1)
            {
                int receiveResult = receiveMove(board, sockfd);
                // print<<"Receive result : "<<receiveResult<<newline;
                if(receiveResult == GAME_OVER)
                    break;
                int sendResult = sendMove(board, sockfd);
                // print<<"Send result: "<<sendResult<<newline;
                if(sendResult == GAME_OVER)
                    break;
            }
        }

        // char result = checkResult(board);
        // if(result == playerCharacter)
        //     print<<"Congrats! You won!! "<<newline<<newline;
        // else if(result == 'D')
        //     print<<"Draw match. "<<newline<<newline;
        // else
        //     print<<"You lost :( "<<newline<<newline;

        // indicate if client wants to play another game

        print<<"Do you want to play another game? Press y or n to confirm: ";

        char decision;
        input>>decision;

        while(decision != 'y' and decision != 'n' and decision != 'Y' and decision != 'N')
        {
            input.clear();
            input.ignore(std::numeric_limits<streamsize>::max(), '\n');
            print<<"Invalid decision. Please type y or n:  ";
            input>>decision;
        }
        buffer[0] = tolower(decision);

        if(write(sockfd, buffer, 1) < 0)
            error("ERROR SENDING REMATCH DECISION TO THE HOST");
        
        close(sockfd); // close connection

        if(decision == 'y' or decision == 'Y')
        {
            // client wanted to play, check what the server wants to do
            if(c_libs::read(sockfd, buffer, 1) < 0)
                error("ERROR READING THE SERVER DECISION");
            
            if(buffer[0] == 'E')
            {
                print<<"Server does not want to play another game. Exiting now"<<newline;
                close(sockfd);
                exit(0);
            }
            else
            {
                print<<"Server wants to play another game. Continuing..."<<newline;
                close(sockfd);
            }
        }
    }

}

#endif