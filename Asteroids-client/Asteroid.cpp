#include "Asteroid.h"

sf::Texture asteroidtexture;
void initAsteroidTexture() {
    if (!asteroidtexture.loadFromFile("asteroid.png")) {
        // Handle loading error
        printf("sraka!!");
    }
}

Asteroid::Asteroid(float startX, float startY, float speedX, float speedY, int id, int size)
    : position(startX, startY), speed(speedX, speedY) {

    sprite.setTexture(asteroidtexture);
    sprite.setScale(0.05 * size, 0.05 * size);
    sprite.setOrigin(asteroidtexture.getSize().x / 2, asteroidtexture.getSize().y / 2); // Set origin to center for proper rotation
    rotation = atan2(speedY, speedX) / DEG_TO_RAD + 90;
    this->id = id;
    this->size = size;
    //printf("%f %f",sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
}

void Asteroid::update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window) {
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

void Asteroid::draw(sf::RenderWindow& window) {
    // rysujemy sprita
    window.draw(sprite);
}