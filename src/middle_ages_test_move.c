#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cmocka.h"
#include "engine.h"

extern bool putPiece(int x, int y, int type, int player);

extern void prepare();
 
/**
 * Test checks if move done before init is considered incorrect.
 */
static void testMoveBeforeInit(void **state) {
	int x = move(1, 1, 1, 2);
	assert_int_equal(x, 42);
}

/**
 * Test checks if moving not existing piece is incorrect.
 */
static void testMoveNotExistingPiece(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(1, 1, 1, 2);
	assert_int_equal(x, 42);
}

/**
 * Test checks if moving other player's piece is incorrect.
 */
static void testMoveOtherPlayerPiece(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(4, 11, 4, 12);
	assert_int_equal(x, 42);
}

/**
 * Test checks if moving out of board bounds is incorrect.
 */
static void testMoveOutOfBoard(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(1, 2, 0, 2);
	assert_int_equal(x, 42);
}

/**
 * Test checks if moving more than one square is incorrect.
 */
static void testMoveMoreThanOneSquare(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(1, 2, 3, 2);
	assert_int_equal(x, 42);
}

/**
 * Test checks if moving to already occupied square is incorrect.
 */
static void testMoveToOccupiedSquare(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(1, 2, 2, 2);
	assert_int_equal(x, 42);
}

/**
 * Test checks if moving twice in one turn is incorrect.
 */
static void testMoveTwiceInOneTurn(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(4, 2, 5, 3);
	assert_int_equal(x, 47);
	x = move(5, 3, 6, 3);
	assert_int_equal(x, 42);
}

/**
 * Test checks if list of pieces updates correctly after a move.
 */
static void testListUpdate(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(4, 2, 5, 3);
	assert_int_equal(x, 47);
	endTurn();
	endTurn();
	x = move(5, 3, 6, 3);
	assert_int_equal(x, 47);
}

/**
 * Test checks if list of pieces does not update after incorrect move.
 */
static void testListNotUpdate(void **state) {
	init(15, 10, 1, 1, 2, 4, 11);
	int x = move(1, 2, 0, 2);
	assert_int_equal(x, 42);
	endTurn();
	endTurn();
	x = move(0, 2, 1, 2);
	assert_int_equal(x, 42);
}

/**
 * Test checks if a fight between peasant and peasant is executed correctly.
 */
static void testBattlePeasantPeasant(void **state) {
	prepare();
	putPiece(5, 6, 1, 1);
	putPiece(6, 6, 1, 2);
	int x = move(5, 6, 6, 6);
	assert_int_equal(x, 47);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x,42);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x, 42);
}

/**
 * Test checks if a fight between peasant and king is executed correctly.
 */
static void testBattlePeasantKing(void **state) {
	prepare();
	putPiece(5, 6, 1, 1);
	putPiece(6, 6, 2, 2);
	int x = move(5, 6, 6, 6);
	assert_int_equal(x, 47);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x, 47);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x, 42);
}

/**
 * Test checks if a fight between peasant and knight is executed correctly.
 */
static void testBattlePeasantKnight(void **state) {
	prepare();
	putPiece(5, 6, 1, 1);
	putPiece(6, 6, 3, 2);
	int x = move(5, 6, 6, 6);
	assert_int_equal(x, 47);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x, 47);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x, 42);
}

/**
 * Test checks if the fight between kings is executed correctly.
 * Also checks if function exits with correct code.
 */
static void testBattleKingKing(void **state) {
	prepare();
	putPiece(5, 6, 2, 1);
	putPiece(6, 6, 2, 2);
	int x = move(5, 6, 6, 6);
	assert_int_equal(x, 1);
}

/**
 * Test checks if the fight between king and knight is executed correctly.
 * Also checks if function exits with correct code.
 */
static void testBattleKingKnight(void **state) {
	prepare();
	putPiece(5, 6, 2, 1);
	putPiece(6, 6, 3, 2);
	int x = move(5, 6, 6, 6);
	assert_int_equal(x, 2);
}

/**
 * Test checks if the fight between knight and king is executed correctly.
 * Also checks if function exits with correct code.
 */
static void testBattleKnightKing(void **state) {
	prepare();
	putPiece(5, 6, 3, 1);
	putPiece(6, 6, 2, 2);
	int x = move(5, 6, 6, 6);
	assert_int_equal(x, 0);
}

/**
 * Test checks if the fight between knights is executed correctly.
 */
static void testBattleKnightKnight(void **state) {
	prepare();
	putPiece(5, 6, 3, 1);
	putPiece(6, 6, 3, 2);
	int x = move(5, 6, 6, 6);
	assert_int_equal(x, 47);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x, 42);
	endTurn();
	x = move(6, 6, 6, 7);
	assert_int_equal(x, 42);
}

extern int middle_ages_main(int argc, char *argv[]);


static int teardown(void **state) {
	endGame();
	
	return 0;
}
