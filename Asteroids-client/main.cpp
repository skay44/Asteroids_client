#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define PLAYER_CODE 0b10000001
#define PROJECTILE_CODE 0b10000010

#include <SFML/Graphics.hpp>
#include <cmath>
#include <stdio.h>
#include <list>
#include <io.h>
#include <thread>

#include <winsock2.h>

#include "Projectile.hpp"
#include "Spaceship.h"

using namespace sf;

#pragma pack(push,1)
typedef struct {
    unsigned char ID;
    unsigned char Shot;
    char KeyboardKeys;
    float XPosition;
    float YPosition;
    float XVelocity;
    float YVelocity;
    float angle;
    float XPositionShot;
    float YPositionShot;
    float XVelocityShot;
    float YVelocityShot;
} Frame;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct _sendFrameEntity {
    unsigned char header;     //kod okreslajacy co to za rodzaj ramki (0b1000001 - gracz, 0b10000010 - asteroida)
    short ID;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} sendFrameEntity;
#pragma pack(pop)

Mutex coopMutex;
Mutex connectedMutex;

//size of temporary data packet - we should determine the details of how to send data in order to do anything further
#define DATA_PACKET_SIZE 39
//if defined packets will be send every "WAIT_TIME" seconds, instead of all the time
#define PERIODIC_PACKET_SEND
#define WAIT_TIME 0.5

void receive(int connection, Spaceship &player, bool& connected) {
    int bytes_received = 0;
    int total_bytes_received = 0;
    char readBuffer[sizeof(sendFrameEntity)];
    sendFrameEntity frame;
    while (true) {
        total_bytes_received = 0;
        bytes_received = recv(connection, readBuffer, 1, 0);    //wczytywanie 1 bajtu do odczytania headera
        if (bytes_received <= 0) {
            // handle errors or connection closed
            perror("recv failed");
            _close(connection);
            connected = false;
            return;
        }
        total_bytes_received += bytes_received;
        unsigned char header = readBuffer[0];   //pobieranie headera
        connected = true;

        if (header == PLAYER_CODE || header == PROJECTILE_CODE) { //ramka gracza i asteroid
            // Loop to ensure we receive the complete frame
            while (total_bytes_received < sizeof(sendFrameEntity)) {
                bytes_received = recv(connection, readBuffer + total_bytes_received, sizeof(sendFrameEntity) - total_bytes_received, 0);
                if (bytes_received <= 0) {
                    // handle errors or connection closed
                    perror("recv failed");
                    _close(connection);
                    return;
                }
                total_bytes_received += bytes_received;
            }
            // Now we have a complete frame in readBuffer
            sendFrameEntity* f = (sendFrameEntity*)readBuffer;
            if (f->header == PLAYER_CODE) {
                coopMutex.lock();
                player.position.x = f->posX;
                player.position.y = f->posY;
                player.speed.x = f->speedX;
                player.speed.y = f->speedY;
                player.rotation = f->rotation;
                printf("sraka %f\n", f->posX);
                coopMutex.unlock();
            }
            else {
                printf("Get data from server. Asteroid data: %d\n", f->ID);
            }

        }
        /*else {//inna ramka niz gracz i asteroida
            while (total_bytes_received < sizeof(sendFrameSerwerInfo)) {
                bytes_received = recv(sockfd, readBuffer + total_bytes_received, sizeof(sendFrameSerwerInfo) - total_bytes_received, 0);
                if (bytes_received <= 0) {
                    // handle errors or connection closed
                    perror("recv failed");
                    close(sockfd);
                    return NULL;
                }
                total_bytes_received += bytes_received;
            }
            printf("Unknown frame. Header: %d\n", header);
        }*/
    }
    
}

void GameplayLoop(int connection) {
    //test2323
    VideoMode desktopMode = VideoMode::getDesktopMode(); //wyci¹gamy rozmiar monitora
    RenderWindow window(desktopMode, "Asteroids Clone", Style::Default); //tworzymy okno gry

    //inicjalizacja statku, oraz zmiennych odpowiadaj¹cych za poruszanie.
    initSpaceshipTexture();
    Spaceship spaceship(desktopMode.width -1, desktopMode.height - 1);
    Spaceship cooperator(0, 0);
    bool cooperatorConnected = false;
    spaceship.setSpeed(0, 0);
    double rotationSpeed = 100;
    double acceleration = 0.1 * desktopMode.width;
    double maxSpeed = 1 * desktopMode.width;
    double shotCooldown = 0.05;
    double shotTimer = 0;

    std::vector<Projectile> projectiles;
    initProjectileTexture();

    double sendTimer = 0;
    int shot = 0;
    Projectile proj(0, 0, 0, 0);

    //zegar bo potem musi byæ delta time ¿eby dzia³a³o jak ma
    Clock clock;

    //data to send
    unsigned char data[DATA_PACKET_SIZE];
    unsigned char fakeData[DATA_PACKET_SIZE];
    std::thread cum{ receive, connection, std::ref(cooperator), std::ref(cooperatorConnected) };

    while (window.isOpen()) {
        fakeData[0] = 12323; //id of packet (first 4 bytes of packet) 12323 - defoult id for movement
        for (int i = 1; i < DATA_PACKET_SIZE; i++) {
            fakeData[i] = 0;
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

        int keys = 0;

        // Handle input
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            spaceship.rotate(-rotationSpeed * deltaTime); // lewa rotacja
            keys += 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            spaceship.rotate(rotationSpeed * deltaTime); // prawa rotacja
            keys += 2;
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            spaceship.accelerate(acceleration * deltaTime); // przyœpieszenie
            keys += 4;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (shotTimer > shotCooldown)
            {
                proj = spaceship.shooting(desktopMode.width);
                projectiles.push_back(proj);
                shotTimer = 0;
                shot = 1;
            }
        }
        shotTimer += deltaTime;

        Frame frame = {1, shot, keys, spaceship.position.x, spaceship.position.y, spaceship.speed.x, spaceship.speed.y, spaceship.rotation, proj.position.x, proj.position.y, proj.speed.x, proj.speed.y };

        if (sendTimer > shotCooldown) {
            send(connection, (const char*)&frame, sizeof(Frame), 0);
            sendTimer = 0;
            shot = 0;
        }
        else {
            sendTimer += deltaTime;
        }


        window.clear();

        // Update spaceship
        spaceship.update(deltaTime, desktopMode.width, desktopMode.height, window);
        for (int i = 0; i < projectiles.size(); i++) {
            projectiles[i].update(deltaTime, desktopMode.width, desktopMode.height, window);
        }
        if (cooperatorConnected) {
            coopMutex.lock();
            cooperator.draw(window);
            coopMutex.unlock();
        }


        window.display();
    }
}

int main() {

    //initializing Windows Socket API (windows moment)
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    //obtaining local ip adress
    struct hostent* host = gethostbyname("localhost");
    if (host == NULL) {
        return 45;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    //htons - converts data in pc format to network format (probably little endian to big endian idk)
    //based on this port, clients will select the appropriate server in the local network to connect to
    addr.sin_port = htons(2278);
    memcpy(&addr.sin_addr,host->h_addr_list[0],host->h_length);

    //creating socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        return 47;
    }

    //connecting with serwer using sockaddr_in adress
    if (connect(sockfd, (const struct sockaddr*)&addr, sizeof(addr))) {
        return 46;
    }
    const char* text = "connected";
    //send(sockfd, text, strlen(text), 0);

    //main gameplay loop
    GameplayLoop(sockfd);

    closesocket(sockfd);

    WSACleanup();
    return 0;
}
