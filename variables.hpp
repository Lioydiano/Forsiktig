#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <conio.h>

#include "clearScreen.hpp"
#include "settings.hpp"


// Characters constants
#define PLAYER_SKIN '$'
#define ENEMY_SKIN '&'
#define DIED_SKIN '@'
#define BULLET_UP '^'
#define BULLET_DOWN 'v'
#define BULLET_RIGHT '>'
#define BULLET_LEFT '<'

// Directional constants
#define NORTH 'N'
#define EAST 'E'
#define WEST 'O'
#define SOUTH 'S'

std::map<char, char> direction_to_skin = {
    {NORTH, BULLET_UP},
    {EAST, BULLET_RIGHT},
    {WEST, BULLET_LEFT},
    {SOUTH, BULLET_DOWN}
};
char directional_constants[] = {NORTH, EAST, WEST, SOUTH};
char directional_chars[] = {'w', 'd', 'a', 's'};

// Speed constants
#define SLOW 1
#define MEDIUM 2
#define FAST 3

// Game status constants
#define PLAYING true
#define PAUSED false

// Fired bullet constants
#define PLAYER true
#define ENEMY false

// getch() constants
#define BEFORE_KEY_UP '{'
#define BEFORE_KEY_DOWN '|'
#define BEFORE_KEY_RIGHT '}'
#define BEFORE_KEY_LEFT '~'

#define AFTER_KEY_UP 72
#define AFTER_KEY_DOWN 80
#define AFTER_KEY_RIGHT 77
#define AFTER_KEY_LEFT 75

#define KEY_ESC -32 // This isn't ESC, but the prefix for the arrow keys

// Others
#define SPACE_CHAR ' '
#define TEN_SPACES "          "
#define SEVENTEEN_SPACES "               "
#define SS "\33[2J\33[H" // Scroll Screen
#define CLS clearScreen() // CLear Screen (with spaces but without keeping attributes)
#define CLSa clearScreen(true, true) // CLear Screen (with spaces and keeping attributes)

// Probability constants
#define PROBABILITY_OF_ENEMY_APPEARING 0.02
#define PROBABILITY_OF_ENEMY_MOVING 0.5
#define PROBABILITY_OF_ENEMY_SHOOTING 0.3
#define PROBABILITY_OF_ENEMY_TURNING 0.5

class Character {
public:

    int x; // x position
    int y; // y position
    char direction; // direction
    int bullet_speed; // bullet speed
    bool alive;

    Character (int x_origin, int y_origin, int direction) {
        this->x = x_origin;
        this->y = y_origin;
        this->direction = direction;
        this->alive = true;
    }

    void movePlayer(char choice) {
        switch (choice) {
            case 'w': case 'W':
                this->y--;
                this->direction = NORTH;
                break;
            case 'a': case 'A':
                this->x--;
                this->direction = WEST;
                break;
            case 's': case 'S':
                this->y++;
                this->direction = SOUTH;
                break;
            case 'd': case 'D':
                this->x++;
                this->direction = EAST;
                break;
        }

        // Check if the player is out of the screen
        if (this->x <= 0 || this->x >= 49 || this->y <= 0 || this->y >= 19) {
            if (this->x <= 0 && this->direction == WEST) {
                this->x = 48;
            } else if (this->x >= 49 && this->direction == EAST) {
                this->x = 1;
            } else if (this->y <= 0 && this->direction == NORTH) {
                this->y = 18;
            } else if (this->y >= 19 && this->direction == SOUTH) {
                this->y = 1;
            }
        }
    }
};

class Bullet;
class Enemy;
class Player;

namespace game {
    unsigned int starting_enemies;
    unsigned int starting_ammunitions;
    unsigned int frame_duration; // duration of each frame in milliseconds
    bool status; // game status (PLAYING or PAUSED)

    char field[20][50] = {' '}; // game field
    const char void_field[20][50] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
    };
    void emptyField() {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 50; j++) {
                field[i][j] = void_field[i][j];
            }
        }
    }

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;


    void configure(bool standard) {
        if (standard) {
            game::frame_duration = 100;
            game::starting_ammunitions = 100;
            game::starting_enemies = 4;
            std::cout << "Standard configuration loaded" << std::endl;
            getch();
            CLS;
        } else {
            std::cout << "Frame duration [unsigned int] (ms): ";
            while (!(std::cin >> game::frame_duration)) {
                std::cout << "Invalid input. Try again.\n";
                std::cout << "Frame duration [unsigned int] (ms): ";
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            }
            if (game::frame_duration < 50) {
                std::cout << "Frame duration too low. Setting to 50.\n";
                game::frame_duration = 50;
            } else if (game::frame_duration > 500) {
                std::cout << "Frame duration too high. Setting to 500.\n";
                game::frame_duration = 500;
            }

            std::cout << "Starting ammunitions [unsigned int]: ";
            while (!(std::cin >> game::starting_ammunitions)) {
                std::cout << "Invalid input. Try again.\n";
                std::cout << "Starting ammunitions [unsigned int]: ";
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            }
            if (game::starting_ammunitions < 1) {
                std::cout << "Did you want to start without ammunitions? Setting to 1.\n";
                game::starting_ammunitions = 1;
            }

            std::cout << "Starting enemies [unsigned int]: ";
            while (!(std::cin >> game::starting_enemies)) {
                std::cout << "Invalid input. Try again.\n";
                std::cout << "Starting enemies [unsigned int]: ";
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            }
            if (game::starting_enemies > 20) {
                std::cout << "Not enough space for everyone in the space! Setting to 50.\n";
                game::starting_enemies = 50;
            }

            std::cout << "Configuration loaded" << std::endl;
            getch();
            CLS;
            std::cout << "Frame duration: " << game::frame_duration << std::endl;
            std::cout << "Starting ammunitions: " << game::starting_ammunitions << std::endl;
            std::cout << "Starting enemies: " << game::starting_enemies << std::endl << std::endl;
            std::cout << "Do you want to save the configuration? (y/n)\n> ";
            char answer = getch();
            switch (answer) {
                case 'y': case 'Y':
                    saveSettings(std::make_tuple(game::frame_duration, game::starting_enemies, game::starting_ammunitions));
            }

            getch();
            CLS;
            std::cout << "\nPress any key to start the game." << std::endl;
            getch();
            CLS;
        }
    }
};


class Enemy: public Character {
public:

    int value; // character value (an Enemy's skin will be its value)
    char skin;

    Enemy (int x_origin, int y_origin, int direction): Character(x_origin, y_origin, direction) {
        this->value = rand()%9+1;
        this->skin = std::to_string(this->value).c_str()[0];
        this->alive = true;
    }

    char getSkin() {
        if (!this->alive)
            this->skin = DIED_SKIN;
        else
            this->skin = std::to_string(this->value).c_str()[0];
        return this->skin;
    }

    void fireBullet();

    void turn();
};


class Player: public Character {
public:

    int ammunitions; // ammunitions counter
    int points; // points counter
    bool auto_fire; // auto fire status
    int fire_direction; // fire direction

    Player(): Character(10, 10, NORTH) {
        this->bullet_speed = SLOW;
        this->ammunitions = 100;
        this->points = 0;
        this->auto_fire = false;
        this->fire_direction = NORTH;
    }

    void fireBullet();

    void changeFireDirection(int direction);
};


class Bullet {
public:

    int x; // x position
    int y; // y position
    int speed; // speed of the bullet
    char direction; // direction of the bullet
    bool fired; // if the bullet was fired by the player or by the enemy
    bool active; // if the bullet is active or not
    char skin; // skin of the bullet

    Bullet(int x_origin, int y_origin, int speed, int direction, bool fired) {
        this->x = x_origin;
        this->y = y_origin;
        this->speed = speed;
        this->direction = direction;
        this->fired = fired;
        this->active = true;
        this->skin = direction_to_skin[direction];
    }

    void move() {
        if (direction == NORTH) {
            this->y -= speed;
        } else if (direction == EAST) {
            this->x += speed;
        } else if (direction == WEST) {
            this->x -= speed;
        } else if (direction == SOUTH) {
            this->y += speed;
        }

        using namespace game;
        // Check if the bullet hits the border of the screen
        if (this->x <= 0 || this->x >= 49 || this->y <= 0 || this->y >= 19)
            this->active = false;
    }
};


void Enemy::fireBullet() {
    Bullet bullet(this->x, this->y, rand()%3+1, this->direction, ENEMY);
    game::bullets.push_back(bullet);
}


void Enemy::turn() {
    // As of now it's just a random turn, but it could be a smarter one
    this->direction = directional_constants[rand()%4];
}


void Player::fireBullet() {
    if (this->ammunitions > 0) {
        this->ammunitions--;
        Bullet bullet(this->x, this->y, this->bullet_speed, this->fire_direction, PLAYER);
        game::bullets.push_back(bullet);
    }
}


void Player::changeFireDirection(int direction) {
    switch (direction) {
        case BEFORE_KEY_UP:
            this->fire_direction = NORTH;
            break;
        case BEFORE_KEY_RIGHT:
            this->fire_direction = EAST;
            break;
        case BEFORE_KEY_LEFT:
            this->fire_direction = WEST;
            break;
        case BEFORE_KEY_DOWN:
            this->fire_direction = SOUTH;
            break;
    }
}


namespace game {
    namespace random {
        char direction() {
            return directional_constants[rand() % 4];
        }

        void addEnemy(int x, int y) {
            game::enemies.push_back(Enemy(x, y, direction()));
        }

        char directionalChar() {
            return directional_chars[rand() % 4];
        }
    }
}