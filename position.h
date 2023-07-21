// -*- LSST-C++ -*-
//
//
//
#ifndef __POSITION_H__
#define __POSITION_H__

#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <array>
#include <map>

#include <chrono>



extern std::chrono::time_point<std::chrono::high_resolution_clock> __BIG_BANG__;
#define GET_EPOCH_TIME ([&]() { \
    long long int __ctime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - __BIG_BANG__).count(); \
    static char __buffer[13]; \
    snprintf(__buffer, sizeof(__buffer), "%5lld.%06lld", (__ctime / 1000000LL), (__ctime % 1000000LL)); \
    return __buffer; \
})()



/**
 * Pseudo Random Number Generator Uniformly Distributed (PRNG UD) 
 * returns a value within the interval [0, n[
 *  
 * Acknowledgment: Each time the method is called, teh generator is 
 *                 reseeded based on large number of entropy sources.
 */

#define PRNG_UD(N) ([&]() { \
        uint64_t entropy = 0; \
        std::chrono::high_resolution_clock::duration dur = std::chrono::high_resolution_clock::now().time_since_epoch(); \
        entropy ^= static_cast<uint64_t>(dur.count()); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy)); \
        entropy ^= static_cast<uint64_t>(__LINE__); \
        entropy ^= static_cast<uint64_t>(__FILE__[0]); \
        entropy ^= static_cast<uint64_t>(__FUNCTION__[0]); \
        entropy ^= static_cast<uint64_t>(std::chrono::high_resolution_clock::period::den); \
        entropy ^= static_cast<uint64_t>(std::chrono::high_resolution_clock::period::num); \
        entropy ^= static_cast<uint64_t>(__TIME__[0]); \
        entropy ^= static_cast<uint64_t>(__DATE__[0]); \
        entropy ^= static_cast<uint64_t>(__cplusplus); \
        entropy ^= static_cast<uint64_t>(sizeof(std::chrono::high_resolution_clock::rep)); \
        entropy ^= static_cast<uint64_t>(sizeof(std::chrono::high_resolution_clock::period)); \
        entropy ^= static_cast<uint64_t>(sizeof(void*)); \
        entropy ^= static_cast<uint64_t>(sizeof(size_t)); \
        entropy ^= static_cast<uint64_t>(sizeof(std::chrono::high_resolution_clock::time_point)); \
        entropy ^= static_cast<uint64_t>(__FILE__[1]); \
        entropy ^= static_cast<uint64_t>(__FUNCTION__[1]); \
        entropy ^= static_cast<uint64_t>(__TIME__[1]); \
        entropy ^= static_cast<uint64_t>(__DATE__[1]); \
        entropy ^= static_cast<uint64_t>(__cplusplus * 1000); \
        entropy ^= static_cast<uint64_t>(sizeof(std::chrono::high_resolution_clock::duration)); \
        entropy ^= static_cast<uint64_t>(sizeof(std::chrono::high_resolution_clock::time_point::rep)); \
        entropy ^= static_cast<uint64_t>(sizeof(std::chrono::high_resolution_clock::period::den)); \
        entropy ^= static_cast<uint64_t>(sizeof(std::chrono::high_resolution_clock::period::num)); \
        entropy ^= static_cast<uint64_t>(sizeof(__TIME__[0])); \
        entropy ^= static_cast<uint64_t>(sizeof(__DATE__[0])); \
        entropy ^= static_cast<uint64_t>(sizeof(__cplusplus)); \
        entropy ^= static_cast<uint64_t>(sizeof(__FILE__[0])); \
        entropy ^= static_cast<uint64_t>(sizeof(__FUNCTION__[0])); \
        entropy ^= static_cast<uint64_t>(sizeof(__LINE__)); \
        entropy ^= static_cast<uint64_t>(sizeof(__TIME__[1])); \
        entropy ^= static_cast<uint64_t>(sizeof(__DATE__[1])); \
        entropy ^= static_cast<uint64_t>(sizeof(__cplusplus * 1000)); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy)); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x00000000FFFFFFFFULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0xFFFFFFFF00000000ULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x000000000000FFFFULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0xFFFF000000000000ULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x00000000000000FFULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0xFF00000000000000ULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x000000000000000FULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0xF000000000000000ULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x0000000000000003ULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x3000000000000000ULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x0000000000000001ULL); \
        entropy ^= static_cast<uint64_t>(reinterpret_cast<uintptr_t>(&entropy) & 0x1000000000000000ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x1000000000000000ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x0000000000000001ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x0000000000000003ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x3000000000000000ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x000000000000000FULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0xF000000000000000ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x00000000000000FFULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0xFF00000000000000ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x000000000000FFFFULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0xFFFF000000000000ULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0x00000000FFFFFFFFULL); \
        entropy ^= static_cast<uint64_t>((rand() % entropy) & 0xFFFFFFFF00000000ULL); \
        srand(entropy); \
        return static_cast<uint64_t>(rand() % (N)); \
})()

/**
 * 
 * board representation in 64 bits:
 * ([0000] [0000]) ([0000] [0000])
 * ([0000] [0000]) ([0000] [0000])
 * ([0000] [0000]) ([0000] [0000])
 * ([0000] [0000]) ([0000] [0000])
 * 
 * board representation in hexadecimal:
 *  [0] [1] [2] [3]
 *  [4] [5] [6] [7]
 *  [8] [9] [A] [B]
 *  [C] [D] [E] [F]
 */

/**
 * A class storing the a 2048 position.
 * 
 * The entire board is encoded on a 64-bit integer (where the 16 tiles are 
 * encoded on 4 bits chunck each) [bitmap representation]. The allow on modern
 * CPU (64-bit) to load the entire board in a single register.
 * 
 * Example of 4-bit chunck order to encode the board:
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  A  B
 * C  D  E  F
 * 
 */
class Position {
    public:
        typedef uint64_t position_t;

        typedef position_t (Position::*functionptr)(position_t) const;              // Function pointer

        position_t              set_gravity_right(position_t) const;                // Return the board, with "gravity" applied to right (merge tiles)
        position_t              set_gravity_up(position_t) const;                   // Return the board, with "gravity" applied to up    (merge tiles)
        position_t              set_gravity_left(position_t) const;                 // Return the board, with "gravity" applied to left  (merge tiles)
        position_t              set_gravity_down(position_t) const;                 // Return the board, with "gravity" applied to down  (merge tiles)

        int                     highest_vtiles(position_t) const;                   // Return the board highest tile value
        
        int                     predict(position_t, bool) const;                    // Return the best move [indexed] for a given board
        position_t              spawn_tile(position_t) const;                       // Return the board with a new tile randomly added (2 or 4)
        std::array<float, 2>    get_scores(position_t) const;                       // Return both heuristic and game score of a board

        Position();                                                                 // Constructor

    private:
        struct transtable
        {
            uint8_t depth;
            float heuristic;
        };

        typedef uint16_t row_t;
        typedef std::map<position_t, transtable> transtable_t;

        struct scores_t 
        {
            float game      [0x10000];
            float heuristic [0x10000];
        };

        struct cache_tables_t
        {
            Position::row_t      lrow       [0x10000];
            Position::row_t      rrow       [0x10000];
            Position::position_t ucolumn    [0x10000];
            Position::position_t dcolumn    [0x10000];
        };

        struct state_t 
        {
            transtable_t ttable;
            int current_depth;
            int limit_depth;
            int max_depth;
            int cache_hits;
            unsigned long nevaluated_moves;

            state_t() : current_depth(0), limit_depth(0), max_depth(0), cache_hits(0), nevaluated_moves(0) {}
        };


        static constexpr int   WIDTH_n_HEIGHT  = 4;                                 // Board width/height
        static constexpr float LOST_PENALTY         = 200000.0f;                    // Heuristic scoring constant
        static constexpr float MONOTONICITY_POWER   = 4.0f;                         // Heuristic scoring constant
        static constexpr float MONOTONICITY_WEIGHT  = 47.0f;                        // Heuristic scoring constant
        static constexpr float SUM_POWER            = 3.5f;                         // Heuristic scoring constant
        static constexpr float SUM_WEIGHT           = 11.0f;                        // Heuristic scoring constant
        static constexpr float MERGES_WEIGHT        = 700.0f;                       // Heuristic scoring constant
        static constexpr float EMPTY_WEIGHT         = 270.0f;                       // Heuristic scoring constant

        static constexpr float CPROB_THRESHOLD_BASE = 0.0001f;                      // Cumulative probability threshold base, avoid recursing into a tile with a cprob lower than this limit
        static constexpr int   CACHE_DEPTH_LIMIT    = 15;                           // Cache depth limit, avoid recursing into a tile with a depth higher than this limit

        static float GAME_SCORE      [0x10000];                                     // Store row [  game   ] score, since a row is 16-bits long, then 2^(16) = 65536 entries [0, 65535]
        static float HEURISTIC_SCORE [0x10000];                                     // Store row [heuristic] score, since a row is 16-bits long, then 2^(16) = 65536 entries [0, 65535]

        static constexpr position_t ROW_MASK = 0xFFFFULL;
        static constexpr position_t COLUMN_MASK = 0x000F000F000F000FULL;


        scores_t scores;
        cache_tables_t cache_tables;

        position_t              unpack(row_t) const;                                // Unpack/Flatten a 16-bits integer into a 64-bits integer (spacing with 0's)
        row_t                   reverse(row_t) const;                               // Reverse/Mirror a 16-bits integer
        position_t              transpose(position_t) const;                        // Transpose a given 64-bits integer

        int                     count_empty_tiles(position_t) const;                      // Return the number of 0's 4-bits nibbles on a 64-bits integer
        int                     count_distinct_tiles(position_t) const;                   // Return the number of distinct 4-bits nibbles on a 64-bits integer
        float                   get_score(position_t, const float *) const;         // Return the score of a board based on a given score array
        float                   evaluate(state_t &, position_t, float) const;       // Return the heuristic score of a board (64-bits integer), recursively with get_bestmove() method
        float                   get_best_heuristic(state_t &, position_t, float) const;   // Return the heuristic score of a board (64-bits integer), recursively with evaluate()     method

};

#endif // __POSITION_H__