 /** @file
    Interface of parser.

 */

#ifndef PARSE_H
#define PARSE_H

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
