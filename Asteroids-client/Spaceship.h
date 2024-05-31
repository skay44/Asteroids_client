#pragma once
#include "Projectile.hpp"
#include "utils.h"

class Spaceship {
public:
    Spaceship(float startX, float startY);

    void setSpeed(float x, float y);

    void setRotation(float angle);

    sf::Sprite getSprite();

    void update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

    void rotate(float angle);

    void accelerate(float acceleration);

    Projectile* shooting(int windowWidth);

private:
    sf::Vector2f position;
    sf::Vector2f speed;
    float rotation;
    sf::Texture texture;
    sf::Sprite sprite;
};