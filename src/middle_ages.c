 /** @file
    Implementation of main program.

 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
	int player = 0;
	command * nCommand;
	while (1) {
		nCommand = parseCommand();
		if (nCommand == NULL) {
			// fprintf(stderr, "input error\n");
			freeEverything(nCommand);
			return 42;
		}
			
		switch (nCommand->commandNumber) {
			int x;
			case INIT:
				player = nCommand->data[2];
				x = init(nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3],
					nCommand->data[4],
					nCommand->data[5],
					nCommand->data[6]);
				if (x == 42) {
					// fprintf(stderr, "input error1\n");
					freeEverything(nCommand);
					return 42;
				}
				if (player == 1) {
					printf("END_TURN\n");
					fflush(stdout);
					x = endTurn();
				}
				break;
			case MOVE:
				x = move(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				if (x == 42) {
					freeEverything(nCommand);
					return 42;
				}
				if (x == -1) {
					freeEverything(nCommand);
					return 1;
				}
				if (x == 2) {
					freeEverything(nCommand);
					if (player == 2) return 0;
					return 2;
				}
				if (x == 1) {
					freeEverything(nCommand);
					if (player == 1) return 0;
					return 2;
				}
				break;
			case PRODUCE_KNIGHT:
				x = produceKnight(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				if (x == 42) {
					freeEverything(nCommand);
					return 42;
				}
				break;
			case PRODUCE_PEASANT:
				x = producePeasant(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				if (x == 42) {
					freeEverything(nCommand);
					return 42;
				}
				break;
			case END_TURN:
				x = endTurn();
				if (x == 42) {
					freeEverything(nCommand);
					return 42;
				}
				if (x == 1) {
					freeEverything(nCommand);
					return 1;
				}
				printf("END_TURN\n");
				fflush(stdout);
				x = endTurn();
				if (x == 42) {
					freeEverything(nCommand);
					return 42;
				}
				if (x == 1) {
					freeEverything(nCommand);
					return 1;
				}
		}
		free(nCommand->name);
		free(nCommand);
    }
    endGame();
    return 0;
}
