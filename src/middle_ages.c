 /** @file
    Implementation of main program.

 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"

/**
 * Frees a command and ends the game.
 * @param[in] com Command to free.
 */
void freeEverything(command* com) {
	if (com != NULL)
		free(com->name);
	free(com);
	endGame();
}

/**
 * Main program.
 * Prints on stderr which player won or input error if there was one.
 * Prints on stdout top left corner of the board.
 * @return 42 Input is incorrect.
 * @return 0 One of players won or there was a draw.
 */
int main() {
	command * nCommand;
	while (1) {
		nCommand = parseCommand();
		if (nCommand == NULL) {
			fprintf(stderr, "input error\n");
			freeEverything(nCommand);
			return 42;
		}
			
		switch (nCommand->commandNumber) {
			int x;
			case INIT:
				x = init(nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3],
					nCommand->data[4],
					nCommand->data[5],
					nCommand->data[6]);
				if (x == 42) {
					fprintf(stderr, "input error\n");
					freeEverything(nCommand);
					return 42;
				}
				printTopLeft();
				break;
			case MOVE:
				x = move(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				if (x == 42) {
					fprintf(stderr, "input error\n");
					freeEverything(nCommand);
					return 42;
				}
				printTopLeft();
				if (x == -1) {
					fprintf(stderr, "draw\n");
					freeEverything(nCommand);
					return 0;
				}
				if (x == 2) {
					fprintf(stderr, "player 2 won\n");
					freeEverything(nCommand);
					return 0;
				}
				if (x == 1) {
					fprintf(stderr, "player 1 won\n");
					freeEverything(nCommand);
					return 0;
				}
				break;
			case PRODUCE_KNIGHT:
				x = produceKnight(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				if (x == 42) {
					fprintf(stderr, "input error\n");
					freeEverything(nCommand);
					return 42;
				}
				printTopLeft();
				break;
			case PRODUCE_PEASANT:
				x = producePeasant(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				if (x == 42) {
					fprintf(stderr, "input error\n");
					freeEverything(nCommand);
					return 42;
				}
				printTopLeft();
				break;
			case END_TURN:
				x = endTurn();
				if (x == 42) {
					fprintf(stderr, "input error\n");
					freeEverything(nCommand);
					return 42;
				}
				if (x == 1) {
					fprintf(stderr, "draw\n");
					freeEverything(nCommand);
					return 0;
				}
		}
		free(nCommand->name);
		free(nCommand);
    }
    endGame();
    return 0;
}
