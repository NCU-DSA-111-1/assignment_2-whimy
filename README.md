# Simple Shogi

## With basic rules

* Shogi
* Include read file
* Usage of Linked-List

-------------------------------------
# Instruction

> There are some instrucion that the user need to enter them manually 
> 
> Which will be display by code block above

## Compile

```sh
make
```
```sh
cd bin
```
## Run
### Open a new game
```sh
./run –n –s {new_game_file_name}
```
### Reload old game file
```sh
./run –l {old_game_file_name}
```
## How to play
1. The goma board will display on the terminal.

   Please enter the coordination to move goma.

   (Remember to follow each goma's rule)

   After deciding each position , enter 'y' for certain. If you wanna re-select please enter 'n'.

### example
```sh
[Turn n]
[Player ?]
Please enter the initial position:1 7
Select 步 [1][7]


Please enter the new position:1 6
Select 空 [1][6]

步 [1][7]->[1][6]?

Would you like to re-select?[y/n]:
```
2. The goma board will display on the terminal again after making moves.

3. If you have eaten others goma, it will be place in your mochi bag.

4. After running for 3 turns, you can make choice of whether you wanna to regret your last movement.

   Enter 0 for regret.(If you don't want to, just enter any number but not 0.)

5. When any player's King has been eaten, then lose.

6. All playing data will be record in a csv file named {new_game_file_name}, which user entered while running the program.

# Reference
[Shogi rule](https://shogi.hk/Gameplay-of-Japanese-Chess-Shogi)

> Basic Shogi rule

[Print out with color in C](https://www.796t.com/article.php?id=190246)

> Tutorial of colorful output in C

[png](https://github.com/cutechess/sloppy/blob/master/src/pgn.c)

> Open-Source of png

[Libev](https://gohalo.me/post/linux-libev.html)

> Libev introduction
 
[Redis](https://github.com/redis/hiredis)

> Redis introduction
