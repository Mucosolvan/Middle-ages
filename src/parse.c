#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

typedef struct command command;

struct command {
	int commandNumber;
    char* name;
    int data[7];
};

enum commands {INIT, MOVE, PRODUCE_PEASANT, PRODUCE_KNIGHT, END_TURN};
int parameters[5] = {7, 4, 4, 4, 0};

int convertToInt(char* number) {
	size_t x = strlen(number); // problem z strlenem invalid read
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

void freeCommand(command* com) {
	if (com != NULL)
		free(com->name);
	free(com);
}

command* split(char* str) {
	command* com = malloc(sizeof(command));
	for (int i = 0; i < 7; i++)
		com->data[i] = 0;
	com->name = malloc(16 * sizeof(char));
	int cnt = 0, dataCnt = 0;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == ' ') {
			if (cnt == 0) {
				if (i > 16) {
					freeCommand(com);
					return NULL;
				}
				strncpy(com->name, str, i);
				com->name[i] = '\0';
				cnt = i + 1;
			}
			else {
				if (dataCnt >= 7) {
					freeCommand(com);
					return NULL;
				}
				char* tmp = malloc((i - cnt + 3) * sizeof(char)); // problem z strlenem invalid read
				strncpy(tmp, str + cnt, i - cnt);
				tmp[i - cnt] = '\0';
				com->data[dataCnt] = convertToInt(tmp);
				dataCnt++;
				cnt = i + 1;
				free(tmp);
			}
		}
	}
	if (dataCnt >= 7) {
		freeCommand(com);
		return NULL;
	}
	if (cnt == 0) {
		if (strlen(str) > 16) {
			freeCommand(com);
			return NULL;
		}
		strncpy(com->name, str, strlen(str) - 1);
		com->name[strlen(str) - 1] = '\0';
	}
	else {
		char* tmp = malloc((strlen(str) - cnt + 3) * sizeof(char));
		strncpy(tmp, str + cnt, strlen(str) - 1 - cnt);
		tmp[strlen(str) - cnt - 1] = '\0';
		com->data[dataCnt] = convertToInt(tmp);
		free(tmp);
	}
	return com;
}

command* validateCommand(command* com) {
	if (com == NULL)
		return NULL;
	int paramNumber = 0;
	for (int i = 0; i < 7; i++) {
		if (com->data[i] == -1) {
			freeCommand(com);
			return NULL;
		}
		else
			if (com->data[i] != 0)
				paramNumber = i + 1;
	}
	if (strcmp(com->name, "INIT") == 0) {
		com->commandNumber = INIT;
		if (paramNumber != parameters[INIT]) {
			freeCommand(com);
			return NULL;
		}
		return com;
	}
	else if (strcmp(com->name, "MOVE") == 0) {
		com->commandNumber = MOVE;
		if (paramNumber != parameters[MOVE]) {
			freeCommand(com);
			return NULL;
		}
		return com;
	}
	else if (strcmp(com->name, "PRODUCE_KNIGHT") == 0) {
		com->commandNumber = PRODUCE_KNIGHT;
		if (paramNumber != parameters[PRODUCE_KNIGHT]) {
			freeCommand(com);
			return NULL;
		}
		return com;
	}
	else if (strcmp(com->name, "PRODUCE_PEASANT") == 0) {
		com->commandNumber = PRODUCE_PEASANT;
		if (paramNumber != parameters[PRODUCE_PEASANT]) {
			freeCommand(com);
			return NULL;
		}
		return com;
	}
	else if (strcmp(com->name, "END_TURN") == 0) {
		com->commandNumber = END_TURN;
		if (paramNumber != parameters[END_TURN]) {
			freeCommand(com);
			return NULL;
		}
		return com;
	}
	else {
		freeCommand(com);
		return NULL;
	}
}
command* parseCommand() {
	char* str;
	char tmp[105];
	str = fgets(tmp, 102, stdin);
	if (strlen(str) > 100)
		return NULL;
	command* com = split(str);
	return validateCommand(com);
}

/* int main() {
	while (1)
	{
		if (parseCommand() == NULL)
			break;
	}
	return 0;
} */
