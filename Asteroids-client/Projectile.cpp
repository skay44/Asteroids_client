#include "Projectile.hpp"

sf::Texture projtexture;
void initProjectileTexture() {
    if (!projtexture.loadFromFile("projectile.png")) {
        // Handle loading error
        printf("sraka!!");
    }
}

Projectile::Projectile(float startX, float startY, float speedX, float speedY)
    : position(startX, startY), speed(speedX, speedY) {
    // za³aduj teksturke pocisku
    
    sprite.setTexture(projtexture);
    sprite.setScale(0.05, 0.05);
    sprite.setOrigin(projtexture.getSize().x / 2, projtexture.getSize().y / 2); // Set origin to center for proper rotation
    rotation = atan2(speedY, speedX) / DEG_TO_RAD + 90;
}

void Projectile::update(float deltaTime, int windowWidth, int windowHeight, sf::RenderWindow& window) {
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

void Projectile::draw(sf::RenderWindow& window) {
    // rysujemy sprita
    window.draw(sprite);
}

