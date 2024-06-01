#pragma once
#include "Projectile.hpp"
#include "utils.h"

void initSpaceshipTexture();

class Spaceship {
public:
    Spaceship(float startX, float startY, bool isMe, int id);

    void setSpeed(float x, float y);

    void setRotation(float angle);

    sf::Sprite getSprite();

    void update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

    void rotate(float angle);

    void accelerate(float acceleration);

    Projectile shooting(int windowWidth);

    sf::Vector2f position;
    sf::Vector2f speed;
    float rotation;
    sf::Sprite sprite;
    int id;
};