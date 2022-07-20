/* Author: Nathan Burrows
 * File: KnightTourText.cpp
 *
 * This program attempts an open Knight Tour using Warnsdorff's algorithm.
 * The user specifies the board size (can be square or rectangular) and chooses the starting square for the knight.
 * The program then computes a valid Knight Tour (if it exists) for that board size, and outputs a console display
 * at each stage, for every move.
 */

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <limits>


/* Function: printBoard()
 * @desc: Prints the chessboard (which is a 2d vector int array) to the console. Since the vector is an int vector
 *        internally (more efficient), we need to 'act' like it was really a string array, which is what we want since
 *        it allows for easy understanding.
 *        Internally: 1 = The Knight is currently at this square, so display [K]
 *                    2 = The Knight has previously been to this square, and cannot return, so display [/]
 *                    0/default = The Knight has not visited this square, so display [ ]
 *
 * @param Board: Passing the chessboard into the function by reference.
 */
void printBoard(std::vector<std::vector<int>>& Board) {
    for (int i = 0; i < Board.size(); i++) {
        for (int j = 0; j < Board[i].size(); j++) {
            switch(Board[i][j]) {
                case 1:
                    std::cout << "[K]";
                    break;
                case 2:
                    std::cout << "[/]";
                    break;
                default:
                    std::cout << "[ ]";
                    break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*
 * Function: findMinimumIndex()
 * @desc: Takes in a vector of integers, and finds the index of the smallest one.
 * @param: Passing the vector into the function.
 * @return: Returns the smallest index.
 */
int findMinimumIndex(std::vector<int> vec) {
    int index = 0;
    int smallest = vec[0];
    for (int i = 1; i < vec.size(); i++) {
        if (vec[i] < smallest) {
            smallest = vec[i];
            index = i;
        }
    }
    return index;
}

/*
 * Function: isOnBoard()
 * @desc: A helper function that finds if an X,Y co-ord is a move that will be on the chessboard.
 * @param1/param2: X/Y co-ords of a potential move
 * @param3/param4: Board dimensions (X/Y)
 * @return: Returns true if move fits on the chessboard, false if not.
 */
bool isOnBoard(int x, int y, int boardX, int boardY) {
    return ((x >= 0) && (boardX > x) && (y >= 0) && (boardY > y));
}

/*
 * Function: findMovesFromSquare()
 * @desc: This function takes in a square location, and finds all potential legal moves for the knight if it was placed there.
 * @param1/param2: X/Y co-ords of the knight's location
 * @param3/param4: Board dimensions (X/Y)
 * @param5: The chessboard, passed by reference
 * @return: Returns a vector array of int pairs. These are all the knights legal next moves (as X/Y co-ords)
 *
 */
std::vector<std::pair<int,int>> findMovesFromSquare(int x, int y, int boardX, int boardY, std::vector<std::vector<int>>& Board) {
    std::vector<std::pair<int,int>> moveArray;
    int dx[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int dy[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    for (int i = 0; i < 8; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        if (isOnBoard(newX, newY, boardX, boardY) && Board[newX][newY] != 2) {
            moveArray.push_back(std::pair<int, int>(newX, newY));
        }
    }
    return moveArray;
}


/*
 * Function: makeMove()
 * @desc: This is the central function that calls itself recursively, which moves the knight around the chessboard.
 *        It finds the best legal move. This is the move that has the fewest amount of continuing moves. It calls
 *        findMovesFromSquare() twice, to get the inital and continuing moves, and finds the square with the least
 *        amount, by storing them all and sorting their continuing move sizes in descending order. Once it has the
 *        best move, it sets the current board square to visited (2), and the next board square to the current one (1).
 *        It will keep calling itself while it still has legal moves to make.
 *
 * @param1: This is the Board itself
 * @param2/param3: The boards X/Y dimensions
 * @param4/param5: The current Knight's X/Y co-ords
 * @param6: The amount of moves made, passed by reference from main(). This is used to deduce the final text output.
 */
void makeMove(std::vector<std::vector<int>> Board, int boardX, int boardY, int knightRow, int knightCol, int& movesMade) {
    std::vector<int> sizes;
    std::vector<std::pair<int, int>> array = findMovesFromSquare(knightRow, knightCol, boardX, boardY, Board);
    //while there are moves, loop:
    if (!array.empty()) {
        //get the amount of continuing moves for each legal move
        for (std::pair<int, int> p : array) {
            sizes.push_back(findMovesFromSquare(p.first, p.second, boardX, boardY, Board).size());
        }
        //find the one with the fewest
        int index = findMinimumIndex(sizes);
        std::pair<int, int> nextMove = array[index];
        //set up a knight move to the new square, then call this function again
        Board[knightRow][knightCol] = 2;
        Board[nextMove.first][nextMove.second] = 1;
        knightRow = nextMove.first;
        knightCol = nextMove.second;
        printBoard(Board);
        movesMade++;
        makeMove(Board, boardX, boardY, knightRow, knightCol, movesMade);

    }
}

/*
 * Function: inputInteger()
 * @desc: A small helper function that gets an integer from the command line. It will repeat until iit finds an
 *        integer value that is between the bounds specified (inclusive).
 * @param1/param2: lower/upper integer bounds
 * @param3: The text displayed to the user to inform what input should be given, passed by reference.
 * @return: Returns the integer inputted by the user.
 */
int inputInteger(int lowerBound, int upperBound, const std::string& text) {
    int i;
    while (true) {
        std::cout << text;
        if ((std::cin >> i)&&(i >= lowerBound)&&(i <= upperBound)) break;  //if an integer, and within size boundaries, then accept input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore non-integer inputs
    }
    return i;
}
/*
 * Function: getPairFromUser()
 * @desc: Gets an integer pair from console input. This calls inputInteger() twice to get an X/Y pair from the user. This
 *        is in a function since it is repeated multiple times, like getting the X/Y co-ords of the Knight's starting
 *        square, as well as the board size.
 * @param1/param2: Upper and lower bounds to be specified for integer1 (inclusive)
 * @param3/param4: Upper and lower bounds to be specified for integer2 (inclusive)
 * @param5/param6: Text displayed to the user to guide their input for integer1 and integer2, passed by reference
 * @param7: An optional offset (integer). This is so a user can input '1' for the knight's row, and this will actually be
 *          changed to 0 to be used in the internal board array since it is indexed from 0. To achieve this, use an offset
 *          of 1 which will be subtracted from the integer after input.
 * @return: An integer pair from the user.
 */
std::pair<int,int> getPairFromUser(int lowerBound, int upperBound, int lowerBound2, int upperBound2, const std::string& text, const std::string& text2, int offset) {
    int x = inputInteger(lowerBound, upperBound, text) - offset;
    int y = inputInteger(lowerBound2, upperBound2, text2) - offset;
    return std::pair<int,int>(x,y);

}


//This is the main method. First a string description of the program is printed, then the user inputs a board size (X/Y), then the Knight's starting square.
//The board is initialised internally as a vector array with 0s. The knight is placed on a starting square and this board state is printed. makeMove() is
//then called, which calls itself while there are still valid moves to make. Once there are no more moves, based on the number of moves successfully made,
//a final text output showing the result of the tour is printed.
int main() {
    std::cout << "This program attempts an open Knight Tour using Warnsdorff's algorithm. Please specify square/rectangular board dimensions, and the Knight's starting square." << std::endl;
    std::pair<int,int> boardSize = getPairFromUser(3,10,3,10,"Enter number of rows (between 3-10):","Enter number of columns (between 3-10):", 0);
    std::pair<int,int> start = getPairFromUser(1,boardSize.first, 1, boardSize.second, "Enter starting row of knight:", "Enter starting column of knight:", 1);

    //initialise board with 0s
    std::vector<std::vector<int>> Board(boardSize.first, std::vector<int>(boardSize.second));

    //place knight on starting square
    Board[start.first][start.second] = 1;
    printBoard(Board);

    int movesMade = 0;
    makeMove(Board, boardSize.first, boardSize.second, start.first, start.second, movesMade);
    if (movesMade == (boardSize.first * boardSize.second) - 1) {
        std::cout << "Tour Completed!" << std::endl;
    }
    else {
        std::cout << "No More Moves!" << std::endl;
    }
    return 0;
}
