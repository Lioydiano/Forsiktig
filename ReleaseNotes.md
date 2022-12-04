# Release Notes

Here you can find the release notes for each version of `Forsiktig` since `v1`.

## `v1.7.1` (4 December 2022)

The `YWSE` randomly respawns after its death, with a probability of `0.1%` for each frame, reduced by `-90%` compared to previous patch (`v1.7.0`).

```c++
#define PROBABILITY_OF_ENEMY_APPEARING 0.02
#define PROBABILITY_OF_BOSS_APPEARING 0.001
#define PROBABILITY_OF_ENEMY_MOVING 0.5
#define PROBABILITY_OF_ENEMY_SHOOTING 0.3
#define PROBABILITY_OF_BOSS_SHOOTING 0.5
#define PROBABILITY_OF_ENEMY_TURNING 0.5
#define PROBABILITY_OF_OBSTACLE 0.05
```

As can be seen in the code taken from `variables.hpp` the probability of the `YWSE` shooting is `+66%` than the probability of an `enemy` shooting, reduced by `-17%` compared to previous patch.

## `v1.7.0` (14 October 2022)

- Added the `smart enemy`, also known as `YWSE` {Your Worst and Smartest Enemy}

The `smart enemy` is represented by a gray `%`.

It will move faster, will search for the player (`$`) and will try to kill them.
Its AI is based on the `enemy` AI but it's set to the maximum value.
Its `HP` is randomly generated between `1` and `10`.

The `YWSE` randomly respawns after its death, with a probability of `1%` for each frame.

## `v1.6.0` (17 August 2022)

- Set the bullet speed

You can set the bullet speed choosing between `SLOW`, `MEDIUM` and `FAST` by clicking `1`/`2`/`3`.

## `v1.5.1` (17 August 2022)

- `mine`s are no more triggered by bullets

## `v1.5.0` (15 August 2022)

- Added `mine`s

The mine is triggered when the player steps on it, or when a bullet hits it, or when another mine detonates.

```batch
000000000
000XXX000
000X*X000  The "X" area is damaged by the "*" mine
000XXX000
000000000
000000000
```

In the below picture, the triggered mine is an "%".

```batch
##################################################
#                                                #
#                                             =  #
#            =                                   #
#     <    << 2                        *         #        POINTS
#                                        **      #        165
#                                       ** *     #
#                             =            *8    #        ENEMIES
#                                        *$*  8  #        6
#       ^                                * %     #
#                                            =   #        AMMUNITIONS
#                                          7     #        999524
#=                                            v  #
#  4 >                                           #        KILLS
#                                                #        0
#                                                #
#                                                #
#       5                   =                    #
#            *                                   #
##################################################
```

The player can also enter mine mode by pressing the `m`/`M` key.

## `v1.4.0` (15 August 2022)

- The player can now **build obstacles**.

To build an obstacle you have to click `b`/`B` to active the `build` mode.

When you are in the `build` mode you can build an obstacle using the arrows, like you would do to fire bullets.

Being obstacles so effective, they cost **5 ammunitions** each.

```batch
##################################################
#                              >                 #
#                                                #
#        5                                       #
#                                                #        POINTS
#                                                #        153
#                                                #
#                                           =    #        ENEMIES
#                      <                         #        7
#        v                =                      #
#                                         9      #        AMMUNITIONS
#                                                #        25
#                                        ^       #
#                                         v      #        KILLS
#        9          1>                   7v      #        0
#                                                #
#        v            <                         =#
#        v                        2   ===========#
# =   6                           v   $          #
##################################################
```

In the above example you can see that the player has built an obstacles wall to protect from enemies coming from North.

- Obstacles can now reach higher HP when more of them spawn over the same point.
- Solved bug from `v1.3.z` which allowed both player and enemies to step over obstacles thanks to the pac-man effect.

## `v1.3.1` (14 August 2022)

- Information about the game are now displayed on the right side and no more under the field. (See `v1.2.1`)

## `v1.3.0` (14 August 2022)

- Added `obstacles`.

Obstacles are represented by `=` (equal sign), and they appear to be `yellow`.

```batch
##################################################
#                                  =             #
#                     ^                          #
#                                                #
#                                                #        POINTS
#               =                                #        248
#                                          =     #
#                                 =              #        ENEMIES
#                                                #        6
#                                                #
#   4                                            #        AMMUNITIONS
#                                                #        9
#   v                                            #
#                                   =            #        KILLS
#               1                           9    #        1
#<     <   <          2                          #
#               ^                                #
#<           1  @>      7     >                  #
#       =                       =           v   =#
##################################################
```

They have HP (health points) randomly in the range 1->3.

You can hit them with bullets, the bullet will be destroyed and the obstacle will lose 1HP.

You **can't step over an obstacle**, so you must get around it if you want to move.

## `v1.2.1` (14 August 2022)

- Information about the game are now displayed on the right side and no more under the field.

```batch
##################################################
#                            7                   #
#                                                #
#                                                #
#                                                #        POINTS
#                                                #        0
#                                                #
#                                                #        ENEMIES
#                                                #        4
#                                                #
#         $                                      #        AMMUNITIONS
#                       9                        #        100
#                                                #
#                                                #        KILLS
#                                                #        0
#       2                                        #
#                    5                           #
#                                                #
#                                                #
##################################################
```

## `v1.2.0` (16 July 2022)

- Enemies are now `smart` and you can configure the AI level from settings (0->1).
- `BEL` sound in terminal when an enemy dies.

## `v1.1.0` (13 July 2022)

- Added `cross fire` mode, which allows you to fire bullets in the four directions at the same time.

In the below image you can see the `cross fire` mode used alongside with `auto fire`.

```batch
##################################################
#  ^                                             #
#  ^                                             #
#  ^                                             #
#  ^         <<             7                    #
#  ^                                             #
#  ^                                             #
#  ^                                             #
#  ^                                             #
#  ^                                             #
#<<$>>>>>>>>>>>>>>>>>>>>3 >     >                #
#  v   2                                         #
#  v                                             #
#  v                  5                          #
#  v                                             #
#  v                                             #
#  v                                             #
#  v                                             #
#  v                                             #
##################################################
```

- Instructions at the beginning of the game are now coloured.
