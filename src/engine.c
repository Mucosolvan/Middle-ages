#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct Piece Piece;
typedef struct PieceList PieceList;
typedef enum PieceType PieceType;

enum PieceType {PEASANT = 1, KING, KNIGHT};

//TODO Conditions in functions

/**
 * Piece knows its coordinates (x - column number, y - row number), type,
 * the last turn it moved and the player it belongs to.
 */
struct Piece {
    int x;
    int y;
    int lastMove;
    int player;
    PieceType type;
};

/**
 * List of pieces.
 */
struct PieceList {
    Piece* piece;
    PieceList* next;
};

/**
 * Global variables.
 *
 * playerNumber - Number of the player that moves now:
 * 0 - first player, 1 - second player.
 *
 * turnNumber - Current turn number.
 * 
 * turnLimit - Maximum number of turns.
 * 
 * initNumber - Number of initializations that have been done.
 *
 * kingX1, kingY1 - Coordinates of first player's king at the beginning.
 * 
 * kingX2, kingY2 - Coordinates of second player's king at the beginning.
 *
 * topLeft[i][j] - What piece is in i-th column and j-th row;
 * null if no piece is in i-th column and j-th row.

 * pieces[i] - List of pieces of i-th player.
 */
 
int boardSize, turnNumber = 1, turnLimit, playerNumber = 0, initNumber;
int kingX1, kingY1, kingX2, kingY2;
Piece* topLeft[15][15];
PieceList* pieces[3];

/**
 * Check if position (x,y) is valid (on the board).
 * @param[in] x Column number.
 * @param[in] y Row number.
 */
bool validPosition(int x, int y) {
	return (x >= 1) && (x <= boardSize) && (y >= 1) && (y <= boardSize);
}

/**
 * Check if position of piece is equal to (x,y).
 * @param[in] x Column number.
 * @param[in] y Row number.
 * @param[in] piece Piece to check.
 */
bool equalPositions(int x, int y, Piece* piece) {
	return (piece->x == x) && (piece->y == y);
}

/**
 * Check if piece can move in this turn.
 * @param[in] piece Piece to check.
 */
bool canMove(Piece* piece) {
	return piece->lastMove != turnNumber;
}

/**
 * Check if piece can produce another piece in this turn.
 * @param[in] piece Piece to check.
 */
bool canProduce(Piece* piece) {
	return (piece->type == PEASANT) && (piece->lastMove <= turnNumber - 3);
}

/**
 * Check if two pieces are equal.
 * @param[in] piece1 First piece.
 * @param[in] piece2 Second piece.
 */
bool equalPieces(Piece* piece1, Piece* piece2) {
	return equalPositions(piece2->x, piece2->y, piece1) &&
	(piece2->type == piece1->type);
}

void freePieceList(PieceList* pieceList) {
	if (pieceList != NULL) {
		freePieceList(pieceList->next);
		if (pieceList->piece != NULL)
			free(pieceList->piece);
		free(pieceList);		
	}
}

void end_game() {
    freePieceList(pieces[0]);
    freePieceList(pieces[1]);
}

/**
 * Create a piece.
 * @param[in] x Column number.
 * @param[in] y Row number.
 * @param[in] type Type of piece.
 * @param[in] player Player who owns the piece.
 * @param[in] turn The last turn piece moved.
 */
Piece* createPiece(int x, int y, PieceType type, int player, int turn){
    Piece* piece = malloc(sizeof(Piece));
    piece->x = x;
    piece->y = y;
    piece->type = type;
    piece->player = player;
    piece->lastMove = turn - 1;
    return piece;
}

/**
 * Create a piece node to add to a list.
 * @param[in] piece Piece that the pointer on a list will point to.
 */
PieceList* createPieceNode(Piece* piece){
    PieceList* pieceNode = malloc(sizeof(PieceList));
    pieceNode->piece = piece;
    pieceNode->next = NULL;
    return pieceNode;
}

/**
 * Add piece at the end of the list.
 * @param[in] piece Piece to add.
 * @param[in] pieceList Pointer pointing at pointer to beginning of the list.
 */
void addPiece(Piece* piece, PieceList** pieceList){
    PieceList* list = *pieceList;
    PieceList* pieceNode = createPieceNode(piece);
    if (list == NULL) {
        *pieceList = pieceNode;
    }
    else {
        while (list->next != NULL) {
            list = list->next;
        }
        list->next = pieceNode;
    }
}

/**
 * Remove piece from the list.
 * @param[in] piece Piece to remove.
 * @param[in] pieceList Pointer pointing at pointer to beginning of the list.
 */
void removePiece(Piece* piece, PieceList** pieceList) {
	PieceList* list = *pieceList;
	if (equalPieces(piece, list->piece)) {
		*pieceList = list->next;
		free(list->piece);
		free(list);
	}
	else {
		while (list->next != NULL) {
			if (equalPieces(piece, list->next->piece)) {
				PieceList* nextPieceNode;
				if (list->next->next == NULL)
					nextPieceNode = NULL;
				else
					nextPieceNode = list->next->next;
				free(list->next->piece);
				free(list->next);
				list->next = nextPieceNode;
				break;
			}
			list = list->next;
		}
	}
}

/**
 * Create lists of pieces on the beginning of the game.
 * @param[in] x1 First player's king column number.
 * @param[in] y1 First player's king row number.
 * @param[in] x2 Second player's king column number.
 * @param[in] y2 Second player's king row number.
 */
void createPieceLists(int x1, int y1, int x2, int y2){
    Piece* king1 = createPiece(x1, y1, KING, 0, 1);
    Piece* peasant1 = createPiece(x1 + 1, y1, PEASANT, 0, 1);
    Piece* knight1 = createPiece(x1 + 2, y1, KNIGHT, 0, 1);
    Piece* knight2 = createPiece(x1 + 3, y1, KNIGHT, 0, 1);

    addPiece(king1, &pieces[0]);
    addPiece(peasant1, &pieces[0]);
    addPiece(knight1, &pieces[0]);
    addPiece(knight2, &pieces[0]);

    Piece* king2 = createPiece(x2, y2, KING, 0, 1);
    Piece* peasant2 = createPiece(x2 + 1, y2, PEASANT, 0, 1);
    Piece* knight3 = createPiece(x2 + 2, y2, KNIGHT, 0, 1);
    Piece* knight4 = createPiece(x2 + 3, y2, KNIGHT, 0, 1);

    addPiece(king2, &pieces[1]);
    addPiece(peasant2, &pieces[1]);
    addPiece(knight3, &pieces[1]);
    addPiece(knight4, &pieces[1]);
}

/**
 * Check if parametres given in both inits are equal.
 * @param[in] n - Board size.
 * @param[in] k - Number of turns.
 * @param[in] x1 - First player's king column number.
 * @param[in] y1 - First player's king row number.
 * @param[in] x2 - Second player's king column number.
 * @param[in] y2 - Second player's king row number.
 */
bool checkEqualInits(int n, int k, int x1, int y1, int x2, int y2) {
	bool result = (boardSize == n) && (turnLimit == k);
	result = result && (x1 == kingX1) && (x2 == kingX2);
	result = result && (y1 == kingY1) && (y2 == kingY2);
	return result;
}

/**
 * Initialize a game.
 * @param[in] n Board size.
 * @param[in] k Number of turns.
 * @param[in] x1 First player's king column number.
 * @param[in] y1 First player's king row number.
 * @param[in] x2 Second player's king column number.
 * @param[in] y2 Second player's king row number.
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2) { 
    if (initNumber > 2)
        return 42;
		
    if (initNumber == 0) {
        boardSize = n;
        turnLimit = k;
        kingX1 = x1;
        kingX2 = x2;
        kingY1 = y1;
        kingY2 = y2;
		
		if (!validPosition(x1, y1) || !validPosition(x1 + 3, y1))
			return 42;
		if (!validPosition(x2, y2) || !validPosition(x2 + 3, y2))
			return 42;
        createPieceLists(x1, y1, x2, y2);
		initNumber++;
        return 0;
    }
    else {
		initNumber++;
		if (!validPosition(x1, y1) || !validPosition(x1 + 3, y1))
			return 42;
		if (!validPosition(x2, y2) || !validPosition(x2 + 3, y2))
			return 42;
		if (checkEqualInits(n, k, x1, y1, x2, y2))
			return 0;
		else
			return 42;
    }
}

/**
 * Check if there exists a piece on position (x,y) on a list.
 * @param[in] x Column number.
 * @param[in] y Row number.
 * @param[in] list List of pieces.
 */
Piece* pieceExists(int x, int y, PieceList* list) {	
	while (list != NULL) {
		if (equalPositions(x, y, list->piece))
			return list->piece;
		list = list->next;
	}
	return NULL;
}

/**
 * Check whether move from position (x1,y1) to (x2,y2) is valid.
 * @param[in] x1 First column number.
 * @param[in] y1 First row number.
 * @param[in] x2 Second column number.
 * @param[in] y2 Second row number.
 */
bool validMove(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2) == 1;
}

/**
 * Decides which piece wins the fight.
 * @param[in] piece1 First piece.
 * @param[in] piece2 Second piece.
 * @return 2 Pieces have the same type.
 * @return 1 Second piece wins.
 * @return 0 First piece wins.
 */
int pieceFight(Piece* piece1, Piece* piece2) {
	if (piece1->type == piece2->type)
		return 2;
	return piece1->type < piece2->type;
}

bool checkMove
/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return 42 Move is incorrect or done before second init.
 * @return 1 By doing this move first player won.
 * @return 2 By doing this move second player won.
 * @return -1 By doing this move players drew.
 * @return 0 Otherwise.
 */
int move(int x1, int y1, int x2, int y2) {
	Piece* piece = pieceExists(x1, y1, pieces[playerNumber]);
	int secondPlayer = (playerNumber + 1) % 2;
	Piece* piece2 = pieceExists(x2, y2, pieces[secondPlayer]);
	bool conditions = initNumber == 2;
	conditions = conditions && piece != NULL && canMove(piece);
	conditions = conditions && pieceExists(x2, y2, pieces[playerNumber]) == NULL;
	conditions = conditions && validPosition(x2, y2);
	conditions = conditions && validPosition(x1, y1);
	conditions = conditions && validMove(x1, y1, x2, y2);
	if (!conditions)
		return 42;
	if (piece2 == NULL) {
		piece->x = x2;
		piece->y = y2;
		piece->lastMove = turnNumber;
	}
	else {
		int loser = pieceFight(piece, piece2);
		if (loser == 2) {
			if (piece->type == KING)
				return -1;
			removePiece(piece, &pieces[playerNumber]);
			removePiece(piece2, &pieces[secondPlayer]);
		}
		else {
			if (loser == 1) {
				if (piece->type == KING)
					return 2;
				removePiece(piece, &pieces[playerNumber]);
			}
			else {
				if (piece2->type == KING)
					return 1;
				removePiece(piece2, &pieces[secondPlayer]);
			}
		}
	}
    return 0;
}

/**
 * Produces a knight.
 * @param[in] x1 Peasant's column number.
 * @param[in] y1 Peasant's row number.
 * @param[in] x2 New knight's column number.
 * @param[in] y2 New knight's row number.
 * @return 42 Move is incorrect or done before second init.
 * @return 0 Otherwise.
 */
int produce_knight(int x1, int y1, int x2, int y2) {
	Piece* piece = pieceExists(x1, y1, pieces[playerNumber]);
	int secondPlayer = (playerNumber + 1) % 2;
	bool conditions = initNumber == 2;
	conditions = conditions && piece != NULL && canProduce(piece);
	conditions = conditions && pieceExists(x2, y2, pieces[playerNumber]) == NULL;
	conditions = conditions && pieceExists(x2, y2, pieces[secondPlayer]) == NULL;
	conditions = conditions && validPosition(x2, y2);
	conditions = conditions && validPosition(x1, y1);
	conditions = conditions && validMove(x1, y1, x2, y2);
	if (!conditions)
		return 42;
	Piece* newPiece = createPiece(x2, y2, KNIGHT, playerNumber, turnNumber);
	addPiece(newPiece, &pieces[playerNumber]);
    return 0;
}

/**
 * Produces a peasant.
 * @param[in] x1 Peasant's column number.
 * @param[in] y1 Peasant's row number.
 * @param[in] x2 New peasant's column number.
 * @param[in] y2 New peasant's row number.
 * @return 42 Move is incorrect or done before second init.
 * @return 0 Otherwise.
 */
int produce_peasant(int x1, int y1, int x2, int y2) {
	Piece* piece = pieceExists(x1, y1, pieces[playerNumber]);
	int secondPlayer = (playerNumber + 1) % 2;
	bool conditions = initNumber == 2;
	conditions = conditions && piece != NULL && canProduce(piece);
	conditions = conditions && pieceExists(x2, y2, pieces[playerNumber]) == NULL;
	conditions = conditions && pieceExists(x2, y2, pieces[secondPlayer]) == NULL;
	conditions = conditions && validPosition(x2, y2);
	conditions = conditions && validPosition(x1, y1);
	conditions = conditions && validMove(x1, y1, x2, y2);
	if (!conditions)
		return 42;
	Piece* newPiece = createPiece(x2, y2, PEASANT, playerNumber, turnNumber);
	addPiece(newPiece, &pieces[playerNumber]);
    return 0;
}

/**
 * Ends turn of current player and starts other player's turn.
 * @return 42 Command done before second init.
 * @return 1 Turn limit has been achieved.
 * @return 0 Otherwise.
 */ 
int end_turn() {
	if (initNumber != 2)
		return 42;
	playerNumber++;
	playerNumber %= 2;
	if (playerNumber == 0)
		turnNumber++;
	if (turnNumber > turnLimit)
		return 1;
    return 0;
}

void printList(PieceList* list) {
	while (list != NULL) {
		Piece* piece = list->piece;
		printf("%d %d %d\n",piece->x, piece->y, piece->type);
		list = list->next;
	}
}

int main(){
    init(15, 5, 1, 2, 1, 1, 2);
    init(15, 5, 2, 2, 1, 1, 2);
    printList(pieces[0]);
	printf("%d\n",move(2,1,2,2));
	end_turn();
	//printf("%d\n",move(4,15,4,16));
    printList(pieces[1]);
    end_game();
    return 0;
}
