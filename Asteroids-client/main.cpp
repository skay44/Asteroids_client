#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <cmath>
#include <stdio.h>
#include <list>
#include <io.h>

#include <winsock2.h>

#include "Projectile.hpp"
#include "Spaceship.h"

using namespace sf;

#define DATA_PACKET_SIZE 10
#define PERIODIC_PACKET_SEND
#define WAIT_TIME 0.5

void GameplayLoop(int connection) {
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

    //data to send
    int data[DATA_PACKET_SIZE];

#ifdef PERIODIC_PACKET_SEND
    double timePassed = 0;
#endif

    while (window.isOpen()) {
        data[0] = 12323;
        for (int i = 1; i < DATA_PACKET_SIZE; i++) {
            data[i] = 0;
        }

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
            data[1] += 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            spaceship.rotate(rotationSpeed * deltaTime); // prawa rotacja
            data[1] += 2;
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            spaceship.accelerate(acceleration * deltaTime); // przyœpieszenie
            data[1] += 4;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            projectiles.push_back(spaceship.shooting(desktopMode.width));
            data[1] += 8;
        }

        window.clear();

        // Update spaceship
        spaceship.update(deltaTime, desktopMode.width, desktopMode.height, window);
        for (int i = 0; i < projectiles.size(); i++) {
            projectiles[i]->update(deltaTime, desktopMode.width, desktopMode.height, window);
        }

#ifdef PERIODIC_PACKET_SEND
        timePassed += deltaTime;
        if (timePassed > WAIT_TIME) {
            send(connection, (const char*)data, sizeof(data), 0);
            timePassed -= WAIT_TIME;
            printf("S");
        }
#elif
        send(connection, (const char*)data, DATA_PACKET_SIZE * sizeof(int), 0);
#endif // PERIODIC_PACKET_SEND

        window.display();
    }
}

int main() {
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct hostent* host = gethostbyname("localhost");
    if (host == NULL) {
        return 45;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2278);
    memcpy(&addr.sin_addr,host->h_addr_list[0],host->h_length);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        return 47;
    }

    if (connect(sockfd, (const struct sockaddr*)&addr, sizeof(addr))) {
        return 46;
    }

    const char* text = "connected";
    send(sockfd, text, strlen(text), 0);
    GameplayLoop(sockfd);

    closesocket(sockfd);



    WSACleanup();
    return 0;
}
