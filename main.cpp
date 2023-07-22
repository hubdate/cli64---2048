//
//
//
#include "position.h"

std::chrono::time_point<std::chrono::high_resolution_clock> __BIG_BANG__ = std::chrono::high_resolution_clock::now();

void print_board(Position::position_t board) {
    int i,j;
    for(i=0; i<4; i++) {
        for(j=0; j<4; j++) {
            uint8_t powerVal = (board) & 0xf;
            printf("%6u", (powerVal == 0) ? 0 : 1 << powerVal);
            board >>= 4;
        }
        printf("\n");
    }
}

int main() {
    Position game;
    Position::position_t board = 0x0000000000000000ULL;

    board = game.spawn_tile(board);
    

    Position::functionptr moves[] {
        &Position::set_gravity_right,
        &Position::set_gravity_up,
        &Position::set_gravity_left,
        &Position::set_gravity_down,
    };


    int iter = 0;

    while (true) {
        print_board(board);

        int __move;
        Position::position_t __board;

        for (__move = 0; __move < 4; __move++) {
            if ((game.*moves[__move])(board) != board) { break; }
        }
        if (__move == 4) { break; }

        printf("\n[%s] %-12s: Move #%d, current score: %.0f\n", GET_EPOCH_TIME, __FUNCTION__, ++iter, game.get_scores(board)[0]);
        __move = game.predict(board, true);
        __board = (game.*moves[__move])(board);

        board = game.spawn_tile(__board);
        printf("\n");    
    }
    printf("\n Game over. Your score is %.0f. The highest rank you achived was %d.\n", game.get_scores(board)[0], game.highest_vtiles(board));

    return 0;
}