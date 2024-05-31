#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <stdio.h>
#include <list>
#include "utils.h"


class Projectile {
public:
    Projectile(float startX, float startY, float speedX, float speedY);

    void update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

private:
    sf::Vector2f position;
    sf::Vector2f speed;
    float rotation;
    sf::Texture texture;
    sf::Sprite sprite;
};