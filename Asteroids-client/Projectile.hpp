#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <stdio.h>
#include <list>
#include "utils.h"

void initProjectileTexture();

class Projectile {
public:
    Projectile(float startX, float startY, float speedX, float speedY);

    void update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);


    sf::Vector2f position;
    sf::Vector2f speed;
    float rotation;
    sf::Sprite sprite;
};