#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cmocka.h"
#include "parse.h"

char napis[1000];
extern int middle_ages_main(int argc, char *argv[]);
extern void freeCommand(command* com);

/**
 * Mocks fgets to control input in tests.
 */ 
char* mock_fgets(char *s, int size, FILE *stream)
{
	assert_true(stream == stdin);
	strncpy(s, mock_ptr_type(char *), size);
	return s;
}

/**
 * Test checks if parse works correctly given weird input.
 */
static void testWeirdInput(void **state) {
	will_return(mock_fgets, "250 72safasfa3864dsadasdsd732\n");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly given too few arguments.
 */
static void testTooFewArguments(void **state) {
	will_return(mock_fgets, "MOVE 2 3 4\n");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly given too many arguments.
 */
static void testTooManyArguments(void **state) {
	will_return(mock_fgets, "MOVE 2 3 4 5 6\n");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly if one of coordinates is too low.
 */
static void testOutOfBounds1(void **state) {
	will_return(mock_fgets, "MOVE 0 1 1 1\n");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly if one of coordinates is too big.
 */
static void testOutOfBounds2(void **state) {
	will_return(mock_fgets, "MOVE 1 1 2147483648 1\n");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly if command is not written in capital letters.
 */
static void testNotCapitalLetters(void **state) {
	will_return(mock_fgets, "Move 1 1 2 1\n");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly if one of coordinates is not an integer.
 */
static void testNotInteger(void **state) {
	will_return(mock_fgets, "MOVE 123a23 1 123124 1\n");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly if there is no endline sign.
 */
static void testNoEndLine(void **state) {
	will_return(mock_fgets, "MOVE 1 1 2 1");
	command* com = parseCommand();
	assert_null(com);
}

/**
 * Test checks if parse works correctly given correct input.
 */
static void testCorrectInput(void **state) {
	will_return(mock_fgets, "MOVE 2 3 4 5\n");
	command* com = parseCommand();
	assert_non_null(com);
	assert_int_equal(com->data[0], 2);
	assert_int_equal(com->data[1], 3);
	assert_int_equal(com->data[2], 4);
	assert_int_equal(com->data[3], 5);
	freeCommand(com);
}

/**
 * Test checks if parse works correctly given correct input.
 * Also checks if upper bound is correct.
 */
static void testCorrectInputMaxBound(void **state) {
	will_return(mock_fgets, "MOVE 2 3 2147483647 5\n");
	command* com = parseCommand();
	assert_non_null(com);
	assert_int_equal(com->data[0], 2);
	assert_int_equal(com->data[1], 3);
	assert_int_equal(com->data[2], 2147483647);
	assert_int_equal(com->data[3], 5);
	freeCommand(com);
}
