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


/** Reads a command.
  Returns command with data points using 'command' structure.
  */
command* parseCommand();

#endif /* PARSE_H */
