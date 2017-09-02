//
// Created by Bach Phan on 8/26/17.
//

#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include <iostream>
#include <ncurses.h>
#include <list>

#define NUM_BOMBS 50
#define HEIGHT  15
#define WIDTH   15

extern bool running;
extern bool firstTry;

struct mine {
    int x, y;
    mine(int yCo, int xCo) : x(xCo), y(yCo) {}
    bool operator<(const mine& other) const;
    bool operator==(const mine& other) const;
};

class game {

public:
    static void drawGame(WINDOW* window);
    static void action(WINDOW* window, std::list<mine>& mines, int c);
    static std::list<mine> createMines(std::list<mine> &mines);
private:
    static bool isWithinBoundary(int y, int x);
    static  int isOnMine(std::list<mine> &mines, int y, int x);
    static void defuse(WINDOW* window, std::list<mine>& mines, int y, int x);
    static void getMinesCount(WINDOW *window, std::list<mine> &mines, int y, int x);
    static void automaticWin(WINDOW* window, std::list<mine>& mines);
    static bool areAllMinesFlagged(WINDOW *window, std::list<mine> &mines);
    static void reveal(WINDOW* window, std::list<mine>& mines, const char*);
    static  int countFlags(WINDOW* window);
};


#endif //MINESWEEPER_GAME_H
