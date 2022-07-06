#include <conio.h>
#include <chrono>
#include <thread>
#include <future>
#include <time.h>

#include "variables.hpp"


namespace game {
    Player player; // player
};

void printField() {
    system("cls");
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 50; j++)
            std::cout << game::field[i][j];
        std::cout << std::endl;
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
                // i--;    <-- THE WHOLE ERROR WAS HERE!
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
        // Insert dead player into the field
        field[player.y][player.x] = DIED_SKIN;
        printField();
        std::cout << "You died!\n";
        std::cin.get(); // Prevent Window from closing
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
    } else {
        return c;
    }
}


void mainloop() {
    // Configure the game
    system("cls");
    std::cout << "Do you want to configure the game? (y/n)\n> ";
    char c = getch();
    system("cls");
    switch (c) {
        case 'y': case 'Y':
            game::configure(false);
            break;
        default:
            game::configure(true);
            break;
    }

    srand(time(NULL));
    game::emptyField();
    printField();

    for (int i=0; i<game::starting_enemies; i++)
        game::enemies.push_back(Enemy(rand()%48+1, rand()%18+1, SOUTH));
    game::player.ammunitions = game::starting_ammunitions;

    char choice;
    while (choice != 'q') {
        auto input = std::async(std::launch::async, getCharOrArrow);
        while (input.wait_for(std::chrono::milliseconds(game::frame_duration)) != std::future_status::ready) {
            if (game::status == PAUSED) // if the game is paused, do nothing
                continue; // just wait for the next char
            
            moveAllBullets(); // move all bullets
            updateField(); // update the field
            printField(); // print the field

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
        printField(); // print the field

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