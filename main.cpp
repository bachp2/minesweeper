#include <iostream>
#include <ncurses.h>
#include "game.h"

int main(int argc, char ** argv) {

    int c;
    // initializes the screen
    // sets up memory and clears the screen
    initscr();
    cbreak();
    noecho();

    keypad(stdscr, TRUE);
    
    printw("Mine Sweeper\n");
    WINDOW *window = newwin(HEIGHT, WIDTH, 1, 1);
    refresh();

    game::drawGame(window);
    //draw game board
    wmove(window, 0, 0);
    //move cursor onto the window's frame

    wrefresh(window);

    std::list<mine> mines;
    game::createMines(mines);

    while(running){
        c = getchar();
        game::action(window, mines, c);
    }

    getchar();
    //wait to close

    endwin();
    //deallocates memory and ends ncurses
    return 0;
}

