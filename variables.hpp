#include <vector>
#include <iostream>
#include <map>
#include <string>


// Characters constants
#define PLAYER_SKIN '$'
#define ENEMY_SKIN '&'
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

// Speed constants
#define SLOW 1
#define MEDIUM 2
#define FAST 3

// Game status constants
#define PLAYING true
#define PAUSED false

// Duration constants
#define FRAME_DURATION 100

// Fired bullet constants
#define PLAYER true
#define ENEMY false


class Character {
public:

    int x; // x position
    int y; // y position
    char direction; // direction
    int bullet_speed; // bullet speed

    Character (int x_origin, int y_origin, int direction) {
        this->x = x_origin;
        this->y = y_origin;
        this->direction = direction;
    }
};

class Bullet;
class Enemy;
class Player;

namespace game {
    bool status; // game status (PLAYING or PAUSED)

    char field[20][20] = {' '}; // game field
    const char void_field[20][20] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    };
    void emptyField() {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                field[i][j] = void_field[i][j];
            }
        }
    }

    std::vector<Bullet> bullets; // bullets vector
    std::vector<Enemy> enemies; // enemies vector
};


class Enemy: public Character {
public:

    int value; // character value (an Enemy's skin will be its value)
    char skin;

    Enemy (int x_origin, int y_origin, int direction): Character(x_origin, y_origin, direction) {
        this->value = rand()%9+1;
        this->skin = std::to_string(this->value).c_str()[0];
        this->bullet_speed = rand()%3+1;
    }

    void movePlayer(char choice) {
        switch (choice) {
            case 'w': case 'W':
                this->y--;
                this->direction = NORTH;
            case 'a': case 'A':
                this->x--;
                this->direction = WEST;
            case 's': case 'S':
                this->y++;
                this->direction = SOUTH;
            case 'd': case 'D':
                this->x++;
                this->direction = EAST;
        }

        // TODO: check if the player is out of the screen
    }

    char getSkin() {
        this->skin = std::to_string(this->value).c_str()[0];
        return this->skin;
    }

    void fireBullet();
};


class Player: public Character {
public:

    int ammunitions; // ammunitions counter
    int points; // points counter

    Player(): Character(10, 10, NORTH) {
        this->bullet_speed = SLOW;
        this->ammunitions = 100;
        this->points = 0;
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

        // TODO: check if the player is out of the screen
    }

    void fireBullet();
};


class Bullet {
public:

    int x; // x position
    int y; // y position
    int speed; // speed of the bullet
    char direction; // direction of the bullet
    bool fired; // if the bullet was fired by the player or by the enemy
    // std::pair<int, int> last_position; // last position of the bullet
    bool active; // if the bullet is active or not

    Bullet(int x_origin, int y_origin, int speed, int direction, bool fired) {
        this->x = x_origin;
        this->y = y_origin;
        this->speed = speed;
        this->direction = direction;
        this->fired = fired;
        // this->last_position = std::make_pair(this->x, this->y);
        this->active = true;
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
        if (this->x < 0 || this->x > 19 || this->y < 0 || this->y > 19)
            this->active = false;
    }
};


void Enemy::fireBullet() {
    Bullet bullet(this->x, this->y, bullet_speed, this->direction, ENEMY);
    game::bullets.push_back(bullet);
}


void Player::fireBullet() {
    if (this->ammunitions > 0) {
        this->ammunitions--;
        Bullet bullet(this->x, this->y, this->bullet_speed, this->direction, PLAYER);
        game::bullets.push_back(bullet);
    }
}