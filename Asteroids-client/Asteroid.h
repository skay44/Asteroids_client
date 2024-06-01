#pragma once
#include "Spaceship.h"
#include "utils.h"

void initAsteroidTexture();

class Asteroid {
public:
    Asteroid(float startX, float startY, float speedX, float speedY, int id, int size);

    void update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

    sf::Vector2f position;
    sf::Vector2f speed;
    float rotation;
    sf::Sprite sprite;
    int id;
    int size;
};