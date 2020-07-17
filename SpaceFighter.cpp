#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;


class Player: public sf::Sprite {
    float speed = 5.f;
    int health = 10;
    int damage = 1;

    sf::Texture player_texture;
    
    public:
        Player () {
            if(!player_texture.loadFromFile("assets/playerShip1_blue.png")) {
                std::cout << "Error loading texture";
            }
            player_texture.setSmooth(true);

            setTexture(player_texture);

            setScale(sf::Vector2f(0.75f, 0.75f));

            setOrigin(sf::Vector2f(49.5f, 37.5f));
            setRotation(90.f);
            setPosition(sf::Vector2f(37.5f + 5, 49.5f + 5));
        }

        bool MovePlayer (float x, float y) {
            move(x*speed, y*speed);

            return true;
        }
};


class SpaceFighter {
    public: SpaceFighter() {
        sf::RenderWindow window;

        window.create(sf::VideoMode(800, 600), "Space Fighter!");
        window.setVerticalSyncEnabled(true);

        Player player;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player.MovePlayer(-1.f, 0.f);
            } 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player.MovePlayer(1.f, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                player.MovePlayer(0.f, 1.f);
            } 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                player.MovePlayer(0.f, -1.f);
            }

            window.clear(sf::Color::Black);

            window.draw(player);

            window.display();
        }
    }
};

int main() {

    SpaceFighter();

    return 0;

}