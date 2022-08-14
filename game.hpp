#include <chrono>
#include <thread>
#include <future>
#include <random>
#include <time.h>

#include "variables.hpp"

Player player;

void printField() {
    int sum = 0;
    for (auto& enemy : game::enemies)
        sum += enemy.alive;

    std::cout << SS;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 50; j++) {
            if (i == player.y && j == player.x) {
                if (player.alive)
                    std::cout << "\x1b[91m$\033[0m";
                else
                    std::cout << "\x1b[91m@\033[0m";
                continue;
            }
            for (int k = 0; k < game::enemies.size(); k++) {
                if (i == game::enemies[k].y && j == game::enemies[k].x) {
                    if (game::enemies[k].alive)
                        std::cout << std::string("\x1b[1m\x1B[94m") + game::enemies[k].skin + std::string("\033[0m");
                    else
                        std::cout << "\x1B[94m@\033[0m";
                    goto escape;
                }
            }
            for (int k = 0; k < game::bullets.size(); k++) {
                if (!game::bullets[k].active)
                    continue;
                if (i == game::bullets[k].y && j == game::bullets[k].x) {
                    if (game::bullets[k].fired == PLAYER)
                        std::cout << std::string("\x1B[95m\x1b[1m") + game::bullets[k].skin + std::string("\033[0m");
                    else
                        std::cout << std::string("\x1B[92m") + game::bullets[k].skin + std::string("\033[0m");
                    goto escape;
                }
            }
            if (game::obstacles_field[i][j]) {
                std::cout << std::string("\x1B[33m") + OBSTACLE_SKIN + std::string("\033[0m");
                goto escape;
            }
            std::cout << game::field[i][j];
            escape:
                continue;
        }

        if (i == 4)
            std::cout << "        \x1b[1mPOINTS\x1b[0m";
        else if (i == 5)
            std::cout << "        \x1b[1m" << player.points << "\x1b[0m";
        else if (i == 7)
            std::cout << "        \x1b[1mENEMIES\x1b[0m";
        else if (i == 8)
            std::cout << "        \x1b[1m" << sum << "\x1b[0m";
        else if (i == 10)
            std::cout << "        \x1b[1mAMMUNITIONS\x1b[0m";
        else if (i == 11)
            std::cout << "        \x1b[1m" << player.ammunitions << "\x1b[0m";
        else if (i == 13)
            std::cout << "        \x1b[1mKILLS\x1b[0m";
        else if (i == 14)
            std::cout << "        \x1b[1m" << player.kills << "\x1b[0m";

        std::cout << '\n';
    }
    std::cout << std::flush;
}

void moveAllBullets() {
    for (int i=0; i<game::bullets.size(); i++) {
        for (int j=0; j<game::bullets.size(); j++) {
            if (i == j) continue;

            if (game::bullets[i].x == game::bullets[j].x && game::bullets[i].y == game::bullets[j].y) {
                if (game::bullets[i].fired ==  game::bullets[j].fired)
                    continue; // Ignore when both are fired by same "team"
                game::bullets[i].active = false;
                game::bullets[j].active = false;
            }
        }
    }

    for (int i=0; i<game::bullets.size(); i++) {
        if (!game::bullets[i].active)
            game::bullets.erase(game::bullets.begin() + i);
    }

    for (auto& bullet: game::bullets) {
        if (bullet.active) // This control shouldn't be necessary but it's better to be safe than sorry
            bullet.move();
    }
}

void updateField() {
    using namespace game; // This should be automatically unused after the scope dies
    bool died = false;
    emptyField();

    for (int i=0; i<enemies.size(); i++) {
        if (!enemies[i].alive)
            enemies.erase(enemies.begin() + i); // remove enemy
    }

    for (int i=0; i<obstacles.size(); i++) {
        if (!obstacles[i].active)
            obstacles.erase(obstacles.begin() + i); // remove obstacle
    }

    for (auto& obstacle: game::obstacles) {
        if (obstacle.active) // This control shouldn't be necessary but it's better to be safe than sorry
            game::field[obstacle.y][obstacle.x] = obstacle.skin;
    }

    for (int i=0; i<bullets.size(); i++) {
        for (int j=0; j<enemies.size(); j++) {
            if (bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y && bullets[i].fired == PLAYER) {
                player.ammunitions += enemies[j].value;
                enemies[j].value--;

                if (enemies[j].value == 0) {
                    std::cout << '\x07'; // Beep
                    enemies[j].alive = false;
                    player.kills++;
                }

                bullets[i].active = false;
                bullets.erase(bullets.begin() + i); // remove bullet from the vector
                break;
            }
        }
        if (bullets[i].x == player.x && bullets[i].y == player.y && bullets[i].fired == ENEMY)
            died = true;
    }

    // Get skins for enemies
    for (auto& enemy: enemies) {
        enemy.getSkin();
    }

    if (died) {
        player.alive = false;
        printField();
        std::cout << "\x1b[31m\x1b[1mYou died!\033[0m" << std::endl;
        std::cout << "\x1b[?25h"; // Show cursor
        exit(0); // exit the game
    }
}


char getCharOrArrow() {
    char c = getch();
    if (c == KEY_ESC) { // Skip the 224 prefix
        switch (getch()) {
            case AFTER_KEY_UP:
                return BEFORE_KEY_UP;
            case AFTER_KEY_DOWN:
                return BEFORE_KEY_DOWN;
            case AFTER_KEY_LEFT:
                return BEFORE_KEY_LEFT;
            case AFTER_KEY_RIGHT:
                return BEFORE_KEY_RIGHT;
        }
    }
    return c; // This one could be in an else branch but it triggers g++ warnings
}


void mainloop() {
    // Configure the game
    CLS;
    std::cout << "Do you want to configure the game? (y/n)\n> ";
    char c = getch();
    CLS;
    switch (c) {
        case 'y': case 'Y':
            std::cout << "Do you want to load the settings? (y/n)\n> ";
            c = getch();
            CLS;
            switch (c) {
                case 'y': case 'Y': {
                    auto settings = loadSettings();
                    game::frame_duration = std::get<0>(settings);
                    game::starting_enemies = std::get<1>(settings);
                    game::starting_ammunitions = std::get<2>(settings);
                    std::cout << "Settings loaded!\n";
                    getch();
                    CLS;

                    std::cout << "AI difficulty [unsigned float] (0->1): ";
                    while (!(std::cin >> game::AI)) {
                        std::cout << "Invalid input. Try again.\n";
                        std::cout << "AI difficulty [unsigned float]: ";
                        std::cin.clear();
                        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    }
                    if (game::AI > 1) {
                        std::cout << "AI difficulty too high. Setting to 1.\n";
                        game::AI = 1;
                    } else if (game::AI < 0) {
                        std::cout << "AI difficulty too low. Setting to 0.\n";
                        game::AI = 0;
                    }
                    game::AI = closest(AI_DIFFICULTIES, 5, game::AI);
                    getch();
                    CLS;
                }
                    break;
                default:
                    game::configure(false);
                    break;
            }
            break;
        default:
            game::configure(true);
            break;
    }
    srand(time(NULL));

    for (int i=0; i<game::starting_enemies; i++)
        game::enemies.push_back(Enemy(rand()%48+1, rand()%18+1, SOUTH));
    player.ammunitions = game::starting_ammunitions;

    memset(game::obstacles_field, 0, sizeof(game::obstacles_field)); // Clear the obstacle's field

    srand(time(NULL));
    std::ios_base::sync_with_stdio(false);
    game::emptyField();
    printField();
    std::cout << "\x1b[?25l"; // Hide cursor


    // Random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution turning_distribution(PROBABILITY_OF_ENEMY_TURNING);
    std::bernoulli_distribution shooting_distribution(PROBABILITY_OF_ENEMY_SHOOTING);
    std::bernoulli_distribution moving_distribution(PROBABILITY_OF_ENEMY_MOVING);
    std::bernoulli_distribution appearing_distribution(PROBABILITY_OF_ENEMY_APPEARING);
    std::bernoulli_distribution intelligence_distribution(game::AI);
    std::bernoulli_distribution obstacle_distribution(PROBABILITY_OF_OBSTACLE);

    char choice;
    while (choice != 'q') {
        auto input = std::async(std::launch::async, getCharOrArrow);
        while (input.wait_for(std::chrono::milliseconds(game::frame_duration)) != std::future_status::ready) {
            if (game::status == PAUSED)
                continue;
            
            moveAllBullets();
            updateField();
            printField();

            for (auto& enemy: game::enemies) {
                if (!enemy.alive)
                    continue;
                if (turning_distribution(gen)) {
                    enemy.turn(intelligence_distribution(gen), player);
                } else if (shooting_distribution(gen)) {
                    enemy.fireBullet();
                } else if (moving_distribution(gen)) {
                    enemy.move();
                }
            }
            if (appearing_distribution(gen))
                game::random::addEnemy(rand()%48+1, rand()%18+1);
            if (obstacle_distribution(gen))
                game::random::addObstacle(rand()%48+1, rand()%18+1);

            if (player.auto_fire)
                player.fireBullet();
            player.points++;
        }

        choice = input.get();
        switch (choice) {
            case 'q': case 'Q':
                std::cout << "\x1b[?25h"; // Show cursor
                return;
            case 'p': case 'P':
                game::status = PAUSED;
                break;
            case 'r': case 'R':
                game::status = PLAYING;
                break;
            case 'w': case 'W': case 'a': case 'A': case 's': case 'S': case 'd': case 'D':
                if (game::status == PLAYING)
                    player.movePlayer(choice);
                break;
            case BEFORE_KEY_UP: case BEFORE_KEY_DOWN: case BEFORE_KEY_LEFT: case BEFORE_KEY_RIGHT:
                player.changeFireDirection(choice);
                if (game::status == PLAYING && player.ammunitions > 0 && !player.auto_fire)
                    player.fireBullet();
                break;
            case 'x': case 'X':
                player.auto_fire = !player.auto_fire;
                break;
            case '+':
                player.cross_fire = !player.cross_fire;
                break;
        }
    }
}