const int ROW = 6;
const int COLUMN = 7;

using namespace std;

struct currentState{
	char state[ROW][COLUMN];
};

/*
This method scores each state of the board. It checks around each empty space in the matrix for a computer
piece or a human piece. The score is decremented if a human piece is encountered and incremented if
a computer piece is encountered. A positive score means the computer has the advantage and a negative score
means the human has the advantage. A score of zero means the human and computer are even.
*/
int boardScore(char piecePlacement[ROW][COLUMN]) {
	int countScore = 0;

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			
			//Only consider empty spaces in the matrix
			if (piecePlacement[i][j] == 'e') {
				
				//Only count 3 spaces since 4 is a win condition and is already handled by the checkForWin method
				for(int k = 1; k <= 3; k++){
					
					//Check to the right of the empty space
					if(piecePlacement[i][j + k] == 'H' && piecePlacement[j + 1][k] != 'e'){
						countScore--;
					}
					if(piecePlacement[i][j + k] == 'C' && piecePlacement[j + 1][k] != 'e'){
						countScore++;
					}
					
					//Check to the left of the empty space
					if(piecePlacement[i][j - k] == 'H' && piecePlacement[j + 1][k] != 'e'){
						countScore--;
					}
					if(piecePlacement[i][j - k] == 'H' && piecePlacement[j + 1][k] != 'e'){
						countScore++;
					}
					
					//Check below the empty space
					if(piecePlacement[i + k][j] == 'H'){
						countScore--;
					}
					if(piecePlacement[i + k][j] == 'C'){
						countScore++;
					}
					
					//Check above the empty space
					if(piecePlacement[i - k][j] == 'H'){
						countScore--;
					}
					if(piecePlacement[i - k][j] == 'C'){
						countScore++;
					}
					
					//Check to the lower right of the empty space
					if(piecePlacement[i + k][j + k] == 'H' && piecePlacement[j + 1][k] != 'e'){
						countScore--;
					}
					if(piecePlacement[i + k][j + k] == 'C' && piecePlacement[j + 1][k] != 'e'){
						countScore++;
					}
					
					//Check to the upper right of the empty space
					if(piecePlacement[i - k][j + k] == 'H' && piecePlacement[j + 1][k] != 'e'){
						countScore--;
					}
					if(piecePlacement[i - k][j + k] == 'C' && piecePlacement[j + 1][k] != 'e'){
						countScore++;
					}
					
					//Check to the lower left of the empty space
					if(piecePlacement[i + k][j - k] == 'H' && piecePlacement[j + 1][k] != 'e'){
						countScore--;
					}
					if(piecePlacement[i + k][j - k] == 'C' && piecePlacement[j + 1][k] != 'e'){
						countScore++;
					}
					
					//Check to the upper left of the empty space
					if(piecePlacement[i - k][j - k] == 'H' && piecePlacement[j + 1][k] != 'e'){
						countScore--;
					}
					if(piecePlacement[i - k][j - k] == 'C' && piecePlacement[j + 1][k] != 'e'){
						countScore++;
					}
				}
			}
		}
	}

	return countScore;
}

/*
This method checks to see if a column in the matrix is full or not.
*/
bool isColumnFull(int col, char node[ROW][COLUMN]){
	bool isFull = true;

	for(int i = 5; i >= 0; i--){
		if(node[i][col] == 'e'){
			isFull = false;
			break;
		}
	}
	
	return isFull;
}

/*
This method is a special version of placeHumanPiece/placeComputerPiece that is only used by the minimax algorithm.
It takes in a child node and places a piece in the appropriate column. If it is the maxPlayer's turn, a computer
piece is placed. If not, a human piece is placed.
*/
void specialInsertion(char node[ROW][COLUMN], int choice, bool maxPlayer){
	if(maxPlayer){
		for(int i = 5; i >= 0; i--){
			if(node[i][choice] == 'e'){
				node[i][choice] = 'C';
				break;
			}
		}
	}
	else{
		for(int i = 5; i >= 0; i--){
			if(node[i][choice] == 'e'){
				node[i][choice] = 'H';
				break;
			}
		}
	}
}

/*
This method takes in a node, expands it to create children, and stores those children in an array.
*/
void fillChildren(currentState children[7], char node[ROW][COLUMN], bool maxPlayer){
	char temp[ROW][COLUMN];
	
	for(int i = 0; i < 7; i++){
		if(!isColumnFull(i, children[i].state)){ //Don't create child if the column is full.
			for(int j = 0; j < ROW; j++){
				for(int k = 0; k < COLUMN; k++){
					temp[j][k] = node[j][k];
				}
			}
			specialInsertion(temp, i, maxPlayer);
			
			for(int j = 0; j < ROW; j++){
				for(int k = 0; k < COLUMN; k++){
					children[i].state[j][k] = temp[j][k];
				}
			}
		}
	}
}

/*
This method checks to see if the either the human or computer has won.

A win occurs when either the human or computer has four consecutive pieces
going up or down, left or right, or in diagonal.
*/
void checkForWin(char pieces[ROW][COLUMN], bool& isWinH, bool& isWinC) {
	const int winCond = 4;

	int countCLR = 0; //Counts computer pieces going left and right
	int countCUD = 0; //Counts computer pieces going up and down
	int countHLR = 0; //Counts human pieces going left and right
	int countHUD = 0; //Counts human pieces going up and down
	int countCDiag1 = 0; //Counts human pieces going from up/left to down/right
	int countHDiag1 = 0; //Counts computer pieces going from up/left to down/right
	int countCDiag2 = 0; //Counts human pieces going from up/right to down/left
	int countHDiag2 = 0; //Counts computer pieces going from up/right to down/left

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
	 	//Begin checking left to right
			if (i < 6) {
				if (pieces[i][j] == 'C') {
					countCLR++;
					if (pieces[i][j + 1] != 'C' && countCLR != winCond) {
						countCLR = 0;
					}
					if (countCLR == winCond) {
						isWinC = true;
						return;
					}
				}
				else if (pieces[i][j] == 'H') {
					countHLR++;
					if (pieces[i][j + 1] != 'H' && countHLR != winCond) {
						countHLR = 0;
					}
					if (countHLR == winCond) {
						isWinH = true;
						return;
					}
				}
			}
			//End checking left to right

			//Begin checking up and down
			if (j < 6) {
				if (pieces[j][i] == 'C') {
					countCUD++;
					if (pieces[j + 1][i] != 'C' && countCUD != winCond) {
						countCUD = 0;
					}
					if (countCUD == winCond) {
						isWinC = true;
						return;
					}
				}
				else if (pieces[j][i] == 'H') {
					countHUD++;
					if (pieces[j + 1][i] != 'H' && countHUD != winCond) {
						countHUD = 0;
					}
					if (countHUD == winCond) {
						isWinH = true;
						return;
					}
				}
			}
			//End checking up and down
		}
		//Reset counters
		countCLR = 0;
		countHLR = 0;
		countCUD = 0;
		countHUD = 0;
	}

	//Begin checking diagonal up/left to down/right
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 7; j++) {
			if (pieces[j + i][j] == 'C') {
				countCDiag1++;
				if (pieces[(j + 1) + i][j + 1] != 'C' && countCDiag1 != winCond) {
					countCDiag1 = 0;
				}
				if (countCDiag1 == winCond) {
					isWinC = true;
					return;
				}
			}
			else if (pieces[j + i][j] == 'H') {
				countHDiag1++;
				if (countHDiag1 == winCond) {
					isWinH = true;
					return;
				}
				if (pieces[(j + 1) + i][j + 1] != 'H' && countHDiag1 != winCond) {
					countHDiag1 = 0;
				}
			}
		}
		//Reset counters
		countCDiag1 = 0;
		countHDiag1 = 0;
	}
	//*********************************************************
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			if (pieces[j][j + i] == 'C') {
				countCDiag1++;
				if (pieces[j + 1][(j + 1) + i] != 'C' && countCDiag1 != winCond) {
					countCDiag1 = 0;
				}
				if (countCDiag1 == winCond) {
					isWinC = true;
					return;
				}
			}
			else if (pieces[j][j + i] == 'H') {
				countHDiag1++;
				if (countHDiag1 == winCond) {
					isWinH = true;
					return;
				}
				if (pieces[j + 1][(j + 1) + i] != 'H' && countHDiag1 != winCond) {
					countHDiag1 = 0;
				}
			}
		}
		//Reset counters
		countCDiag1 = 0;
		countHDiag1 = 0;
	}
	//End checking diagonal up/left to down/right

	//Begin checking diagonal up/right to down/left
	for (int i = 0; i < 3; i++) {
		for (int j = 0, k = 5; j < 6, k >= 0; j++, k--) {
			if (pieces[j][k - i] == 'C') {
				countHDiag2 = 0;
				countCDiag2++;
				if (countCDiag2 == winCond) {
					isWinC = true;
					return;
				}
				else if (pieces[j + 1][(k - 1) - i] != 'C' && countCDiag2 != winCond) {
					countCDiag2 = 0;
				}
			}
			else if (pieces[j][k - i] == 'H') {
				countCDiag2 = 0;
				countHDiag2++;
				if (countHDiag2 == winCond) {
					isWinH = true;
					return;
				}
				else if (pieces[j + 1][(k - 1) - i] != 'H' && countHDiag2 != winCond) {
					countHDiag2 = 0;
				}
			}
		}
		//Reset counters
		countCDiag2 = 0;
		countHDiag2 = 0;
	}
	//*********************************************************
	for (int i = 0; i < 3; i++) {
		for (int j = 0, k = 6; j < 6, k >= 0; j++, k--) {
			if (pieces[j + i][k] == 'C') {
				countHDiag2 = 0;
				countCDiag2++;
				if (countCDiag2 == winCond) {
					isWinC = true;
					return;
				}
				if (pieces[(j + 1) + i][k - 1] != 'C' && countCDiag2 != winCond) {
					countCDiag2 = 0;
				}
			}
			else if (pieces[j + i][k] == 'H') {
				countCDiag2 = 0;
				countHDiag2++;
				if (countHDiag2 == winCond) {
					isWinH = true;
					return;
				}
				else if (pieces[(j + 1) + i][k - 1] != 'H' && countHDiag2 != winCond) {
					countHDiag2 = 0;
				}
			}
		}
		//Reset counters
		countCDiag2 = 0;
		countHDiag2 = 0;
	}
	//End checking diagonal up/right to down/left
}

/*
This method checks to see if the board is full with pieces and no winner has been determined
*/
bool isFull(char pieces[ROW][COLUMN]) {
	int countTotal = 0;
	int fullCond = ROW * COLUMN;

	//This nested loop counts the amount of places on the board that contain a piece
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (pieces[i][j] == 'C' || pieces[i][j] == 'H') {
				countTotal++;
			}
		}
	}
	//There are 42 pieces on the board. If 42 nonempty spaces are counted, a value of true is returned.
	//If there are any less than 42 nonempty pieces, a value of false is returned.
	if (countTotal == fullCond) {
		return true;
	}
	else {
		return false;
	}
}

/*
The main algorithm for the AI.
*/
int miniMax(char node[ROW][COLUMN], int depth, bool maxPlayer, int& compChoice){
	char temp[ROW][COLUMN];
	
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COLUMN; j++){
			temp[i][j] = node[i][j];
		}
	}
	
	bool isWinH = false, isWinC = false;
	checkForWin(temp, isWinH, isWinC);
	if(isWinH){
		return INT_MIN;
	}
	else if(isWinC){
		return INT_MAX;
	}
	else if(depth == 0){
		return boardScore(temp);
	}
	
	currentState children[7];
	//Initialize the children array
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < ROW; j++){
			for(int k = 0; k < COLUMN; k++){
				children[i].state[j][k] = 'e';
			}
		}
	}
	
	if(maxPlayer){
		int bestValue = INT_MIN;
		fillChildren(children, temp, maxPlayer);	
		
		for(int i = 0; i < 7; i++){ //for each child of node
			int v = miniMax(children[i].state, depth - 1, false, compChoice);
			if(v > bestValue){
				bestValue = v;
				compChoice = i;
			}
		}
		return bestValue;
	}
	else{
		int bestValue = INT_MAX;
		fillChildren(children, node, maxPlayer);
		
		for(int i = 0; i < 7; i++){ //for each child of node
			int v = miniMax(children[i].state, depth - 1, true, compChoice);
			if(v < bestValue){
				bestValue = v;
				compChoice = i;
			}
		}
		return bestValue;
	}
}
