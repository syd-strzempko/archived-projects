#ifndef SNAKE_H
#define SNAKE_H
//
//    SNAKE.H   - modern Tron/Snake game, w wrapping, point system
//    author    - Sydney Strzempko
//    lastEdit  - 24 Oct 2015
//
const int ROWS = 23;
const int COLS = 63;

struct gps /*positional values for each component*/
{
        int rows;
        int cols;
};
struct snake
{
        gps pos;
        gps old;
        int score;
        int EU;
        char pic;
};

//    BOARD & required functions
void setup_board        (char x[][COLS]);       //prefills board
void print              (char x[][COLS]);
int play_game           (char x[][COLS]);       //runs the game
void report             (int a);                //prints out final score
void draw_board         (char x[][COLS], snake *s1);

//    SNAKE functions
void print_score        (snake *s1);
void setup_snake        (snake *s1);
void head_fwd           (snake *s1);                    //moves snake fwd, by struct
void head_check         (char x[][COLS], snake *s1);    //checks for presence on board
void turn               (char a, snake *s1);            //turns snake, by struct

//    bug&egg functions
void set_bug            (char x[][COLS], bool bug);     //sets bug position
#endif
