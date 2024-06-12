# `Forsiktig`
![image](https://github.com/Lioydiano/Forsiktig/assets/83119697/648532b3-156a-4c64-bc14-825c4ea69710)


Forsiktig is a terminal 2D shooting game.

## OS Compatibility

⚠️ - Until `v1.9.1` Forsiktig was compatible with only Windows due to `conio.h`.

Since `v1.9.1` Forsiktig is compatible with Windows and Linux.

## Download

Executable files are available for both OSs in the [`download page`](https://flak-zoso.github.io/src/repo/Forsiktig/about.html).

Source code is available in the [`releases`](https://github.com/Lioydiano/Forsiktig/releases) in `.zip` format.

## Run

Once you have your `forsiktig.exe`/`forsiktig` file, you can play Forsiktig by running it.

```bash

user@user:~/Downloads$ ./forsiktig

```

On Windows you should run it with `Windows Terminal` (WT), which supports colors.

```batch

C:\Users\YOUR_USERNAME\Downloads> Forsiktig

```

You will see the instructions on how to play the game.

```txt

Instructions:
- 'q' to quit
- 'p' to pause
- 'r' to resume <-- you also start the game resuming it
- 'w | a | s | d' to step
- Use arrows (or 'i | j | k | l' on Linux) to fire a bullet
- 'x' to enable/disable auto-strike

Digits from 1 to 9 are your enemies, while you're a $
Avoid bullets fired from enemies, and shoot them to get ammunitions

```

After pressing any key, you will be asked if you want to configure the settings.

```batch

Do you want to configure the game? (y/n)
>

```

If you want to go with the default settings (10fps, 4 starting enemies, 100 ammunition), press anything else than `y`/`Y`.

```batch

Standard configuration loaded

```

## Play

Now you're ready to play. You just have to send a key, and the game will start.
By default it's paused, you can resume it by pressing `r`/`R`.

```batch

##################################################
#      ^              ^                          #
#                                    <   9       #
#                     8                          #
#                                                #
#                                       >        #
#                                                #
#                                                #
#                                                #
#                                                #
#                                1     >         #
#        1                                       #
#                                                #
#        v                     >                 #
#                                                #
#           $                           6        #
#                                                #
#                                                #
#                                                #
##################################################
POINTS          ENEMIES          AMMUNITIONS
  47               5               100

```

Avoid green bullets, and shoot blue enemies to get ammunitions!

## Rules

Figure the rules out by yourself, it's part of the game!

## Release notes

You can find the release notes of the updates [`here`](ReleaseNotes.md).
