#include "termfuncs.h"
using namespace std;
//
// TERMFUNCS.CPP Version: 2015-07-09 -- some simple functions for terminal
//
// ADDED BY SYD: THIS WAS MADE BY COMP-11 STAFF FOR THE COURSE TAKEN AT TUFTS
// UNIVERSITY. THIS MATERIAL IS NOT MY OWN, SPECIFICALLY TERMFUNCS.
//
//   char getachar()     -- returns next char with no echo and no Enter needed
//   static char handle_escape_seq(); -- handles buffer, helper w getachar
//
//   void screen_clear() -- clears the screen
//   void screen_home()  -- moves cursor to top of screen
//
//   int  random_int(int low, int high) -- returns random int in [low, high]
//         NOTE: if SNAKE_SEED is set then use that for srand; if not, then
//         call srand with time(0) at first call
//   void seed_random(s) -- helper function for r_int
//
static char handle_escape_seq();
static termios prev_tty_state;
static const string CSI = "\033[";
static int prev_state_stored = 0;
static int rand_seed = -1;
//
// getachar
// returns a char using noecho and -icanon unless not a tty
//         NOTE: getachar handles arrow keys by returning
//         KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
//
char getachar()
{
        char c;

        cout << std::flush;
        if ( isatty(0) ){
                struct termios  info, orig;
                // system("stty -echo -icanon");
                tcgetattr(0, &info);
                orig = info;
                prev_tty_state = orig;
                prev_state_stored = 1;

                info.c_lflag &= ~ECHO;
                info.c_lflag &= ~ICANON;
                tcsetattr(0, TCSANOW, &info);

                read(0, &c, 1);
                if ( c == ESCAPE )
                        c = handle_escape_seq();

                // system("stty echo icanon");
                tcsetattr(0, TCSANOW, &orig);
        }
        else {
                read(0, &c, 1);
        }

        return c;
}
//
// ignore signals, then set tty to read in up to 100 chars in 1/10 second
// examine sequence and return code
// Note: read until an alpha char or inactivity
//
char handle_escape_seq()
{
        struct termios  info, orig;
        char            buf[100];
        int             pos = 0;
        char            c;
        void            (*oldhand)(int);

        if ( !isatty(0) ) 
                return ESCAPE;

        oldhand = signal(SIGINT, SIG_IGN);

        // system("stty -echo -icanon");
        tcgetattr(0, &info);
        orig = info;

        info.c_lflag &= ~ECHO;
        info.c_lflag &= ~ICANON;
        info.c_cc[VMIN] = 0;
        info.c_cc[VTIME] = 1;                   // 1/10 second
        tcsetattr(0, TCSANOW, &info);

        do
        {
                if ( read(0, &c, 1) == 1 )      // if got one
                        buf[pos++] = c;         // store it
                else
                        break;                  // else timeout
        }
        while( !isalpha((int)c) && pos < 99 );  // until alpha or full

        if ( pos == 0 || (buf[0] != '[' && buf[0] != 'O' ) )
                c = ESCAPE;
        else 
                switch(buf[1]){
                        case 'A':               c = KEY_UP;     break;
                        case 'B':               c = KEY_DOWN;   break;
                        case 'C':               c = KEY_RIGHT;  break;
                        case 'D':               c = KEY_LEFT;   break;
                        default :
                                                c = ESCAPE;
                }

        tcsetattr(0, TCSANOW, &orig);
        signal(SIGINT, oldhand);
        // system("stty echo icanon");
        return c;
}
//
//    returns a "random" int between the range of given lo&hi parameters
//
int random_int(int lo, int hi)
{
        int     range = hi - lo + 1;
        char    *seed_str;

        if ( rand_seed == -1 ) {
                seed_str = getenv( "SNAKE_SEED" );
                if ( seed_str != NULL )
                        seed_random( (unsigned) atoi(seed_str) );
                else 
                        seed_random( (unsigned) time(NULL) );
        }
        if ( range <= 0 )
                return 17;              // add to function specs

        return lo + ( rand() % range );
}
//
//    Allows for seeding in random_int function
//
void seed_random(int s)
{
        if ( s > 0 ) {
                srand(s);
                rand_seed = s;
        }
}