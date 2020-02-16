/*
 * Jonah Feldman and Sydney Strzempko
 * 11-13-16
 *
 * io.h: interface for input and output for univeral machine
 */

 #include <stdint.h>


/*
 * prints out a number between 0 and 255 to stdout
 * Arguments:
 *           out: the number that will be printed to stdout
 */
void output(int val);


/*
 * gets a number between 0 and 255 from stdin and returns it to the um
 *
 * returns: the byte (0-255 number) we are getting from stdin
 */
 
uint32_t input();
