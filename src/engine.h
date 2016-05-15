/** @file
    Interface of game engine.

 */

#ifndef ENGINE_H
#define ENGINE_H

/**
 * Frees memory after finishing the game.
 */
void endGame();

/**
 * Initializes a game.
 * @param[in] n Board size.
 * @param[in] k Number of turns.
 * @param[in] p Player number.
 * @param[in] x1 First player's king column number.
 * @param[in] y1 First player's king row number.
 * @param[in] x2 Second player's king column number.
 * @param[in] y2 Second player's king row number.
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return 42 Move is incorrect or done before second init.
 * @return 1 By doing this move first player won.
 * @return 2 By doing this move second player won.
 * @return -1 By doing this move players drew.
 * @return 0 Otherwise.
 */
int move(int x1, int y1, int x2, int y2);

/**
 * Produces a knight.
 * @param[in] x1 Peasant's column number.
 * @param[in] y1 Peasant's row number.
 * @param[in] x2 New knight's column number.
 * @param[in] y2 New knight's row number.
 * @return 42 Move is incorrect or done before second init.
 * @return 0 Otherwise.
 */
int produceKnight(int x1, int y1, int x2, int y2);

/**
 * Produces a peasant.
 * @param[in] x1 Peasant's column number.
 * @param[in] y1 Peasant's row number.
 * @param[in] x2 New peasant's column number.
 * @param[in] y2 New peasant's row number.
 * @return 42 Move is incorrect or done before second init.
 * @return 0 Otherwise.
 */
int producePeasant(int x1, int y1, int x2, int y2);

/**
 * Ends the turn of current player.
 * @return 42 Command done before second init.
 * @return 1 Turn limit has been achieved.
 * @return 0 Otherwise.
 */ 
int endTurn();

/**
 * Prints (into stdout) top-left corner of the board of size 
  m x m where m = min(n,10).
 */
void printTopLeft();

#endif /* ENGINE_H */
