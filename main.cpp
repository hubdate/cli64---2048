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
    //printf("\n");
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


/*
    printf("\n# macros\n");
    printf("[x] GET_EPOCH_TIME                      :  %12s\n", GET_EPOCH_TIME);
    printf("[x] PRNG_UD(16)                         :  %d\n", PRNG_UD(16));

    printf("\n# board utils functions\n");
    printf("[x] board.empty_tiles(m)                :  %d\n", board.empty_tiles(m)    );
    printf("[x] board.distinct_tiles(m)             :  %d\n", board.distinct_tiles(m) );
    printf("[x] board.highest_vtiles(m)             :  %d\n", board.highest_vtiles(m) );

    printf("\n# board scores functions\n");
    printf("[x] board.get_scores(m)[0]              :  %f\n", board.get_scores(m)[0]  );
    printf("[x] board.get_scores(m)[1]              :  %f\n", board.get_scores(m)[1]  );

    printf("\n# matrix operations\n");
    printf("[x] board.reverse(0x0123)               :  %04x\n", board.reverse(0x0123) );
    printf("[x] board.unpack(0x0123)                :  %016llx\n", board.unpack(0x0123) );
    printf("[x] board.transpose(m)                  :  %016llx\n", board.transpose(m) );

    printf("\n# board moves operations\n");
    printf("[x] board.__set_gravity_right(m)        :  %016llx\n", board.__set_gravity_right(m) );
    printf("[x] board.__set_gravity_up(m)           :  %016llx\n", board.__set_gravity_up(m)    );
    printf("[x] board.__set_gravity_left(m)         :  %016llx\n", board.__set_gravity_left(m)  );
    printf("[x] board.__set_gravity_down(m)         :  %016llx\n", board.__set_gravity_down(m)  );


    printf("\n# game based functions\n");
    printf("[x] board.__spawn_tile(m)               :  %016llx\n", board.__spawn_tile(m)  );

    printf("\n# private methods\n");
    Position::state __state;
    printf("[x] board.evaluate(__state, m, 1.0f)    :  %0.f\n", board.evaluate(__state, m, 1.0f)  );
    printf("[x] board.get_bestmove(m)               :  %0.f\n", board.get_bestmove(__state, m, 1.0f)  );

    printf("\n# predict\n");
    //printf("[x] board.predict(m, false)              :  %d\n", board.predict(m, true)  );
    board.predict(m, true);

    printf("\n[%12s] %-10s\n", GET_EPOCH_TIME, __FUNCTION__);
*/