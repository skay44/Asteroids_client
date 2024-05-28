#include <SFML/Graphics.hpp>
#include <cmath>
#include <stdio.h>
#include <list>
using namespace sf;

// Helper constants for converting degrees to radians
const float DEG_TO_RAD = 3.14159f / 180.0f;

class Projectile {
public:
    Projectile(float startX, float startY, float speedX, float speedY)
        : position(startX, startY), speed(speedX, speedY) {
        // za³aduj teksturke pocisku
        if (!texture.loadFromFile("projectile.png")) {
            // Handle loading error
            printf("sraka!!");
        }
        sprite.setTexture(texture);
        sprite.setScale(0.05, 0.05);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2); // Set origin to center for proper rotation
        rotation = atan2(speedY, speedX) / DEG_TO_RAD + 90;
    }

    void update(float deltaTime, int windowWidth, int windowHeight, RenderWindow& window) {
        // update pozycji
        position.x += speed.x * deltaTime;
        position.y += speed.y * deltaTime;

        // eleportacja jak jest na krañcu ekranu
        if (position.x < 0) {
            position.x = windowWidth;
        }
        else if (position.x > windowWidth) {
            position.x = 0;
        }
        if (position.y < 0) {
            position.y = windowHeight;
        }
        else if (position.y > windowHeight) {
            position.y = 0;
        }

        sprite.setPosition(position);
        sprite.setRotation(rotation);

        //rysowanie statku
        draw(window);
        if (sprite.getGlobalBounds().top < 0) {
            position.y += windowHeight;
            sprite.setPosition(position);
            draw(window);
            position.y -= windowHeight;
            sprite.setPosition(position);
        }
        else if (sprite.getGlobalBounds().top + sprite.getGlobalBounds().height > windowHeight)
        {
            position.y -= windowHeight;
            sprite.setPosition(position);
            draw(window);
            position.y += windowHeight;
            sprite.setPosition(position);
        }
        if (sprite.getGlobalBounds().left < 0) {
            position.x += windowWidth;
            sprite.setPosition(position);
            draw(window);
            position.x -= windowWidth;
            sprite.setPosition(position);
        }
        else if (sprite.getGlobalBounds().left + sprite.getGlobalBounds().width > windowWidth)
        {
            position.x -= windowWidth;
            sprite.setPosition(position);
            draw(window);
            position.x += windowWidth;
            sprite.setPosition(position);
        }
    }

    void draw(sf::RenderWindow& window) {
        // rysujemy sprita
        window.draw(sprite);
    }

private:
    sf::Vector2f position;
    sf::Vector2f speed;
    float rotation;
    sf::Texture texture;
    sf::Sprite sprite;
};

class Spaceship {
public:
    Spaceship(float startX, float startY)
        : position(startX, startY), speed(0, 0), rotation(0.0f) {
        // Load spaceship texture
        if (!texture.loadFromFile("spaceship.png")) {
            // Handle loading error
            printf("sraka!!");
        }
        sprite.setTexture(texture);
        sprite.setScale(0.02, 0.02);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2); // Set origin to center for proper rotation
    }

    void setSpeed(float x, float y) {
        speed.x = x;
        speed.y = y;
    }

    void setRotation(float angle) {
        rotation = angle;
    }

    Sprite getSprite() {
        return sprite;
    }

    void update(float deltaTime, int windowWidth, int windowHeight, RenderWindow& window) {
        // update pozycji
        position.x += speed.x * deltaTime;
        position.y += speed.y * deltaTime;

        // eleportacja jak jest na krañcu ekranu
        if (position.x < 0) {
            position.x = windowWidth;
        }
        else if (position.x > windowWidth) {
            position.x = 0;
        }
        if (position.y < 0) {
            position.y = windowHeight;
        }
        else if (position.y > windowHeight) {
            position.y = 0;
        }

        sprite.setPosition(position);
        sprite.setRotation(rotation);

        //rysowanie statku
        draw(window);
        if (sprite.getGlobalBounds().top < 0) {
            position.y += windowHeight;
            sprite.setPosition(position);
            draw(window);
            position.y -= windowHeight;
            sprite.setPosition(position);
        }
        else if (sprite.getGlobalBounds().top + sprite.getGlobalBounds().height > windowHeight)
        {
            position.y -= windowHeight;
            sprite.setPosition(position);
            draw(window);
            position.y += windowHeight;
            sprite.setPosition(position);
        }
        if (sprite.getGlobalBounds().left < 0) {
            position.x += windowWidth;
            sprite.setPosition(position);
            draw(window);
            position.x -= windowWidth;
            sprite.setPosition(position);
        }
        else if (sprite.getGlobalBounds().left + sprite.getGlobalBounds().width > windowWidth)
        {
            position.x -= windowWidth;
            sprite.setPosition(position);
            draw(window);
            position.x += windowWidth;
            sprite.setPosition(position);
        }
    }

    void draw(sf::RenderWindow& window) {
        // rysujemy sprita
        window.draw(sprite);
    }

    void rotate(float angle) {
        rotation += angle;
    }

    void accelerate(float acceleration) {
        // Calculate new speed based on the rotation
        speed.x += acceleration * std::sin(rotation * DEG_TO_RAD);
        speed.y += acceleration * -std::cos(rotation * DEG_TO_RAD);
    }

    Projectile* shooting(int windowWidth)
    {
        return new Projectile(position.x, position.y, sin(rotation * DEG_TO_RAD) * 0.1 * windowWidth, -cos(rotation * DEG_TO_RAD) * 0.1 * windowWidth);
    }

private:
    sf::Vector2f position;
    sf::Vector2f speed;
    float rotation;
    sf::Texture texture;
    sf::Sprite sprite;
};


int main() {
    //test2323
    VideoMode desktopMode = VideoMode::getDesktopMode(); //wyci¹gamy rozmiar monitora
    sf::RenderWindow window(desktopMode, "Asteroids Clone", Style::Fullscreen); //tworzymy okno gry

    //inicjalizacja statku, oraz zmiennych odpowiadaj¹cych za poruszanie.
    Spaceship spaceship(desktopMode.width -1, desktopMode.height - 1);
    spaceship.setSpeed(0, 0);
    double rotationSpeed = 100;
    double acceleration = 0.1 * desktopMode.width;

    std::vector<Projectile*> projectiles;

    //zegar bo potem musi byæ delta time ¿eby dzia³a³o jak ma
    sf::Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Tab) { //debug log
                printf("top: %f\n", spaceship.getSprite().getGlobalBounds().top);
                printf("%f\n", spaceship.getSprite().getGlobalBounds().height);
                printf("left: %f\n", spaceship.getSprite().getGlobalBounds().left);
                printf("%f\n", spaceship.getSprite().getGlobalBounds().width);
            }
            if (event.type == sf::Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) //zamykanie okna x'em
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        // Handle input
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            spaceship.rotate(-rotationSpeed * deltaTime); // lewa rotacja
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            spaceship.rotate(rotationSpeed * deltaTime); // prawa rotacja
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            spaceship.accelerate(acceleration * deltaTime); // przyœpieszenie
        }
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            projectiles.push_back(spaceship.shooting(desktopMode.width));
        }

        window.clear();

        // Update spaceship
        spaceship.update(deltaTime, desktopMode.width, desktopMode.height, window);
        for (int i = 0; i < projectiles.size(); i++) {
            projectiles[i]->update(deltaTime, desktopMode.width, desktopMode.height, window);
        }

        window.display();
    }

    return 0;
}
