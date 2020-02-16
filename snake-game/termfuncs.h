#ifndef TERMFUNCS_H
#define TERMFUNCS_H
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <string>
using std::string;
//
// TERMFUNCS.H Version: 2015-07-09 -- some simple functions for terminal
//
//
// ADDED BY SYD: THIS WAS MADE BY COMP-11 STAFF FOR THE COURSE TAKEN AT TUFTS
// UNIVERSITY. THIS MATERIAL IS NOT MY OWN, SPECIFICALLY TERMFUNCS.
//
//   char getachar()     -- returns next char with no echo and no Enter needed
//
//   void screen_clear() -- clears the screen
//   void screen_home()  -- moves cursor to top of screen
//
//   int  random_int(int low, int high) -- returns random int in [low, high]
//         NOTE: if SNAKE_SEED is set then use that for srand; if not, then
//         call srand with time(0) at first call
//   void seed_random(s) -- helper function for r_int
//
const char      KEY_UP    = 130;
const char      KEY_DOWN  = 131;
const char      KEY_LEFT  = 132;
const char      KEY_RIGHT = 133;
const char      ESCAPE    = 27;

char getachar();
void screen_clear();
void screen_home();

int  random_int(int, int);
void seed_random(int);

#endif