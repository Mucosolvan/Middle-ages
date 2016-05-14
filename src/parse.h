 /** @file
    Interface of parser.

 */

#ifndef PARSE_H
#define PARSE_H

enum commands {INIT, MOVE, PRODUCE_PEASANT, PRODUCE_KNIGHT, END_TURN};

typedef struct command {
	int commandNumber;
    char* name;
    int data[7];
} command;


/**
 * Reads and parses an input to command format.
 * @return NULL Command was in incorrect format.
 * @return command* Command was correct.
 */
command* parseCommand();

#endif /* PARSE_H */
