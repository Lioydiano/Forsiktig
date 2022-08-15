# Release Notes

Here you can find the release notes for each version of `Forsiktig` since `v1`.

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