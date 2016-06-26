 /** @file
    Implementation of main program.

 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
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
	int exitCode = 47;
	while (exitCode == 47) {
		nCommand = parseCommand();
		if (nCommand == NULL) {
			exitCode = 42;
			break;
		}
		
		switch (nCommand->commandNumber) {
			case INIT:
				player = nCommand->data[2];
				exitCode = init(nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3],
					nCommand->data[4],
					nCommand->data[5],
					nCommand->data[6]);
				
				if (player == 1 && exitCode == 47) {
					makeMove();
					// exitCode = endTurn();
				}
				break;
			case MOVE:
				exitCode = move(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
					
				break;
			case PRODUCE_KNIGHT:
				exitCode = produceKnight(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				break;
			case PRODUCE_PEASANT:
				exitCode = producePeasant(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				break;
			case END_TURN:
				exitCode = endTurn();
				if (exitCode == 47) {
					exitCode = makeMove();
				}
				break;
		}
		free(nCommand->name);
		free(nCommand);
    }
    endGame();
    return exitCode;
}
