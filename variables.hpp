#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <conio.h>

#include "utilities.hpp"
#include "clearScreen.hpp"
#include "settings.hpp"


// Debug
#define DEBUG 0
#define OBSTACLES 0
#define BUILD 0

// Characters constants
#define PLAYER_SKIN '$'
#define ENEMY_SKIN '&'
#define BOSS_SKIN '%'
#define DIED_SKIN '@'
#define BULLET_UP '^'
#define BULLET_DOWN 'v'
#define BULLET_RIGHT '>'
#define BULLET_LEFT '<'
#define OBSTACLE_SKIN '='
#define MINE_SKIN '*'
#define TRIGGERED_MINE_SKIN '%'

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
#define CLS clearScreen(true, false) // CLear Screen (with spaces but without keeping attributes)
#define CLSa clearScreen(true, true) // CLear Screen (with spaces and keeping attributes)

// Probability constants
#define PROBABILITY_OF_ENEMY_APPEARING 0.02
#define PROBABILITY_OF_BOSS_APPEARING 0.01
#define PROBABILITY_OF_ENEMY_MOVING 0.5
#define PROBABILITY_OF_ENEMY_SHOOTING 0.3
#define PROBABILITY_OF_BOSS_SHOOTING 0.6
#define PROBABILITY_OF_ENEMY_TURNING 0.5
#define PROBABILITY_OF_OBSTACLE 0.05

// AI constants
#define AI_DIFFICULTY_IDIOT 0
#define AI_DIFFICULTY_EASY 0.25
#define AI_DIFFICULTY_MEDIUM 0.5
#define AI_DIFFICULTY_HARD 0.75
#define AI_DIFFICULTY_INSANE 1.0
const double AI_DIFFICULTIES[] = {0.0, 0.25, 0.5, 0.75, 1.0};


namespace game {
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
}


class Character {
public:

    int x; // x position
    int y; // y position
    char direction; // direction
    int bullet_speed; // bullet speed
    bool alive;

    Character(int x_origin, int y_origin, int direction) {
        this->x = x_origin;
        this->y = y_origin;
        this->direction = direction;
        this->alive = true;
    }

    void movePlayer(char choice);
};

class Bullet;
class Enemy;
class Boss;
class Player;
class Obstacle;
class Mine;

namespace game {
    unsigned int starting_enemies;
    unsigned int starting_ammunitions;
    unsigned int frame_duration; // duration of each frame in milliseconds
    float AI; // difficulty of the AI
    bool status; // game status (PLAYING or PAUSED)

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Obstacle> obstacles;
    std::vector<Boss> bosses;
    std::vector<Mine> mines;

    bool obstacles_field[20][50]; // This prevents the enemies from spawning on top of obstacles
    bool mines_field[20][50]; // This prevents the enemies from spawning on top of mines

    void configure(bool standard) {
        if (standard) {
            game::frame_duration = 100;
            game::starting_ammunitions = 100;
            game::starting_enemies = 4;
            game::AI = AI_DIFFICULTY_MEDIUM;
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

            std::cout << "\nPress any key to start the game." << std::endl;
            getch();
            CLS;
        }
    }
};


void Character::movePlayer(char choice) {
    int previous_x=this->x, previous_y=this->y;

    switch (choice) {
        case 'w': case 'W':
            if (!game::obstacles_field[this->y-1][this->x])
                this->y--;
            this->direction = NORTH;
            break;
        case 'a': case 'A':
            if (!game::obstacles_field[this->y][this->x-1])
                this->x--;
            this->direction = WEST;
            break;
        case 's': case 'S':
            if (!game::obstacles_field[this->y+1][this->x])
                this->y++;
            this->direction = SOUTH;
            break;
        case 'd': case 'D':
            if (!game::obstacles_field[this->y][this->x+1])                
                this->x++;
            this->direction = EAST;
            break;
    }

    // Check if the player is out of the screen
    if (this->x <= 0 || this->x >= 49 || this->y <= 0 || this->y >= 19) {
        if (this->x <= 0 && this->direction == WEST) {
            if (!game::obstacles_field[this->y][48])
                this->x = 48;
            else
                this->x = previous_x;
        } else if (this->x >= 49 && this->direction == EAST) {
            if (!game::obstacles_field[this->y][1])
                this->x = 1;
            else
                this->x = previous_x;
        } else if (this->y <= 0 && this->direction == NORTH) {
            if (!game::obstacles_field[18][this->x])
                this->y = 18;
            else
                this->y = previous_y;
        } else if (this->y >= 19 && this->direction == SOUTH) {
            if (!game::obstacles_field[1][this->x])
                this->y = 1;
            else
                this->y = previous_y;
        }
    }
}


class Obstacle {
public:
    int x; // x position
    int y; // y position
    char skin; // skin of the obstacle
    int hp; // health points of the obstacle
    bool active; // if the obstacle is active or not

    Obstacle(int x, int y, char skin) {
        this->x = x;
        this->y = y;
        // Check if the obstacle hits the border of the screen
        if (this->x <= 0 || this->x >= 49 || this->y <= 0 || this->y >= 19) {
            this->active = false;
            return;
        }
        // Check if the coordinates are already occupated by another obstacle
        if (game::obstacles_field[this->y][this->x]) {
            // Then sum their hp and set the obstacle as inactive
            for (auto &obstacle: game::obstacles) {
                if (obstacle.x == this->x && obstacle.y == this->y) {
                    obstacle.hp += rand()%3+1;
                    break;
                }
            }
            this->active = false;
        } else {
            this->skin = skin;
            this->hp = rand()%3+1;
            this->active = true;

            game::obstacles_field[y][x] = true; // Add the obstacle to the field
        }
    };

    void checkHit();
};


class Mine {
public:
    int x; // x position
    int y; // y position
    char skin; // skin of the mine
    bool active; // if the mine is active
    bool triggered; // if the mine has been triggered
    bool just_triggered; // if the mine has just been triggered so that it can't explode this turn

    Mine(int x, int y) {
        this->x = x;
        this->y = y;
        this->skin = MINE_SKIN;
        this->active = true;
        this->triggered = false;
        this->just_triggered = false;
        // Check if the mine hits the border of the screen
        if (this->x <= 0 || this->x >= 49 || this->y <= 0 || this->y >= 19)
            return;
        // Check if the coordinates are already occupated by another mine
        if (game::mines_field[this->y][this->x]) {
            return; // Unlike the obstacles, the mines can't be duplicated
        } else {
            game::mines_field[y][x] = true; // Add the mine to the field
        }
    };


    void trigger() {
        this->just_triggered = true;
        this->triggered = true;
        this->skin = TRIGGERED_MINE_SKIN;
    };


    void detonate(Player &player);
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

    void turn(bool smart, Player &player);

    void move();
};


class Player: public Character {
public:

    int ammunitions; // ammunitions counter
    int points; // points counter
    int kills; // kills counter
    bool auto_fire; // auto fire status
    bool cross_fire; // cross fire status
    bool build; // build status
    bool mine; // mine status
    int fire_direction; // fire direction

    Player(): Character(10, 10, NORTH) {
        this->bullet_speed = SLOW;
        this->ammunitions = 100;
        this->points = 0;
        this->kills = 0;
        this->auto_fire = false;
        this->cross_fire = false;
        this->build = false;
        this->mine = false;
        this->fire_direction = NORTH;
    }

    void changeBulletSpeed(char speed) {
        switch (speed) {
            case '1':
                this->bullet_speed = SLOW;
                break;
            case '2':
                this->bullet_speed = MEDIUM;
                break;
            case '3':
                this->bullet_speed = FAST;
                break;
        }
    }

    void fireBullet();

    void changeFireDirection(int direction);

    void buildObstacle();

    void placeMine();
};


class Boss: public Enemy {
public:

    Boss (int x_origin, int y_origin, int direction): Enemy(x_origin, y_origin, direction) {
        this->value = rand()%9+1;
        this->skin = BOSS_SKIN;
        this->alive = true;
    }

    char getSkin() {
        if (!this->alive)
            this->skin = DIED_SKIN;
        return this->skin;
    }
    
    void turn(bool smart, Player &player) {
        this->Enemy::turn(true, player);
    }
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
            for (int i = 0; i < speed; i++) {
                if (game::obstacles_field[this->y-1][this->x]) {
                    this->active = false;
                    for (auto& obstacle: game::obstacles) {
                        if (obstacle.x == this->x && obstacle.y == this->y-1) {
                            obstacle.hp--;
                            if (obstacle.hp == 0) {
                                obstacle.active = false;
                                game::obstacles_field[this->y-1][this->x] = false;
                            }
                            break;
                        }
                    }
                    return;
                }
                this->y--;
            }
        } else if (direction == EAST) {
            for (int i = 0; i < speed; i++) {
                if (game::obstacles_field[this->y][this->x+1]) {
                    this->active = false;
                    for (auto& obstacle: game::obstacles) {
                        if (obstacle.x == this->x+1 && obstacle.y == this->y) {
                            obstacle.hp--;
                            if (obstacle.hp == 0) {
                                obstacle.active = false;
                                game::obstacles_field[this->y][this->x+1] = false;
                            }
                            break;
                        }
                    }
                    return;
                }
                this->x++;
            }
        } else if (direction == WEST) {
            for (int i = 0; i < speed; i++) {
                if (game::obstacles_field[this->y][this->x-1]) {
                    this->active = false;
                    for (auto& obstacle: game::obstacles) {
                        if (obstacle.x == this->x-1 && obstacle.y == this->y) {
                            obstacle.hp--;
                            if (obstacle.hp == 0) {
                                obstacle.active = false;
                                game::obstacles_field[this->y][this->x-1] = false;
                            }
                            break;
                        }
                    }
                    return;
                }
                this->x--;
            }
        } else if (direction == SOUTH) {
            for (int i = 0; i < speed; i++) {
                if (game::obstacles_field[this->y+1][this->x]) {
                    this->active = false;
                    for (auto& obstacle: game::obstacles) {
                        if (obstacle.x == this->x && obstacle.y == this->y+1) {
                            obstacle.hp--;
                            if (obstacle.hp == 0) {
                                obstacle.active = false;
                                game::obstacles_field[this->y+1][this->x] = false;
                            }
                            break;
                        }
                    }
                    return;
                }
                this->y++;
            }
        }

        // Check if the bullet hits the border of the screen
        if (this->x <= 0 || this->x >= 49 || this->y <= 0 || this->y >= 19)
            this->active = false;
    }
};


void Obstacle::checkHit() {
    for (int i = 0; i < game::bullets.size(); i++) {
        if (!game::bullets[i].active) return;
        if (game::bullets[i].x == this->x && game::bullets[i].y == this->y) {
            game::bullets[i].active = false; // Deactivate the bullet which hit the obstacle
            this->hp--;
            if (!this->hp) {
                this->active = false;
                game::obstacles_field[this->y][this->x] = false;
                break;
            }
        }
    }
};


void Enemy::fireBullet() {
    Bullet bullet(this->x, this->y, rand()%3+1, this->direction, ENEMY);
    game::bullets.push_back(bullet);
}


void Enemy::turn(bool smart, Player &player) {
    if (smart) {
        // If the enemy can see the player, it will fire
        if (this->x == player.x && this->y == player.y) {
            this->fireBullet();
        } else if (this->x == player.x) {
            if (this->y > player.y) {
                this->direction = NORTH;
            } else {
                this->direction = SOUTH;
            }
        } else if (this->y == player.y) {
            if (this->x > player.x) {
                this->direction = WEST;
            } else {
                this->direction = EAST;
            }
        } else {
            // If the enemy can't see the player, it will move towards the player's coordinates
            if (abs(this->x - player.x) < abs(this->y - player.y)) { // You are closer to the x coordinate of the player
                if (this->x > player.x) {
                    this->direction = WEST;
                } else {
                    this->direction = EAST;
                }
            } else {
                if (this->y > player.y) {
                    this->direction = NORTH;
                } else {
                    this->direction = SOUTH;
                }
            }
        }

    } else {
        // As of now it's just a random turn, but it could be a smarter one
        this->direction = directional_constants[rand()%4];
    }
}


void Enemy::move() {
    int previous_x=this->x, previous_y=this->y;

    if (this->direction == NORTH) {
        if (!game::obstacles_field[this->y-1][this->x])
            this->y--;
    } else if (this->direction == EAST) {
        if (!game::obstacles_field[this->y][this->x+1])
            this->x++;
    } else if (this->direction == WEST) {
        if (!game::obstacles_field[this->y][this->x-1])
            this->x--;
    } else if (this->direction == SOUTH) {
        if (!game::obstacles_field[this->y+1][this->x])
            this->y++;
    }

    if (this->x <= 0 || this->x >= 49 || this->y <= 0 || this->y >= 19) {
        if (this->x <= 0 && this->direction == WEST) {
            if (!game::obstacles_field[this->y][48])
                this->x = 48;
            else
                this->x = previous_x;
        } else if (this->x >= 49 && this->direction == EAST) {
            if (!game::obstacles_field[this->y][1])
                this->x = 1;
            else
                this->x = previous_x;
        } else if (this->y <= 0 && this->direction == NORTH) {
            if (!game::obstacles_field[18][this->x])
                this->y = 18;
            else
                this->y = previous_y;
        } else if (this->y >= 19 && this->direction == SOUTH) {
            if (!game::obstacles_field[1][this->x])
                this->y = 1;
            else
                this->y = previous_y;
        }
    }
}


void Player::fireBullet() {
    if (this->cross_fire) {
        if (this->ammunitions < 4) // Can't fire 4 bullets, so we don't fire
            return;
        this->ammunitions -= 4; // Remove 4 ammunitions, because the player fires 4 bullets (one for each side)
        game::bullets.push_back(Bullet(this->x, this->y, this->bullet_speed, NORTH, PLAYER));
        game::bullets.push_back(Bullet(this->x, this->y, this->bullet_speed, EAST, PLAYER));
        game::bullets.push_back(Bullet(this->x, this->y, this->bullet_speed, WEST, PLAYER));
        game::bullets.push_back(Bullet(this->x, this->y, this->bullet_speed, SOUTH, PLAYER));
    } else if (this->ammunitions > 0) {
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


void Player::buildObstacle() {
    if (this->ammunitions < 5) return;

    this->ammunitions -= 5;
    if (this->fire_direction == NORTH)
        game::obstacles.push_back(Obstacle(this->x, this->y-1, OBSTACLE_SKIN));
    else if (this->fire_direction == EAST)
        game::obstacles.push_back(Obstacle(this->x+1, this->y, OBSTACLE_SKIN));
     else if (this->fire_direction == WEST)
        game::obstacles.push_back(Obstacle(this->x-1, this->y, OBSTACLE_SKIN));
     else if (this->fire_direction == SOUTH)
        game::obstacles.push_back(Obstacle(this->x, this->y+1, OBSTACLE_SKIN));
}


void Mine::detonate(Player &player) {
    if (!this->triggered)
        return;

    for (auto &bullet: game::bullets) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (bullet.x == this->x+i && bullet.y == this->y+j)
                    bullet.active = false;
            }
        }
    }
    for (auto &enemy: game::enemies) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (enemy.x == this->x+i && enemy.y == this->y+j) {
                    player.ammunitions += enemy.value;
                    enemy.value--;
                    if (enemy.value <= 0)
                        enemy.alive = false;
                }
            }
        }
    }
    for (auto &obstacle: game::obstacles) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (obstacle.x == this->x+i && obstacle.y == this->y+j) {
                    obstacle.hp -= 1;
                    if (obstacle.hp <= 0) {
                        obstacle.active = false;
                        game::obstacles_field[obstacle.y][obstacle.x] = false;
                    }
                }
            }
        }
    }
    for (auto &mine: game::mines) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (mine.x == this->x+i && mine.y == this->y+j)
                    mine.trigger();
            }
        }
    }

    this->active = false; // The mine doesn't exist anymore
    game::mines_field[this->y][this->x] = false; // Remove the mine from the field
};


void Player::placeMine() {
    if (this->ammunitions < 3)
        return;

    this->ammunitions -= 3;
    if (this->fire_direction == NORTH)
        game::mines.push_back(Mine(this->x, this->y-1));
    else if (this->fire_direction == EAST)
        game::mines.push_back(Mine(this->x+1, this->y));
    else if (this->fire_direction == WEST)
        game::mines.push_back(Mine(this->x-1, this->y));
    else if (this->fire_direction == SOUTH)
        game::mines.push_back(Mine(this->x, this->y+1));
}


namespace game {
    namespace random {
        char direction() {
            return directional_constants[rand() % 4];
        }

        void addEnemy(int x, int y) {
            game::enemies.push_back(Enemy(x, y, direction()));
        }

        void addBoss(int x, int y) {
            game::bosses.push_back(Boss(x, y, direction()));
        }

        void addObstacle(int x, int y) {
            if (field[y][x] == ' ') {
                field[y][x] = OBSTACLE_SKIN;
                obstacles.push_back(Obstacle(x, y, OBSTACLE_SKIN));
            }
        }

        char directionalChar() {
            return directional_chars[rand() % 4];
        }
    }
}