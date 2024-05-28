#include <SFML/Graphics.hpp>
#include <cmath>
#include <stdio.h>
#include <list>

#include "Projectile.hpp"
#include "Spaceship.h"

using namespace sf;

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
