 /** @file
    Implementation of parser.

 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

typedef struct command command;

/**
 * Command structure that holds and input in a specific format.
 */ 
struct command {
	int commandNumber; /**< Number of command to easily recognise it. */
    char* name; /**< Name of command. */
    int data[7]; /**< Parametres of command. */
};

/**
 * Numbers of specific command as an enum.
 */ 
enum commands {INIT, MOVE, PRODUCE_PEASANT, PRODUCE_KNIGHT, END_TURN};

/**
 * Number of parameters each command requires.
 */
int parameters[5] = {7, 4, 4, 4, 0};

/**
 * Converts string to number between 1 and 2^31 - 1/
 * @param[in] number String with a number.
 * @return -1 String is not a number or not in limits.
 * @return int Number.
 */
int convertToInt(char* number) {
	size_t x = strlen(number);
	if (x == 0 || x > 10)
		return -1;
	long long int result = 0;
	long long int power = 1;
	for (int i = x - 1; i >= 0; i--) {
		if (number[i] < '0' || number[i] > '9')
			return -1;
		result += (number[i] - 48) * power;
		power *= 10;
	}
	if ((long long)INT_MAX < result || result == 0)
		return -1;
	return result;
}

/**
 * Frees command.
 * @param[in] com Command to free.
 */
void* freeCommand(command* com) {
	if (com != NULL)
		free(com->name);
	free(com);
	return NULL;
}

/**
 * Splits a string to command format.
 * @param[in] str String to split.
 * @return NULL String is in incorrect format(input error).
 * @return command* Correct command.
 */
command* split(char* str) {
	command* com = malloc(sizeof(command));
	for (int i = 0; i < 7; i++)
		com->data[i] = 0;
	com->name = malloc(16 * sizeof(char));
	int cnt = 0, dataCnt = 0;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == ' ') {
			if (cnt == 0) {
				if (i > 16) // command is too long
					return freeCommand(com);
				strncpy(com->name, str, i);
				com->name[i] = '\0';
				cnt = i + 1;
			}
			else {
				if (dataCnt >= 7) // too many parameters
					return freeCommand(com);
				char* tmp = malloc((i - cnt + 1) * sizeof(char));
				strncpy(tmp, str + cnt, i - cnt);
				tmp[i - cnt] = '\0';
				com->data[dataCnt] = convertToInt(tmp);
				dataCnt++;
				cnt = i + 1;
				free(tmp);
			}
		}
	}
	if (dataCnt >= 7) // too many parametres
		return freeCommand(com);
	if (cnt == 0) {
		if (strlen(str) > 16) // command is too long
			return freeCommand(com);
		strncpy(com->name, str, strlen(str) - 1);
		com->name[strlen(str) - 1] = '\0';
	}
	else {
		char* tmp = malloc((strlen(str) - cnt + 1) * sizeof(char));
		strncpy(tmp, str + cnt, strlen(str) - 1 - cnt);
		tmp[strlen(str) - cnt - 1] = '\0';
		com->data[dataCnt] = convertToInt(tmp);
		free(tmp);
	}
	return com;
}

/**
 * Checks if command is valid.
 * @param[in] com Command to check.
 */
command* validateCommand(command* com) {
	if (com == NULL)
		return NULL;
	int paramNumber = 0;
	for (int i = 0; i < 7; i++) {
		if (com->data[i] == -1) // parameter was not a correct integer
			return freeCommand(com);
		else
			if (com->data[i] != 0)
				paramNumber = i + 1;
	}
	if (strcmp(com->name, "INIT") == 0) {
		com->commandNumber = INIT;
		if (paramNumber != parameters[INIT])
			return freeCommand(com);
		return com;
	}
	else if (strcmp(com->name, "MOVE") == 0) {
		com->commandNumber = MOVE;
		if (paramNumber != parameters[MOVE])
			return freeCommand(com);
		return com;
	}
	else if (strcmp(com->name, "PRODUCE_KNIGHT") == 0) {
		com->commandNumber = PRODUCE_KNIGHT;
		if (paramNumber != parameters[PRODUCE_KNIGHT])
			return freeCommand(com);
		return com;
	}
	else if (strcmp(com->name, "PRODUCE_PEASANT") == 0) {
		com->commandNumber = PRODUCE_PEASANT;
		if (paramNumber != parameters[PRODUCE_PEASANT])
			return freeCommand(com);
		return com;
	}
	else if (strcmp(com->name, "END_TURN") == 0) {
		com->commandNumber = END_TURN;
		if (paramNumber != parameters[END_TURN])
			return freeCommand(com);
		return com;
	}
	else
		return freeCommand(com);
}

/**
 * Reads and parses an input to command format.
 * @return NULL Command was in incorrect format.
 * @return command* Command was correct.
 */
command* parseCommand() {
	char* str;
	char tmp[105];
	str = fgets(tmp, 102, stdin);
	if (strlen(str) > 100) // input was too long
		return NULL;
	command* com = split(str);
	return validateCommand(com);
}
