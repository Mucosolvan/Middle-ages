#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct Piece Piece;
typedef struct PieceList PieceList;
typedef enum PieceType PieceType;

enum PieceType {PEASANT = 1, KING, KNIGHT};


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
 * kingX1, kingY1 - coordinates of first player's king on the beginning.
 * 
 * kingX2, kingY2 - coordinates of second player's king on the beginning.
 *
 * topLeft[i][j] - What piece is in i-th column and j-th row;
 * null if no piece is in i-th column and j-th row.

 * pieces[i] - List of pieces of i-th player.
 */
 
int boardSize, turnNumber = 1, turnLimit, playerNumber = 1, initNumber;
int kingX1, kingY1, kingX2, kingY2;
Piece* topLeft[15][15];
PieceList* pieces[3];

/**
 * Check if position (x,y) is valid.
 */
bool validPosition(int x, int y) {
	return (x >= 1) && (x <= boardSize) && (y >= 1) && (y <= boardSize);
}

/**
 * Check if position of piece is equal to (x,y).
 */
bool equalPositions(int x, int y, Piece* piece) {
	return (piece->x == x) && (piece->y == y);
}

/**
 * Check if piece can move in this turn.
 */
bool canMove(Piece* piece) {
	return piece->lastMove != turnNumber;
}

/**
 * Check if piece can produce another piece in this turn.
 */
bool canProduce(Piece* piece) {
	return (piece->type == PEASANT) && (piece->lastMove <= turnNumber - 3);
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

Piece* createPiece(int x, int y, PieceType type, int player, int turn){
    Piece* piece = malloc(sizeof(Piece));
    piece->x = x;
    piece->y = y;
    piece->type = type;
    piece->player = player;
    piece->lastMove = turn - 1;
    return piece;
}

PieceList* createPieceNode(Piece* piece){
    PieceList* pieceNode = malloc(sizeof(PieceList));
    pieceNode->piece = piece;
    pieceNode->next = NULL;
    return pieceNode;
}

/**
 * Add piece at the end of the list.
 * @param[in] piece Piece to add.
 * @param[in] pieceList Pointer to the beginning of the list.
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
 * Create lists of pieces on the beginning of the game.
 * @param[in] x1 - Column number of the first player's king.
 * @param[in] y1 - Row number of the first player's king.
 * @param[in] x2 - Column number of the second player's king.
 * @param[in] y2 - Row number of the second player's king.
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
 */
bool checkEqualInits(int n, int k, int x1, int y1, int x2, int y2) {
	bool result = (boardSize == n) && (turnLimit == k);
	result = result && (x1 == kingX1) && (x2 == kingX2);
	result = result && (y1 == kingY1) && (y2 == kingY2);
	return result;
}

int init(int n, int k, int p, int x1, int y1, int x2, int y2) { 
    if (initNumber > 2)
        return 42;
	if (!validPosition(x1, y1) || !validPosition(x1 + 3, y1))
		return 42;
	if (!validPosition(x2, y2) || !validPosition(x2 + 3, y2))
		return 42;
		
    if (initNumber == 0) {
        boardSize = n;
        turnLimit = k;
        kingX1 = x1;
        kingX2 = x2;
        kingY1 = y1;
        kingY2 = y2;

        createPieceLists(x1, y1, x2, y2);
		initNumber++;
        return 0;
    }
    else {
		initNumber++;
		if (checkEqualInits(n, k, x1, y1, x2, y2))
			return 0;
		else
			return 42;
    }
}

Piece* pieceExists(int x, int y, PieceList* list) {	
	while (list != NULL) {
		if (equalPositions(x, y, list->piece))
			return list->piece;
		list = list->next;
	}
	return NULL;
}

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

int move(int x1, int y1, int x2, int y2) {
	Piece* piece = pieceExists(x1, y1, pieces[playerNumber]);
	int secondPlayer = (playerNumber + 1) % 2;
	Piece* piece2 = pieceExists(x2, y2, pieces[secondPlayer]);
	bool conditions = initNumber == 2;
	conditions = conditions && piece != NULL && canMove(piece);
	conditions = conditions && piece2 == NULL;
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
	}
    return 0;
}

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

int produce_peasant(int x1, int y1, int x2, int y2) {
	Piece* piece = pieceExists(x1, y1, pieces[playerNumber]);
	int secondPlayer = (playerNumber + 1) % 2;
	bool conditions = initNumber == 2;
	conditions = conditions && piece != NULL && canProduce(piece);
	conditions = conditions && pieceExists(x2, y2, pieces[playerNumber]) == NULL;
	conditions = conditions && pieceExists(x2, y2, pieces[secondPlayer]) == NULL;
	conditions = conditions && validMove(x1, y1, x2, y2);
	if (!conditions)
		return 42;
	Piece* newPiece = createPiece(x2, y2, PEASANT, playerNumber, turnNumber);
	addPiece(newPiece, &pieces[playerNumber]);
    return 0;
}

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

int main(){
    init(15, 5, 1, 1, 1, 1, 10);
    if (pieceExists(2, 10, pieces[1]) != NULL)
		printf("JEJ");
    end_game();
    return 0;
}
