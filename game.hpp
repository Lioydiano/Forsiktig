#include <chrono>
#include <thread>
#include <future>
#include <time.h>

#include "variables.hpp"


namespace game {
    Player player;
};

void printField() {
    std::cout << CLS;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 50; j++) {
            if (i == game::player.y && j == game::player.x) {
                if (game::player.alive)
                    std::cout << "\x1B[31m$\033[0m";
                else
                    std::cout << "\x1B[31m@\033[0m";
                continue;
            }
            for (int k = 0; k < game::enemies.size(); k++) {
                if (!game::enemies[k].alive)
                    continue;
                if (i == game::enemies[k].y && j == game::enemies[k].x) {
                    std::cout << std::string("\x1B[34m") + game::enemies[k].skin + std::string("\033[0m");
                    goto escape;
                }
            }
            for (int k = 0; k < game::bullets.size(); k++) {
                if (!game::bullets[k].active)
                    continue;
                if (i == game::bullets[k].y && j == game::bullets[k].x) {
                    if (game::bullets[k].fired == PLAYER)
                        std::cout << std::string("\x1B[35m") + game::bullets[k].skin + std::string("\033[0m");
                    else
                        std::cout << std::string("\x1B[32m") + game::bullets[k].skin + std::string("\033[0m");
                    goto escape;
                }
            }
            std::cout << game::field[i][j];
            escape:
                continue;
        }
        std::cout << '\n';
    }

    int sum = 0;
    for (auto& enemy : game::enemies)
        sum += enemy.alive;

    std::cout << "POINTS" << TEN_SPACES << "ENEMIES" << TEN_SPACES << "AMMUNITIONS" << std::endl;
    std::cout << "  " << game::player.points << SEVENTEEN_SPACES << sum << SEVENTEEN_SPACES << game::player.ammunitions << std::endl;
}

void moveAllBullets() {
    for (int i=0; i<game::bullets.size(); i++) {
        for (int j=0; j<game::bullets.size(); j++) {
            if (i == j) continue;

            if (game::bullets[i].x == game::bullets[j].x && game::bullets[i].y == game::bullets[j].y) {
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

    for (int i=0; i<bullets.size(); i++) {
        for (int j=0; j<enemies.size(); j++) {
            if (bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y && bullets[i].fired == PLAYER) {
                player.ammunitions += enemies[j].value;
                enemies[j].value--;

                if (enemies[j].value == 0)
                    enemies[j].alive = false;

                bullets[i].active = false;
                bullets.erase(bullets.begin() + i); // remove bullet from the vector
                break;
            }
        }
        if (bullets[i].x == player.x && bullets[i].y == player.y && bullets[i].fired == ENEMY)
            died = true;
    }

    // Insert bullets into the field
    for (auto& bullet: bullets) {
        if (bullet.active)
            field[bullet.y][bullet.x] = direction_to_skin[bullet.direction];
    }

    // Insert enemies into the field
    for (auto& enemy: enemies) {
        field[enemy.y][enemy.x] = enemy.getSkin();
    }

    if (died) {
        player.alive = false;
        // Insert dead player into the field
        field[player.y][player.x] = DIED_SKIN;
        printField();
        std::cout << "You died!\n";
        getch(); // Prevent Window from closing
        exit(0); // exit the game
    }

    // Insert player into the field
    field[player.y][player.x] = PLAYER_SKIN;
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
    std::cout << CLS;
    std::cout << "Do you want to configure the game? (y/n)\n> ";
    char c = getch();
    std::cout << CLS;
    switch (c) {
        case 'y': case 'Y':
            std::cout << "Do you want to load the settings? (y/n)\n> ";
            c = getch();
            std::cout << CLS;
            switch (c) {
                case 'y': case 'Y': {
                    auto settings = loadSettings();
                    game::frame_duration = std::get<0>(settings);
                    game::starting_enemies = std::get<1>(settings);
                    game::starting_ammunitions = std::get<2>(settings);
                    std::cout << "Settings loaded!\n";
                    getch();
                    std::cout << CLS;
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
    std::ios_base::sync_with_stdio(false);
    game::emptyField();
    printField();

    for (int i=0; i<game::starting_enemies; i++)
        game::enemies.push_back(Enemy(rand()%48+1, rand()%18+1, SOUTH));
    game::player.ammunitions = game::starting_ammunitions;

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
                if (rand()%5 == 0) {
                    enemy.turn();
                    enemy.fireBullet();
                } else if (rand()%2 == 0) {
                    enemy.movePlayer(game::random::directionalChar());
                }
            }
            if (rand()%50 == 0)
                game::random::addEnemy(rand()%48+1, rand()%18+1);

            if (game::player.auto_fire)
                game::player.fireBullet();
            game::player.points++;
        }

        choice = input.get();
        switch (choice) {
            case 'q': case 'Q':
                return;
            case 'p': case 'P':
                game::status = PAUSED;
                break;
            case 'r': case 'R':
                game::status = PLAYING;
                break;
            case 'w': case 'W': case 'a': case 'A': case 's': case 'S': case 'd': case 'D':
                if (game::status == PLAYING)
                    game::player.movePlayer(choice);
                break;
            case BEFORE_KEY_UP: case BEFORE_KEY_DOWN: case BEFORE_KEY_LEFT: case BEFORE_KEY_RIGHT:
                game::player.changeFireDirection(choice);
                if (game::status == PLAYING && game::player.ammunitions > 0 && !game::player.auto_fire)
                    game::player.fireBullet();
                break;
            case 'x': case 'X':
                game::player.auto_fire = !game::player.auto_fire;
                break;
        }
    }
}