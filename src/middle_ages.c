#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"

int main() {
	command * nCommand;
	while (1) {
		nCommand = parseCommand();
		if (nCommand == NULL) {
			fprintf(stderr, "input error\n");
			free(nCommand);
			endGame();
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
					free(nCommand->name);
					free(nCommand);
					endGame();
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
					free(nCommand->name);
					free(nCommand);
					endGame();
					return 42;
				}
				printTopLeft();
				if (x == -1) {
					fprintf(stderr, "draw\n");
					free(nCommand->name);
					free(nCommand);
					endGame();
					return 0;
				}
				if (x == 2) {
					fprintf(stderr, "player 2 won\n");
					free(nCommand->name);
					free(nCommand);
					endGame();
					return 0;
				}
				if (x == 1) {
					fprintf(stderr, "player 1 won\n");
					free(nCommand->name);
					free(nCommand);
					endGame();
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
					free(nCommand->name);
					free(nCommand);
					endGame();
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
					free(nCommand->name);
					free(nCommand);
					endGame();
					return 42;
				}
				printTopLeft();
				break;
			case END_TURN:
				x = endTurn();
				if (x == 42) {
					fprintf(stderr, "input error\n");
					free(nCommand->name);
					free(nCommand);
					endGame();
					return 42;
				}
				if (x == 1) {
					fprintf(stderr, "draw\n");
					free(nCommand->name);
					free(nCommand);
					endGame();
					return 0;
				}
		}
		free(nCommand->name);
		free(nCommand);
    }
    endGame();
    return 0;
}
