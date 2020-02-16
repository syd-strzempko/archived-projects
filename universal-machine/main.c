/*
 * Jonah Feldman and Sydney Strzempko
 * 11-20-16
 * main file for the univeral machine
 *
 */
#include "um.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
 
        
        if (argc != 2)
        {
          
                fprintf (stderr,
                "incorrect number of arguments on command line\n");
                exit(1);   
        }

        Um um = new_Um(argv[1]);
        run_Um(um);
        free_Um(um);
        
}
