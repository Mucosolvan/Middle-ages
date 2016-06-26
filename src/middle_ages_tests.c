#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cmocka.h"
#include "engine.h"
#include "middle_ages_test_piece_exists.c"
#include "middle_ages_test_move.c"
#include "middle_ages_test_move_parse.c"

extern int middle_ages_main(int argc, char *argv[]);

int main() {
	
const struct CMUnitTest tests[] = {
		cmocka_unit_test_teardown(testEmptyList, teardownP),
		cmocka_unit_test_setup_teardown(testPieceDoesNotExist, setup_add_one, teardownP),
		cmocka_unit_test_setup_teardown(testPieceDoesNotExist2, setup_add_more, teardownP),
		cmocka_unit_test_setup_teardown(testPieceExists, setup_add_one, teardownP),
		cmocka_unit_test_setup_teardown(testPieceExists2, setup_add_more, teardownP),
		cmocka_unit_test_setup_teardown(testPieceExists3, setup_add_more, teardownP),
		cmocka_unit_test_setup_teardown(testPieceExists4, setup_add_more, teardownP),
		cmocka_unit_test_setup_teardown(testPieceExists5, setup_add_more, teardownP),
		cmocka_unit_test_teardown(testMoveNotExistingPiece, teardown),
        cmocka_unit_test_teardown(testMoveBeforeInit, teardown),
        cmocka_unit_test_teardown(testMoveNotExistingPiece, teardown),
        cmocka_unit_test_teardown(testMoveOtherPlayerPiece, teardown),
        cmocka_unit_test_teardown(testMoveOutOfBoard, teardown),
        cmocka_unit_test_teardown(testMoveMoreThanOneSquare, teardown),
        cmocka_unit_test_teardown(testMoveToOccupiedSquare, teardown),
        cmocka_unit_test_teardown(testListUpdate, teardown),
        cmocka_unit_test_teardown(testListNotUpdate, teardown),
        cmocka_unit_test_teardown(testMoveTwiceInOneTurn, teardown),
        cmocka_unit_test_teardown(testBattlePeasantPeasant, teardown),
        cmocka_unit_test_teardown(testBattlePeasantKing, teardown),
        cmocka_unit_test_teardown(testBattlePeasantKnight, teardown),
        cmocka_unit_test_teardown(testBattleKingKing, teardown),
        cmocka_unit_test_teardown(testBattleKingKnight, teardown),
		cmocka_unit_test_teardown(testBattleKnightKing, teardown),
		cmocka_unit_test_teardown(testBattleKnightKnight, teardown),
		cmocka_unit_test(testWeirdInput),
		cmocka_unit_test(testTooFewArguments),
		cmocka_unit_test(testTooManyArguments),
		cmocka_unit_test(testOutOfBounds1),
		cmocka_unit_test(testOutOfBounds2),
		cmocka_unit_test(testNotCapitalLetters),
		cmocka_unit_test(testNotInteger),
		cmocka_unit_test(testNoEndLine),
		cmocka_unit_test(testCorrectInput),
		cmocka_unit_test(testCorrectInputMaxBound)
};
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
