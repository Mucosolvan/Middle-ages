#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "engine.h"

int main() {
	command * nCommand;
	while (1) {
		nCommand = parseCommand();
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
				printTopLeft();
				if (x == 42) {
					fprintf(stderr, "input error");
					endGame();
					return 42;
				}
				break;
			case MOVE:
				x = move(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				printTopLeft();
				if (x == 42) {
					fprintf(stderr, "input error");
					endGame();
					return 42;
				}
				if (x == -1) {
					fprintf(stderr, "draw");
					endGame();
					return 0;
				}
				if (x == 2) {
					fprintf(stderr, "player 2 won");
					endGame();
					return 0;
				}
				if (x == 1) {
					fprintf(stderr, "player 1 won");
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
				printTopLeft();
				if (x == 42) {
					fprintf(stderr, "input error");
					endGame();
					return 42;
				}
				break;
			case PRODUCE_PEASANT:
				x = producePeasant(
					nCommand->data[0],
					nCommand->data[1],
					nCommand->data[2],
					nCommand->data[3]);
				printTopLeft();
				if (x == 42) {
					fprintf(stderr, "input error");
					endGame();
					return 42;
				}
				break;
			case END_TURN:
				x = endTurn();
				if (x == 42) {
					fprintf(stderr, "input error");
					endGame();
					return 42;
				}
				if (x == 1) {
					fprintf(stderr, "draw");
					endGame();
					return 0;
				}
		}
    }
    endGame();
    return 0;
} 
