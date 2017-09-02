//
// Created by Bach Phan on 8/26/17.
// MIT License 2017
//

#include "game.h"
bool running = true;

bool firstTry = true;
//will allow player to continue if pick a mined grid on first try

void game::drawGame(WINDOW *window){
    std::string s = std::string(WIDTH, '.');
    for(int i = 0; i < HEIGHT; ++i){
        wprintw(window, s.c_str());
        wmove(window, i+1, 0);
    }
}

void game::action(WINDOW *window, std::list<mine>& mines, int c){
    int y,x;
    getyx(window, y, x);
    switch(c){
        case KEY_LEFT:
        case 'j':
            if(x == 0)
                x = WIDTH;
            x--;
            break;
        case KEY_RIGHT:
        case 'l':
            if(x == WIDTH-1)
                x = -1;
            x++;
            break;
        case KEY_DOWN:
        case 'k':
            if(y == HEIGHT-1)
                y = -1;
            y++;
            break;
        case KEY_UP:
        case 'i':
            if(y == 0)
                y = HEIGHT;
            y--;
            break;
        case 'o':
        case '@':
            wprintw(window, "@");
            if(countFlags(window) == NUM_BOMBS && areAllMinesFlagged(window, mines)){
                running = false;
                mvprintw(HEIGHT+3, 0, "YOU WIN!");
            }
            break;
        case 'p':
        case '.':
            wprintw(window, ".");
            if(countFlags(window) == NUM_BOMBS && areAllMinesFlagged(window, mines))
        case ' ':
            if(winch(window) == '.' && !isOnMine(mines, y, x)){
                defuse(window, mines, y, x);
                firstTry = false;
            }
            else if(isOnMine(mines, y, x) && firstTry){
                firstTry = false;
                std::list<mine>::const_iterator it;
                // same procedure at remove random mine
                std::advance((it = mines.begin()), c);
                mines.erase(it);
                defuse(window, mines, y, x);

            }
            else if(isOnMine(mines, y, x)) {
                running = false;
                reveal(window, mines, "X");
                mvprintw(HEIGHT+3, 0, "GAME OVER!");
            }
            break;
        case ',':
            automaticWin(window, mines);
            break;
        case 'q':
            clear();
            running = false;
            break;
    }
    mvprintw(HEIGHT+2, 0, "coord x:%3d, y:%3d", x, y);
    wmove(window,y, x);
    refresh();
    wrefresh(window);

}

bool mine::operator<(const mine& other) const{
    if((this->x < other.x) && (this->y < other.y)) return true;
    else if(!(this->x < other.x) && !(this->y < other.y)) return false;
    else return this->x < other.x;
}

bool mine::operator==(const mine &other) const {
    return this->x == other.x && this->y == other.y;
}

std::list<mine> game::createMines(std::list<mine> &mines) {
    if(!mines.size()) mines.clear();
    for(int i = 0; i < HEIGHT; ++i){
        for(int j = 0; j < WIDTH; ++j){
            auto b = new mine(i,j);
            mines.push_back(*b);
            delete b;
        }
    }

    srand(time(NULL));
    std::list<mine>::iterator it;

    while(mines.size() != NUM_BOMBS){
        int c = rand()%mines.size();
        std::advance((it = mines.begin()), c);
        mines.erase(it);
    }

    return mines;
}

int game::isOnMine(std::list<mine> &mines, int y, int x) {
    std::list<mine>::const_iterator it;
    for(it = mines.begin(); it != mines.end(); it++){
        if(it->x == x && it->y == y) return 1;
    }
    return 0;
}
bool game::isWithinBoundary(int y, int x){
    return (y>=0 && y<HEIGHT) && (x>=0 && x < WIDTH);
}
void game::getMinesCount(WINDOW *window, std::list<mine> &mines, int y, int x) {

    if(isWithinBoundary(y, x) && mvwinch(window, y, x) == '.'){
        int count = isOnMine(mines, y - 1, x) + isOnMine(mines, y, x - 1) +
                isOnMine(mines, y + 1, x) + isOnMine(mines, y, x + 1) +
                isOnMine(mines, y - 1, x - 1) + isOnMine(mines, y + 1, x + 1) +
                isOnMine(mines, y - 1, x + 1) + isOnMine(mines, y + 1, x - 1);

        mvwprintw(window, y, x, "%d", count);
        //recursive loop will stop if encounter a position's char that is not '.'
        if(count == 0){
            //matches by pair
            getMinesCount(window, mines, y - 1, x);
            getMinesCount(window, mines, y, x - 1);

            getMinesCount(window, mines, y + 1, x);
            getMinesCount(window, mines, y, x + 1);

            getMinesCount(window, mines, y - 1, x - 1);
            getMinesCount(window, mines, y + 1, x + 1);

            getMinesCount(window, mines, y - 1, x + 1);
            getMinesCount(window, mines, y + 1, x - 1);
        }

    }
}

void game::defuse(WINDOW* window, std::list<mine>& mines, int y, int x) {
    getMinesCount(window, mines, y, x);
}

void game::reveal(WINDOW* window, std::list<mine> &mines, const char* c) {
    std::list<mine>::iterator it;
    for(it = mines.begin(); it != mines.end(); ++it){
        mvwprintw(window, it->y, it->x, c);
    }
}

bool game::areAllMinesFlagged(WINDOW *window, std::list<mine> &mines) {
    std::list<mine>::iterator it;
    for(it = mines.begin(); it != mines.end(); ++it){
        if(mvwinch(window, it->y, it->x) != '@') return false;
    }
    return true;
}

void game::automaticWin(WINDOW *window, std::list<mine> &mines) {
    reveal(window, mines, "@");
    if(areAllMinesFlagged(window, mines)) mvprintw(HEIGHT+3, 0, "YOU WIN!");
}

int game::countFlags(WINDOW *window) {
    int count = 0;
    for(int y = 0; y < HEIGHT; ++y){
        for(int x = 0; x < WIDTH; ++x){
            if(mvwinch(window, y, x) == '@') count++;
        }
    }
    return count;
}

