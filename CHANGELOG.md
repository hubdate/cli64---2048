# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 0.0.1 (Unreleased)

### NEW FEATURES

- `[public] Position::set_gravity_right()`. The purpose of this function is to return the 64-bit integer baord with its "gravity" sets to "right", resulting in tile merging . For example, `set_gravity_right(0x0001000100010001)` returns `0x0001000100010001`.

- `[public] Position::set_gravity_up()`. The purpose of this function is to return the 64-bit integer baord with its "gravity" sets to the "up", resulting in tile merging . For example, `set_gravity_up(0x0001000100010001)` returns `0x0003000000000000`.

- `[public] Position::set_gravity_left()`. The purpose of this function is to return the 64-bit integer baord with its "gravity" sets to "left", resulting in tile merging . For example, `set_gravity_up(0x0001000100010001)` returns `0x1000100010001000`.

- `[public] Position::set_gravity_down()`. The purpose of this function is to return the 64-bit integer baord with its "gravity" sets to "down", resulting in tile merging . For example, `set_gravity_up(0x0001000100010001)` returns `0x0000000000000003`.

#### Private Functions

- `[private] Position::unpack()`. The purpose of this function is to unpack a 16-bit integer into a 64-bit integer by inserting 0 bits between each 4-bit nibble of the 16-bit integer. For example, `unpack(0x1234)` returns `0x0001000200030004`.

- `[private] Position::reverse()`. The purpose of this function is to reverse the order of the bits within each 4-bit nibble of the 16-bit integer. The function takes a 16-bit integer as input and returns the 16-bit integer with its nibbles' bit order reversed. For example, `reverse(0x1234)` returns `0x4321`.

- `[private] Position::transpose()`. The purpose of this function is to perform a matrix tranposition operation on a 64-bit integer representing a 4x4 matrix. The function isolates main diagonal, antidiagonal, upper-right, and lower-left elements, swaps them, and then transposes the middle rows and columns. For example, `transpose(0x0001000200030004)` returns `0x0000000000001234`.

- `[private] Position::count_empty_tiles()`. The purpose of this function is to return the number of empty tiles (4-bit nibbles) in a 64-bit integer by bitwise folding. For example, `empty_tiles(0x000D5000001AE008)` returns `6`.

- `[private] Position::count_distinct_tiles()`. The purpose of this function is to return the number of distinct tiles (4-bit nibbles) in a 64-bit integer (empty tiles does not count). For example, `empty_tiles(0xAA5D5ED11E1AE088)` returns `6`.

- `[private] Position::get_score()`. The purpose of ths function is to return the score of a 64-bit integer based on pre-calculated tables. The score is the sum of the values of all tiles (4-bit nibbles) in the 64-bit integer. For example, `get_score(0x000D5000001AE008, GAME_SCORE)` returns `322432.00000`.



<!-- ### ENHANCEMENTS -->
<!-- ### BUG FIXES -->
<!-- ### DEPRECATIONS -->
<!-- ### REMOVALS -->