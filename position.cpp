//
// Compiling with G++:
//   g++ -std=c++11 -O3 -Wall -Wextra -pedantic -o position position.cpp
//
//   cd "c:\Users\acer\Desktop\github repositories (local)\2048 on a 64bits board\" ; if ($?) { g++ -O3 -Wall -Wextra -pedantic main.cpp position.cpp -o main } ; if ($?) { .\main }
//
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "position.h"


Position::Position() {
    for (unsigned row = 0; row <= 0xFFFF; ++row) {
        unsigned column[WIDTH_n_HEIGHT] = {
            (row >> 0)  & 0xF,
            (row >> 4)  & 0xF,
            (row >> 8)  & 0xF,
            (row >> 12) & 0xF
        };


        // Game score
        float score = 0.0f;
        for (unsigned cell = 0; cell < WIDTH_n_HEIGHT; ++cell) {
            unsigned rank = column[cell];
            if (rank >= 2) {
                score += (rank - 1) * (1 << rank);
            }
        }
        scores.game[row] = score;

        // Heuristic score
        float sum = 0.0f;
        int empty = 0;
        int merges = 0;

        int previous = 0;
        int counter = 0;

        for (unsigned cell = 0; cell < WIDTH_n_HEIGHT; ++cell) {
            int rank = column[cell];
            sum += pow(rank, SUM_POWER);
            if (rank == 0) { empty++; } 
            
            else {
                if (previous == rank) { counter++; } 
                else if (counter > 0) {
                    merges += 1 + counter;
                    counter = 0;
                }
                
                previous = rank;
            }
        }
        if (counter > 0) { merges += 1 + counter; }

        float monotonicity_left = 0.0f;
        float monotonicity_right = 0.0f;
        for (unsigned cell = 1; cell < WIDTH_n_HEIGHT; ++cell) {
            if (column[cell -1] > column[cell]) { monotonicity_left +=  pow(column[cell - 1], MONOTONICITY_POWER) - pow(column[cell],     MONOTONICITY_POWER); }
            else                                { monotonicity_right += pow(column[cell],     MONOTONICITY_POWER) - pow(column[cell - 1], MONOTONICITY_POWER); }
        }

        scores.heuristic[row] = LOST_PENALTY
            + EMPTY_WEIGHT  * empty
            + MERGES_WEIGHT * merges
            - MONOTONICITY_WEIGHT * std::min(monotonicity_left, monotonicity_right)
            - SUM_WEIGHT * sum
        ;

        // Execute a single move to the left (apply gravity on left side)
        for (uint8_t __n = 0; __n < (WIDTH_n_HEIGHT - 1); ++__n) {
            uint8_t __nplusone;
            for (__nplusone = (__n + 1); __nplusone < WIDTH_n_HEIGHT; __nplusone++) {
                if (column[__nplusone] != 0) { break; }
            }
            if (__nplusone >= WIDTH_n_HEIGHT) { break; } // No more cell to shift

            if (column[__n] == 0) { // Left shift __nplusone to __n
                column[__n] = column[__nplusone];
                column[__nplusone] = 0;
                __n--;
            } else if (column[__n] == column[__nplusone]) {
                if (column[__n] != 0xF) { column[__n]++; }  // Assume that 2^16 + 2^16 = 2^16 (representation limit)
                column[__nplusone] = 0;
            }
        }

        row_t result =  (column[0] <<  0) 
                      | (column[1] <<  4)
                      | (column[2] <<  8)
                      | (column[3] << 12);

        row_t tluser = reverse(result);
        unsigned wor = reverse(row);

        cache_tables.rrow    [row] =        row  ^        result ;
        cache_tables.ucolumn [wor] = unpack(wor) ^ unpack(tluser);
        cache_tables.lrow    [wor] =        wor  ^        tluser ;       
        cache_tables.dcolumn [row] = unpack(row) ^ unpack(result);
    }
}



/** 
 * ---------------------
 * --- Binary format ---
 * ---------------------
 * 
 * Detailled explanation of the unpack function with the following matrix:
 * 
 *                          |0000 0000 0000 0000|
 * |0000 0001 0010 0011| -> |0000 0000 0000 0001|
 *                          |0000 0000 0000 0010|
 *                          |0000 0000 0000 0011|
 * 
 */
Position::position_t Position::unpack(Position::row_t r) const {
    position_t _ = r;
    return (_ | _<<12ULL | _<<24ULL | _<<36ULL) & COLUMN_MASK;
}

/** 
 * ---------------------
 * --- Binary format ---
 * ---------------------
 * 
 * Detailled explanation of the reverse function with the following matrix:
 * 
 * |0000 0001 0010 0011| -> |0011 0010 0001 0000|
 * 
 */
Position::row_t Position::reverse(Position::row_t r) const {
    return 
           (r >> 12)
        | ((r >>  4) & static_cast<unsigned short>(0x00F0))
        | ((r <<  4) & static_cast<unsigned short>(0x0F00))
        |  (r << 12);
}

/** 
 * ---------------------
 * --- Binary format ---
 * ---------------------
 * 
 * Detailled explanation of the transpose function with the following matrix:
 * |0000 0001 0010 0011|
 * |0000 0001 0010 0011| 
 * |0000 0001 0010 0011|
 * |0000 0001 0010 0011|
 * 
 * ---------------------------------------------------------------------------------------------------------------                                                          
 *    |0000 0001 0010 0011|       |1111 0000 1111 0000|    |0000 0000 0010 0000|
 *    |0100 0101 0110 0111| (AND) |0000 1111 0000 1111| -> |0000 0101 0000 0111|  
 *    |1000 1001 1010 1011|       |1111 0000 1111 0000|    |1000 0000 1010 0000|
 *    |1100 1101 1110 1111|       |0000 1111 0000 1111|    |0000 1101 0000 1111|
 *    \___________________/       \___________________/    \___________________/
 *      input matrix (m)                   mask                     (a1)
 * ---------------------------------------------------------------------------------------------------------------
 *    |0000 0001 0010 0011|       |0000 0000 0000 0000|    |0000 0000 0000 0000|         |0000 0100 0000 0110|
 *    |0100 0101 0110 0111| (AND) |1111 0000 1111 0000| -> |0100 0000 0110 0000| (<< 12) |0000 0000 0000 0000|
 *    |1000 1001 1010 1011|       |0000 0000 0000 0000|    |0000 0000 0000 0000|         |0000 1100 0000 1110|
 *    |1100 1101 1110 1111|       |1111 0000 1111 0000|    |1100 0000 1110 0000|         |0000 0000 0000 0000|
 *    \___________________/       \___________________/    \___________________/         \___________________/
 *      input matrix (m)                   mask                     (a2)                       (a2 << 12)
 * ---------------------------------------------------------------------------------------------------------------
 *    |0000 0001 0010 0011|       |0000 1111 0000 1111|    |0000 0001 0000 0011|         |0000 0000 0000 0000|
 *    |0100 0101 0110 0111| (AND) |0000 0000 0000 0000| -> |0000 0000 0000 0000| (>> 12) |0001 0000 0011 0000|
 *    |1000 1001 1010 1011|       |0000 1111 0000 1111|    |0000 1001 0000 1011|         |0000 0000 0000 0000|
 *    |1100 1101 1110 1111|       |0000 0000 0000 0000|    |0000 0000 0000 0000|         |1001 0000 1011 0000|
 *    \___________________/       \___________________/    \___________________/         \___________________/
 *       input matrix (m)                  mask                      (a3)                      (a3 >> 12)
 * --------------------------------------------------------------------------------------------------------------- 
 *    |0000 0000 0010 0000|      |0000 0100 0000 0110|      |0000 0000 0000 0000|    |0000 0100 0010 0110|
 *    |0000 0101 0000 0111| (OR) |0000 0000 0000 0000| (OR) |0001 0000 0011 0000| -> |0001 0101 0011 0111|
 *    |1000 0000 1010 0000|      |0000 1100 0000 1110|      |0000 0000 0000 0000|    |1000 1100 1010 1110|
 *    |0000 1101 0000 1111|      |0000 0000 0000 0000|      |1001 0000 1011 0000|    |1001 1101 1011 1111|
 *    \___________________/      \___________________/      \___________________/    \___________________/
 *             (a1)                    (a2 << 12)                 (a3 >> 12)                  (a)
 * ---------------------------------------------------------------------------------------------------------------  
 *    |0000 0100 0010 0110|       |1111 1111 0000 0000|    |0000 0100 0000 0000|
 *    |0001 0101 0011 0111| (AND) |1111 1111 0000 0000| -> |0001 0101 0000 0000|
 *    |1000 1100 1010 1110|       |0000 0000 1111 1111|    |0000 0000 1010 1110|
 *    |1001 1101 1011 1111|       |0000 0000 1111 1111|    |0000 0000 1011 1111|
 *    \___________________/       \___________________/    \___________________/
 *             (a)                         mask                    (b1) 
 * ---------------------------------------------------------------------------------------------------------------
 *    |0000 0100 0010 0110|       |0000 0000 1111 1111|    |0000 0000 0010 0110|         |0000 0000 0000 0000|
 *    |0001 0101 0011 0111| (AND) |0000 0000 1111 1111| -> |0000 0000 0011 0111| (>> 24) |0000 0000 0000 0000|
 *    |1000 1100 1010 1110|       |0000 0000 0000 0000|    |0000 0000 0000 0000|         |0010 0110 0000 0000|
 *    |1001 1101 1011 1111|       |0000 0000 0000 0000|    |0000 0000 0000 0000|         |0011 0111 0000 0000|
 *    \___________________/       \___________________/    \___________________/         \___________________/
 *             (a)                         mask                     (b2)                       (b2 >> 24)
 * ---------------------------------------------------------------------------------------------------------------
 *    |0000 0100 0010 0110|       |0000 0000 0000 0000|    |0000 0000 0000 0000|         |0000 0000 1000 1100|
 *    |0001 0101 0011 0111| (AND) |0000 0000 0000 0000| -> |0000 0000 0000 0000| (<< 24) |0000 0000 1001 1101|
 *    |1000 1100 1010 1110|       |1111 1111 0000 0000|    |1000 1100 0000 0000|         |0000 0000 0000 0000|
 *    |1001 1101 1011 1111|       |1111 1111 0000 0000|    |1001 1101 0000 0000|         |0000 0000 0000 0000|
 *    \___________________/       \___________________/    \___________________/         \___________________/
 *             (a)                         mask                     (b3)                       (b3 << 24)
 * ---------------------------------------------------------------------------------------------------------------
 *    |0000 0100 0000 0000|      |0000 0000 0000 0000|      |0000 0000 1000 1100|    |0000 0100 1000 1100|
 *    |0001 0101 0000 0000| (OR) |0000 0000 0000 0000| (OR) |0000 0000 1001 1101| -> |0001 0101 1001 1101|
 *    |0000 0000 1010 1110|      |0010 0110 0000 0000|      |0000 0000 0000 0000|    |0010 0110 1010 1110|
 *    |0000 0000 1011 1111|      |0011 0111 0000 0000|      |0000 0000 0000 0000|    |0011 0111 1011 1111|
 *    \___________________/      \___________________/      \___________________/    \___________________/
 *             (b1)                    (b2 >> 24)                 (b3 << 24)         transposed matrix (m)
 */
Position::position_t Position::transpose(Position::position_t m) const{
    position_t _ = (m & 0xF0F00F0FF0F00F0FULL) | ((m & 0x0000F0F00000F0F0ULL) << 12) | ((m & 0x0F0F00000F0F0000ULL) >> 12);
    return         (_ & 0xFF00FF0000FF00FFULL) | ((_ & 0x00FF00FF00000000ULL) >> 24) | ((_ & 0x00000000FF00FF00ULL) << 24);
}


/**
 * Count the number of empty cells (= zero niblles, 4bits) in a board.
 * It use the bitwise folding method to count.
 * 
 * Precondition: the board cannot be fully empty(=0). 
 * 
 * /!\ Revisionned, accept empty board...
 * 
 */
int Position::count_empty_tiles(position_t m) const {
    if (!m) { return 0x10; }

    m =  ((m >> 2) | m) & 0x3333333333333333ULL;
    m = ~((m >> 1) | m) & 0x1111111111111111ULL;

    m += m >> 32;
    m += m >> 16;
    m += m >>  8;
    m += m >>  4;

    return (m & 0x000000000000000FULL);
}

/**
 * Count the number of distinct cells  in a board.
 * 
 */
int Position::count_distinct_tiles(Position::position_t m) const {
    uint16_t bitset = 0;
    while (m) {
        bitset |= 1<<(m & 0xf);
        m >>= 4;
    }

    // Don't count empty tiles.
    bitset >>= 1;

    int count = 0;
    while (bitset) {
        bitset &= bitset - 1;
        count++;
    }
    return count;
}

/** 
 * Return the highest tile value on the given board.
 * 
 * Aknowledgement: The max research is perform in reverse index order,
 *                 from cell [F] all the way up to cell [0].
 */
int Position::highest_vtiles(Position::position_t b) const {
    int __max = 0;
    while (b) {
        __max = std::max(__max, int(b & 0xF));
        b >>= 4;
    }

    return __max;
}



/**
 * Compute the sum of each row of a board, to return based on a 
 * specific cache table the score of a board.
 * 
 */
float Position::get_score(Position::position_t m, const float *ctable) const {
    return (
          ctable[(m >>  0) & ROW_MASK]
        + ctable[(m >> 16) & ROW_MASK]
        + ctable[(m >> 32) & ROW_MASK]
        + ctable[(m >> 48) & ROW_MASK]
    );
}

/**
 * Return the scores of a board. 
 * 
 * Return type : std::array<float, 2>
 * Return format:
 *      __array[0] : The board score, displayed score
 *      __array[1] : The board heuristic score, the score
 *                   needed to compute the next moves
 */
std::array<float, 2> Position::get_scores(Position::position_t m) const {
    return std::array<float, 2> {
        {
              get_score(          m , scores.game),

              get_score(          m , scores.heuristic)
            + get_score(transpose(m), scores.heuristic)
        }
    };
}


Position::position_t Position::set_gravity_right(Position::position_t m) const {
    position_t __board = m;
    __board ^= (position_t)(cache_tables.rrow   [(m       >>  0) & ROW_MASK]) <<  0;
    __board ^= (position_t)(cache_tables.rrow   [(m       >> 16) & ROW_MASK]) << 16;
    __board ^= (position_t)(cache_tables.rrow   [(m       >> 32) & ROW_MASK]) << 32;
    __board ^= (position_t)(cache_tables.rrow   [(m       >> 48) & ROW_MASK]) << 48;

    return __board;
}

Position::position_t Position::set_gravity_up(Position::position_t m) const {
    position_t __board = m;
    position_t __trans = transpose(m);
    __board ^=             cache_tables.ucolumn[(__trans >>  0) & ROW_MASK]  <<  0;
    __board ^=             cache_tables.ucolumn[(__trans >> 16) & ROW_MASK]  <<  4;
    __board ^=             cache_tables.ucolumn[(__trans >> 32) & ROW_MASK]  <<  8;
    __board ^=             cache_tables.ucolumn[(__trans >> 48) & ROW_MASK]  << 12;

    return __board;
}

Position::position_t Position::set_gravity_left(Position::position_t m) const {
    position_t __board = m;
    __board ^= (position_t)(cache_tables.lrow   [(m       >>  0) & ROW_MASK]) <<  0;
    __board ^= (position_t)(cache_tables.lrow   [(m       >> 16) & ROW_MASK]) << 16;
    __board ^= (position_t)(cache_tables.lrow   [(m       >> 32) & ROW_MASK]) << 32;
    __board ^= (position_t)(cache_tables.lrow   [(m       >> 48) & ROW_MASK]) << 48;

    return __board;
}

Position::position_t Position::set_gravity_down(Position::position_t m) const {
    position_t __board = m;
    position_t __trans = transpose(m);
    __board ^=             cache_tables.dcolumn[(__trans >>  0) & ROW_MASK] <<  0;
    __board ^=             cache_tables.dcolumn[(__trans >> 16) & ROW_MASK] <<  4;
    __board ^=             cache_tables.dcolumn[(__trans >> 32) & ROW_MASK] <<  8;
    __board ^=             cache_tables.dcolumn[(__trans >> 48) & ROW_MASK] << 12;

    return __board;
}


Position::position_t Position::spawn_tile(Position::position_t board) const {
    int location = PRNG_UD(count_empty_tiles(board));
    position_t __explorer = board;
    position_t __tile = (PRNG_UD(10) < 9) ? 0x1ULL : 0x2ULL;

    while (true) {
        while ((__explorer & 0xFULL) != 0) {
            __explorer >>= 4;
            __tile     <<= 4;
        }
        if (location == 0) { break; }

        --location;
        __explorer >>= 4;
        __tile     <<= 4;
    }

    return board | __tile;
}


float Position::evaluate(Position::state_t &state, Position::position_t board, float cprob) const {
    if (cprob < CPROB_THRESHOLD_BASE || state.current_depth >= state.limit_depth) {
        state.max_depth = std::max(state.current_depth, state.max_depth);
        
        return get_scores(board)[1]; // Returns the heuristic score of the current board;
    }

    if (state.current_depth < CACHE_DEPTH_LIMIT) {
        const transtable_t::iterator &iter = state.ttable.find(board);
        if (iter != state.ttable.end()) {
            transtable __table = iter->second;
            /**
             * Returns heuristic from transposition table only if it means taht
             * the node will have been evaluated to a minimum depth of DEPTH_LIMIT.
             * This will result in slightly fewer cache hits, but should not impact
             * the strenght of the AI (negatively).
             */
            if (__table.depth <= state.current_depth) {
                state.cache_hits++;
                return __table.heuristic;
            }
        }
    }

    int nullcells = count_empty_tiles(board);
    cprob /= nullcells;

    float __heuristic = 0.0f;
    position_t __explorer = board;
    position_t __cell_2 = 0x1ULL; // A 2 cell on the board
    position_t __cell_4 = 0x2ULL; // A 4 cell on the board

    while (__cell_2) {
        if ((__explorer & 0xFULL) == 0) {
            __heuristic += get_best_heuristic(state, board | __cell_2, cprob * 0.9f) * 0.9f;
            __heuristic += get_bestmove_heuristic(state, board | __cell_4, cprob * 0.1f) * 0.1f;
        }
        __explorer >>= 4;
        __cell_2   <<= 4;
        __cell_4   <<= 4;
    }

    __heuristic /= nullcells;
    if (state.current_depth < CACHE_DEPTH_LIMIT) {
        transtable __table = {
            static_cast<uint8_t>(state.current_depth),
            static_cast<float>  (__heuristic)
        };

        state.ttable[board] = __table;
    }

    return __heuristic;
}


float Position::get_best_heuristic(Position::state_t &state, Position::position_t board, float cprob) const {
    float __best = 0.0f;
    state.current_depth++;

    functionptr moves[] {
        &Position::set_gravity_right,
        &Position::set_gravity_up,
        &Position::set_gravity_left,
        &Position::set_gravity_down
    };

    for (unsigned move = 0; move < 4; ++move) {
        position_t __to_evaluate = (this->*moves[move])(board);
        state.nevaluated_moves++;

        if (board != __to_evaluate) {
            __best = std::max(__best, evaluate(state, __to_evaluate, cprob));
        }
    }
    state.current_depth--;

    return __best;
}

int Position::predict(Position::position_t board, bool logs) const {
    float __best = 0.0f;
    int __move = -1;

    functionptr moves[] {
        &Position::set_gravity_right,
        &Position::set_gravity_up,
        &Position::set_gravity_left,
        &Position::set_gravity_down,
    };


    if (logs) { printf("[%s] %-12s: # Exploring possible moves to find the highest-scoring option... \n", GET_EPOCH_TIME, __FUNCTION__); }
    for (unsigned move = 0; move < 4; ++move) {
        float __score = 0.0f;
        Position::state_t __state;
        std::chrono::time_point<std::chrono::high_resolution_clock> counter_dummy, counter_stamp;

        __state.limit_depth = std::max(3, count_distinct_tiles(board) - 2);

        counter_dummy = std::chrono::high_resolution_clock::now();
        if ((this->*moves[move])(board) != board) { __score = evaluate(__state, (this->*moves[move])(board), 1.0f) + 1e-6; }
        counter_stamp = std::chrono::high_resolution_clock::now();

        if (logs) {
            printf("[%s]               # %d >--> Heur. score %12.4f, statistics - eval'd %7ld moves (%7d cache hits, %7d cache size) in %5lld ms   (maxdepth=%2d) \n",
                GET_EPOCH_TIME,
                move,
                __score,
                __state.nevaluated_moves,
                __state.cache_hits,
                (int)__state.ttable.size(),
                std::chrono::duration_cast<std::chrono::milliseconds>(counter_stamp - counter_dummy).count(),
                __state.max_depth
            );
        }

        if (__score > __best) {
            __best = __score;
            __move = move;
        }
    }

    return __move;
}
