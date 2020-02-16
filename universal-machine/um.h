/*um.h
 *      Jonah Feldman & Sydney Strzempko, 16 Nov 16
 *     HW6 - UM
 *
 * contains interface for simple 14-instruction Universal Virtual Machine
 */
typedef struct Um *Um;


/*
 * creates a universal machine and allocates all neccesary memory. Initializes
 * mem[0] in the UM to a program given from a filename.  
 *
 * Arguments:
 *           fname - the name of the file containing the instructions
 *
 * Returns: a pointer to the newly created UM struct 
 * c.r.e
 *      fname is not a valid filename
 */
Um new_Um(char *fname);


/*
 * Runs the loaded program in the UM
 * 
 * Arguments:
 *           um - a pointer to a um struct, contains preloaded program at
 *           spot zero in memory
 */
void run_Um(Um um);






/*
 * de-allocates all memory allocated in um, as well as freeing the struct
 * itself
 *
 * Arguments:
 *           um - a pointer to a UM struct 
 */
void free_Um(Um um);