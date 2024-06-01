#include "Spaceship.h"

sf::Texture spacetexture;
void initSpaceshipTexture() {
    if (!spacetexture.loadFromFile("spaceship.png")) {
        // Handle loading error
        printf("sraka!!");
    }
}

Spaceship::Spaceship(float startX, float startY)
    : position(startX, startY), speed(0, 0), rotation(0.0f) {
    // Load spaceship texture
    sprite.setTexture(spacetexture);
    sprite.setScale(0.02, 0.02);
    sprite.setOrigin(spacetexture.getSize().x / 2, spacetexture.getSize().y / 2); // Set origin to center for proper rotation
}

void Spaceship::setSpeed(float x, float y) {
    speed.x = x;
    speed.y = y;
}

void Spaceship::setRotation(float angle) {
    rotation = angle;
}

sf::Sprite Spaceship::getSprite() {
    return sprite;
}

void Spaceship::update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window) {
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

void Spaceship::draw(sf::RenderWindow& window) {
    // rysujemy sprita
    sprite.setPosition(position);
    sprite.setRotation(rotation);
    window.draw(sprite);
}

void Spaceship::rotate(float angle) {
    rotation += angle;
}

void Spaceship::accelerate(float acceleration) {
    // Calculate new speed based on the rotation
    speed.x += acceleration * std::sin(rotation * DEG_TO_RAD);
    speed.y += acceleration * -std::cos(rotation * DEG_TO_RAD);
}

Projectile Spaceship::shooting(int windowWidth)
{
    return Projectile(position.x, position.y, sin(rotation * DEG_TO_RAD) * 0.4 * windowWidth, -cos(rotation * DEG_TO_RAD) * 0.4 * windowWidth);
}