#include "termfuncs.h"
#include "snake.h"
using namespace std;
//
//    SNAKE.CPP - modern Tron/Snake game, w wrapping, point system
//    author    - Sydney Strzempko
//    lastEdit  - 24 Oct 2015
//

int main()
{
        char board[ROWS][COLS];
        int score;
        
        setup_board(board);
        score = play_game(board);
        report(score);
        return 0;
}
//
//
//
void setup_board( char x[][COLS])       //honestly more of a s1 setup
{       
        for (int a=0;a<ROWS;a++)        //BORDER SETUP 
        {
                for(int b=0;b<COLS;b++)
                {
                        if (((a==0)||(a==(ROWS-1)))&&
                            ((b==0)||(b==(COLS-1))))
                        {
                                x[a][b] = '+';
                        } else if ((a==0)||(a==(ROWS-1)))
                        {
                                x[a][b] = '-';
                        } else if ((b==0)||(b==(COLS-1)))
                        {
                                x[a][b] = '|';
                        
                        } else if ((a==((ROWS-1)/2))&&(b==((COLS-1)/2)))
                        {
                                x[a][b] = '^';
                        } else {
                                x[a][b] = ' ';
                        }
                }
        }
        set_bug(x,true);        //sets the bug
        set_bug(x,false);       //sets the egg
        print(x);
        cout << "EU:  60   Score:  0" << endl;
}
void draw_board( char x[][COLS], snake *s1)     //edits internal board values
{
        for (int a=0;a<ROWS;a++)        
        {
                for(int b=0;b<COLS;b++)
                {
                        if((a==s1->pos.rows)&&(b==s1->pos.cols))
                        {
                                x[a][b] = s1->pic;
                        } else if ((a==s1->old.rows)&&(b==s1->old.cols))
                        {
                                x[a][b] = '#';  //sets the old spot to a hash val
                        }
                }
        }
        print(x);
        print_score(s1);
}
//
//
//
void print(char x[][COLS])      //PRINTS ONLY THE ARRAY & SCORE
{
        screen_clear();
        screen_home();
        for (int a=0;a<ROWS;a++)        //BORDER SETUP 
                {
                        for(int b=0;b<COLS;b++)
                        {
                                cout << x[a][b];
                        }
                        cout << endl;
                }
        
}
//
//
//
int play_game(char x[][COLS])
{
        snake s1;
        setup_snake(&s1);
        
        while (s1.EU != 0)
        {
                char input;
                do
                {
                        input = getachar();
                }while((input!='f')&&(input!='l')&&(input!='r'));
                
                if (input=='f')
                {
                        s1.score++;
                        head_fwd(&s1);  //incorporates wall check, TRON aspect
                        head_check(x,&s1);
                } else {
                        turn(input,&s1);
                }
                draw_board(x,&s1);
        }
        return s1.score;
}
//
//
//
void report(int a)
{
        cout << "Game Over.  Final Score: " << a << endl;
}
//
//
//
void head_fwd(snake *s1)        //moves the head forward EFFECTS S1
{
        s1->old.rows = s1->pos.rows;
        s1->old.cols = s1->pos.cols;    //sets hashes to head
        if (s1->pic=='^')
        {
                s1->pos.rows--; //head moves UP
        } else if (s1->pic=='v')
        {
                s1->pos.rows++; //head moves down
        } else if (s1->pic=='<')
        {
                s1->pos.cols--; //head goes L
        } else if (s1->pic=='>')
        {
                s1->pos.cols++; //head goes R
        }
        if (s1->pos.cols==0)    /* BEGIN OF WALL check*/
        {
                s1->pos.cols = (COLS-2);        //sets across board
        } else if (s1->pos.cols==(COLS-1))
        {
                s1->pos.cols = 1;               //sets across board
        } else if (s1->pos.rows==0)
        {
                s1->pos.rows = (ROWS-2);
        } else if (s1->pos.rows==(ROWS-1))
        {
                s1->pos.rows = 1;
        }
        s1->EU--;
}
//
//
//
void turn(char a, snake *s1)            //EFFECTS STRUCT s1
{
        if (a=='l')
        {
                if (s1->pic=='^')
                {
                        s1->pic='<';
                } else if (s1->pic=='<')
                {
                        s1->pic='v';
                } else if (s1->pic=='v')
                {
                        s1->pic='>';
                } else if (s1->pic=='>')
                {
                        s1->pic='^';
                }
        } else if (a=='r')
        {
                if (s1->pic=='^')
                {
                        s1->pic='>';
                } else if (s1->pic=='>')
                {
                        s1->pic='v';
                } else if (s1->pic=='v')
                {
                        s1->pic='<';
                } else if (s1->pic=='<')
                {
                        s1->pic='^';
                } 
        }
}
//
//
//
void head_check(char x[][COLS], snake *s1)              //after snake vals changed
{
        char interact;
        interact = x[s1->pos.rows][s1->pos.cols];
        if (interact=='#')
        {
                s1->EU = 0;     //kills the snake
        } else if (interact=='X')
        {
                s1->EU += 25;   //adds bug value to EU
                set_bug(x,true);
        } else if (interact=='o')
        {
                s1->EU += 15;   //adds egg value to EU
                set_bug(x,false);
        }
}
//
//
//
void set_bug(char x[][COLS], bool bug)  //MULTITASKS - is both a bug&egg setter
{
        int set_row, set_col;   //decl. int values to be used
        
        do      //assigns random vals to animal
        {
                set_row=random_int(1,(ROWS-2));
                set_col=random_int(1,(COLS-2)); 
        }while(x[set_row][set_col]!=' ');
        
        if (bug==true){
                x[set_row][set_col] = 'X';      //sets the bug
        } else {
                x[set_row][set_col] = 'o';      //sets the egg
        }
        //DO I HAVE TO RETURN THE ARRAY? GO TO PRINT FUNCTION
}
//
//
//
void setup_snake(snake *s1)
{
        s1->pos.rows=((ROWS-1)/2);
        s1->pos.cols=((COLS-1)/2);
        s1->pic = '^';
        s1->score = 0;
        s1->EU = 60;
}
//
//
//
void print_score(snake *s1)
{
        cout << "EU:  " << s1->EU << "  Score:  " << s1->score << endl;
}