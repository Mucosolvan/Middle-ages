#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cmocka.h"
#include "engine.h"

extern void addPiece(Piece* piece, PieceList** pieceList);
extern void freePiece(Piece* piece);
extern Piece* createPiece(int x, int y, PieceType type, int player, int turn);
extern void freePieceList(PieceList* pieceList);
extern Piece* pieceExists(int x, int y, PieceList* pieceList);

PieceList* list = NULL;
Piece* pieces[10];

/**
 * Test checks what happens on an empty list.
 */
static void testEmptyList(void **state) {
	Piece* p = pieceExists(1, 1, list);
	assert_null(p);
}

/**
 * Test checks what happens when piece does not exist on the list.
 */
static void testPieceDoesNotExist(void **state) {
	Piece* p = pieceExists(1, 2, list);
	assert_null(p);
}

/**
 * Test checks what happens when piece does not exist on the list.
 */
static void testPieceDoesNotExist2(void **state) {
	Piece* p = pieceExists(2, 9, list);
	assert_null(p);
}

/**
 * Test checking what happens when piece exists.
 */
static void testPieceExists(void **state) {
	Piece* p = pieceExists(1, 1, list);
	assert_non_null(p);
}

/**
 * Test checking what happens when piece exists.
 */
static void testPieceExists2(void **state) {
	Piece* p = pieceExists(1, 1, list);
	assert_non_null(p);
}

/**
 * Test checks what happens when piece exists.
 */
static void testPieceExists3(void **state) {
	Piece* p = pieceExists(900, 125, list);
	assert_non_null(p);
}

/**
 * Test checks what happens when piece exists.
 */
static void testPieceExists4(void **state) {
	Piece* p = pieceExists(3, 4, list);
	assert_non_null(p);
}

/**
 * Test checks what happens when piece exists.
 */
static void testPieceExists5(void **state) {
	Piece* p = pieceExists(10, 17, list);
	assert_non_null(p);
}

extern int middle_ages_main(int argc, char *argv[]);

static int teardownP(void **state) {
	freePieceList(list);
	list = NULL;
	return 0;
}

static int setup_add_one(void **state) {
	addPiece(createPiece(1, 1, KNIGHT, 1, 1), &list);
	return 0;
}

static int setup_add_more(void **state) {
	addPiece(createPiece(1, 1, KNIGHT, 1, 1), &list);
	addPiece(createPiece(2, 7, KNIGHT, 1, 8), &list);
	addPiece(createPiece(3, 4, PEASANT, 1, 1), &list);
	addPiece(createPiece(900, 125, KING, 1, 7), &list);
	addPiece(createPiece(2, 2, KNIGHT, 1, 6), &list);
	addPiece(createPiece(10, 17, PEASANT, 1, 10), &list);
	
	return 0;
}
    
