/******************************************************************/
/*                                                                */
/*    CS 470							  */
/*    Spring Semester, 2018                                       */
/*                                                                */
/*    Term Project: Connect Four with AI	                  */
/*    This program simulates a game of Connect Four. It starts	  */
/*	  with a human move, then alternates between computer and */
/*	  human. User input is used for the human choice and the  */
/*	  minimax AI algorithm is used for the computer choice.	  */
/*                                                    	          */
/*    Programmed by: Dylan Mays                                   */
/*                                                                */
/*    Due: Monday April 30, 2018 	                          */
/*                                                                */
/******************************************************************/

#include <iostream>
#include <ctime>
#include "currentState.h"

/*
This method prints the board with correctly placed pieces. When an 'e' is encountered,
an empty space is printed. Otherwise the appropriate 'H' or 'C' character is printed
along with bars and underscores to simulate the appearance of a Connect Four board.
*/
void printPieces(char pieces[ROW][COLUMN]) {
	cout << "**************************************" << endl;
	cout << "\t Connect Four" << endl;
	cout << "**************************************" << endl;
	cout << " 1  2  3  4  5  6  7" << endl;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (pieces[i][j] == 'e') {
				cout << " ";
			}
			else {
				cout << pieces[i][j];
			}
			cout << " |";
		}
		cout << endl;
		for (int k = 0; k < COLUMN; k++) {
			cout << "__ ";
		}
		cout << endl;
	}
}

/*
This method is used for when it is the human's turn to place a piece on the board.
A column is chosen, and an 'H' character is placed in the lowest empty space in the chosen column.
*/
void placeHumanPiece(char pieces[ROW][COLUMN]) {
	int choiceH;
	int count = 0;

	while (count == 0) {
		cout << "Choose column 1, 2, 3, 4, 5, 6, or 7 to place a piece." << endl;
		cin >> choiceH;
		while ((choiceH - 1) < 0 || (choiceH - 1) > 6) {
			cout << "Invalid choice! Try again!" << endl;
			cin >> choiceH;
			//Check if user inputs a string
			cin.clear();
			cin.ignore(10000, '\n');
		}
		//Check if column is already full
		//If the column is full, the user chooses another
		for (int i = ROW; i >= 0; i--) {
			if (pieces[i][choiceH - 1] == 'e') {
				count++;
			}
		}
	}

	//Loop to place piece in chosen column
	for (int i = 5; i >= 0; i--) {
		if (pieces[i][choiceH - 1] == 'e') {
			pieces[i][choiceH - 1] = 'H';
			break;
		}
	}
}

/*
This method is used for when it is the computer's turn to place a piece on the board.
The choice of a column is passed in, and a 'C' character is placed in the lowest empty space in the chosen column.
*/
void placeComputerPiece(char pieces[ROW][COLUMN], int choice){
	while(isColumnFull(choice, pieces)){
		choice++;
		if(choice > COLUMN - 1){
			choice = 0;
		}
	}
	
	for(int i = 5; i >= 0; i--){
		if(pieces[i][choice] == 'e'){
			pieces[i][choice] = 'C';
			break;
		}
	}
}

/*
Main execution of program
*/
int main() {
	bool isWinH = false; //Checks for a human win
	bool isWinC = false; //Checks for a computer win

	//2D character array representing the placement of pieces on the board
	//All values are initialized to empty, represented by a lowercase 'e'
	char piecePlacement[ROW][COLUMN] = {
		{ 'e','e','e','e','e','e','e' },
		{ 'e','e','e','e','e','e','e' },
		{ 'e','e','e','e','e','e','e' },
		{ 'e','e','e','e','e','e','e' },
		{ 'e','e','e','e','e','e','e' },
		{ 'e','e','e','e','e','e','e' }
	};
	
	int difficulty, choice;
	cout << "Choose a difficulty level:" << endl;
	cout << "*******************************************************" << endl;
	cout << "1: Too Easy (Depth = 1)" << endl;
	cout << "2: Easy (Depth = 4)" << endl;
	cout << "3: Normal (Depth = 6)" << endl;
	cout << "4: Hard (Depth = 8)" << endl;
	cout << "Any other key: Back down like a coward" << endl;
	cout << "*******************************************************" << endl;
	cin >> choice;
	
	switch(choice){
		case 1: difficulty = 1; break;
		case 2: difficulty = 4; break;
		case 3: difficulty = 6; break;
		case 4: difficulty = 8; break;
		default:{
			system("cls");
			cout << "You suck!" << endl;
			system("Pause");
			exit(0);
		} 
	}

	printPieces(piecePlacement);
	
	//This loop continues until a win condition has been met or the board is full with no winner.
	while(true) {
		placeHumanPiece(piecePlacement);
		system("cls");
		
		checkForWin(piecePlacement, isWinH, isWinC);
		if (isWinH) {
			printPieces(piecePlacement);
			cout << "You win!" << endl;
			break;
		}
		else if (isWinC) {
			printPieces(piecePlacement);
			cout << "The computer wins!" << endl;
			break;
		}
		else if (isFull(piecePlacement)) {
			printPieces(piecePlacement);
			cout << "Nobody wins" << endl;
			break;
		}
		
		int compChoice;
		cout << "The computer is thinking..." << endl;
		
		clock_t begin = clock();
		miniMax(piecePlacement, difficulty, true, compChoice);
		clock_t end = clock();
		
		double elapsedTime = double(end - begin) / CLOCKS_PER_SEC;		
		cout << "... and is now done thinking after " << elapsedTime << " seconds." << endl;
		placeComputerPiece(piecePlacement, compChoice);
		
		checkForWin(piecePlacement, isWinH, isWinC);
		if (isWinH) {
			printPieces(piecePlacement);
			cout << "You win!" << endl;
			break;
		}
		else if (isWinC) {
			printPieces(piecePlacement);
			cout << "The computer wins!" << endl;
			break;
		}
		else if (isFull(piecePlacement)) {
			printPieces(piecePlacement);
			cout << "Nobody wins" << endl;
			break;
		}
		
		printPieces(piecePlacement);
		cout << compChoice + 1 << endl;		
	}

	system("Pause");
	return 0;
}
