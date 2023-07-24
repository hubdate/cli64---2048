# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 0.0.1 (2023-07-24)

### NEW FEATURES

`Position` class. The purpose of this class is to store all related functions, constants and variables needed for the 2048 game. The implementation approch relies on a single `64-bit unsigned integer (uint64_t)` to encode the entire board (4x4 matrix), where each tile is represented using a 4-bit nibble. On a x64 machine, this enables the entire board to be passed around in a single register.

Since this a 0-player game implementation, the `Position` class is designed to be used by a 3rd party AI algorithm. The AI algorithm is based on the `expectimax search`. The algorithm itself is coded as a recursive search wich alternates between **"expectation" steps** (testing all possible tile spawn location/values and weighting their optimized scores by the probabilty of each possibility) and **"maximization" steps** (testing all possible moves and selecting the one with the highest score). The tree search terminates when: it sees a previously-seen position; it reaches a predefined depth limit; or it reaches a position that is highly unlikely (e.g., getting 6 "4" tile in a row: 1e-6 chance of occurance).

#### **Public Functions**

- `Position::set_gravity_right()`. The purpose of this function is to return a board position (`64-bit unsigned int`) with its "gravity" sets to "right", resulting in tile merging . For example, `set_gravity_right(0x0001000100010001)` returns `0x0001000100010001`.

- `Position::set_gravity_up()`. The purpose of this function is to return a board position (`64-bit unsigned int`) with its "gravity" sets to "up", resulting in tile merging . For example, `set_gravity_up(0x0001000100010001)` returns `0x0003000000000000`.

- `Position::set_gravity_left()`. The purpose of this function is to return a board position (`64-bit unsigned int`) with its "gravity" sets to "left", resulting in tile merging . For example, `set_gravity_up(0x0001000100010001)` returns `0x1000100010001000`.

- `Position::set_gravity_down()`. The purpose of this function is to return a board position (`64-bit unsigned int`) with its "gravity" sets to "down", resulting in tile merging . For example, `set_gravity_up(0x0001000100010001)` returns `0x0000000000000003`.

- `Position::highest_vtile()`. The purpose of this function is to return the highest tile (4-bit nibble) value in a board position (`64-bit unsigned int`). For example, `highest_vtile(0x000D5000001AE008)` returns `15`.

- `Position::get_scores()`. The purpose of this function is to return both *game* and *heuristic* scores of a 64-bit integer,based on pre-calculated tables (values are encapsulated in `std::array<float, 2>`). For example, `get_scores(0x000D5000001AE008)` returns `[322432.00000, 405718.218750]`.

- `Position::spawn_tile()`. The purpose of this function is to return a board position (`64-bit unsigned int`), with a new tile (4-bit nibble) randomly spawned on an previous empty location. The tile value can be `0x1` (90%) or `0x2` (10%). For example, `spawn_tile(0x000D5000001AE008)` returns `0x100D5000001AE088`.

- `Position::predict()`. The purpose of this function is to return the best move ('right', 'up', 'left' and 'down') as indexed int for a given (`64-bit unsigned int`). It calls the `Position::evaluate()` function to recursively explore future states and find the best heuristic score, wich will determine the best move to play.

#### **Private Functions**

- `Position::unpack()`. The purpose of this function is to unpack a 16-bit integer into a 64-bit integer by inserting 0 bits between each 4-bit nibble of the 16-bit integer. For example, `unpack(0x1234)` returns `0x0001000200030004`.

- `Position::reverse()`. The purpose of this function is to reverse the order of the bits within each 4-bit nibble of the 16-bit integer. The function takes a 16-bit integer as input and returns the 16-bit integer with its nibbles' bit order reversed. For example, `reverse(0x1234)` returns `0x4321`.

- `Position::transpose()`. The purpose of this function is to perform a matrix tranposition operation on a 64-bit integer representing a 4x4 matrix. The function isolates main diagonal, antidiagonal, upper-right, and lower-left elements, swaps them, and then transposes the middle rows and columns. For example, `transpose(0x0001000200030004)` returns `0x0000000000001234`.

- `Position::count_empty_tiles()`. The purpose of this function is to return the number of empty tiles (4-bit nibbles) in a 64-bit integer by bitwise folding. For example, `empty_tiles(0x000D5000001AE008)` returns `6`.

- `Position::count_distinct_tiles()`. The purpose of this function is to return the number of distinct tiles (4-bit nibbles) in a 64-bit integer (empty tiles does not count). For example, `empty_tiles(0xAA5D5ED11E1AE088)` returns `6`.

- `Position::get_score()`. The purpose of ths function is to return the score of a board position (`64-bit unsigned int`) based on pre-calculated tables. The score is the sum of the values of all tiles (4-bit nibbles) in the board position. For example, `get_score(0x000D5000001AE008, GAME_SCORE)` returns `322432.00000`.

- `Position::evaluate() [part of the expetimax search]`. The purpose of this function is to return the best heuristic score of a given board position (`64-bit unsigned int`). It iterates through all possible spawnable tile positions and recursively explore all possible moves by calling the `Position::get_best_heuristic()` function to find the best heuristic score.

- `Position::get_best_heuristic() [part of the expetimax search]`. The purpose of this function is to return the best heuristic score of all possible moves ('right', 'up', 'left' and 'down') for a given board position (`64-bit unsigned int`). It calls the `Position::evaluate()` function to recursively explore future states and find the best heuristic score.



<!-- ### ENHANCEMENTS -->
<!-- ### BUG FIXES -->
<!-- ### DEPRECATIONS -->
<!-- ### REMOVALS -->