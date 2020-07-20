#include <iostream>
#include <stdlib.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;


class Enemy: public sf::Sprite {
    float speed = 4.f;
    int damage = 1;
    int health = 2;

    sf::Vector2f position;

    public:
        static sf::Texture enemy_texture;
        Enemy (int x, int y) {
            if(!enemy_texture.loadFromFile("assets/Enemies/enemyRed4.png")) {
                cout << "Error loading texture";
            }
            enemy_texture.setSmooth(true);

            setTexture(enemy_texture);

            setScale(sf::Vector2f(0.75f, 0.75f));

            setOrigin(sf::Vector2f(49.5f, 37.5f));
            setRotation(90.f);
            setPosition(x, y);
        }
};


class Bullet: public sf::Sprite {
    float speed = 7.f;

    int damage;
    sf::Vector2f position;

    public:
        static sf::Texture bullet_texture;
        Bullet (int x, int y, int given_damage) {
            if(!bullet_texture.loadFromFile("assets/Lasers/laserBlue02.png")) {
                cout << "Error loading texture";
            }
            bullet_texture.setSmooth(true);

            damage = given_damage;

            setTexture(bullet_texture);

            setOrigin(sf::Vector2f(49.5f, 37.5f));
            setRotation(90.f);
            setPosition(sf::Vector2f(x, y));
        }

        bool update_position () {

            move(7.f, 0.f);

            return true;
        }
};


class Player: public sf::Sprite {
    float speed = 5.f;
    int health = 10;
    int damage = 1;

    sf::Vector2f position;
    int move_x;
    int move_y;

    sf::Time shoot_timer = sf::seconds(0);
    sf::Time shoot_cooldown = sf::milliseconds(400);
    
    public:

        static sf::Texture player_texture;

        vector<Bullet> bullet_array;

        Player () {
            if(!player_texture.loadFromFile("assets/playerShip1_blue.png")) {
                cout << "Error loading texture";
            }
            player_texture.setSmooth(true);

            setTexture(player_texture);

            setScale(sf::Vector2f(0.75f, 0.75f));

            setOrigin(sf::Vector2f(49.5f, 37.5f));
            setRotation(90.f);
            setPosition(sf::Vector2f(37.5f + 5, 49.5f + 5));
        }

        bool movePlayer (float x_vector, float y_vector) {

            position = getPosition();
            move_x = 1;
            move_y = 1;

            if (position.x <= 37.5f && x_vector<0 || position.x >= 250.f && x_vector>0) {
                move_x = 0;
            }
            if (position.y <= 49.5f && y_vector<0 || position.y >= 600-49.5 && y_vector>0) {
                move_y = 0;
            }

            move(x_vector*speed*move_x, y_vector*speed*move_y);

            return true;
        }
    
    bool shoot () {

        if (shoot_timer < sf::seconds(0)) {
            position = getPosition();

            Bullet bullet(position.x, position.y+43.5, damage);

            bullet_array.push_back(
                bullet
            );

            shoot_timer = shoot_cooldown;
        }

        return true;
    }

    bool update (sf::Time dt) {

        shoot_timer -= dt;

        return true;
    }

};


class SpaceFighter {

    vector<int> bullets_clear;
    sf::Clock dt_clock;
    sf::Clock spawn_timer;

    vector<Enemy> enemy_array;

    public:
        SpaceFighter() {
            sf::RenderWindow window;

            window.create(sf::VideoMode(800, 600), "Space Fighter!");
            window.setVerticalSyncEnabled(true);
            window.setKeyRepeatEnabled(false);

            Player player;

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    } else if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Space) {
                            player.shoot();
                        }
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    player.movePlayer(-1.f, 0.f);
                } 
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    player.movePlayer(1.f, 0.f);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    player.movePlayer(0.f, 1.f);
                } 
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    player.movePlayer(0.f, -1.f);
                }

                sf::Time elapsed = dt_clock.restart();
                player.update(elapsed);

                if (spawn_timer.getElapsedTime() > sf::seconds(rand() % 100)) {
                    Enemy enemy(100, 100);

                    enemy_array.push_back(
                        enemy
                    );
                    spawn_timer.restart();
                }

                window.clear(sf::Color::Black);

                for (int i=0; i<player.bullet_array.size(); i++) {
                    window.draw(player.bullet_array[i]);
                    player.bullet_array[i].update_position();
                    if (player.bullet_array[i].getPosition().x > 800) {
                        bullets_clear.push_back(i);
                    }
                }
                for (int i=0; i<bullets_clear.size(); i++) {
                    player.bullet_array.erase(player.bullet_array.begin()+i);
                }

                bullets_clear.clear();

                for (int i=0; i<enemy_array.size(); i++) {
                    window.draw(enemy_array[i]);
                }

                window.draw(player);

                window.display();
            }
        }
};

sf::Texture Player::player_texture;
sf::Texture Bullet::bullet_texture;
sf::Texture Enemy::enemy_texture;

int main() {

    SpaceFighter();

    return 0;

}