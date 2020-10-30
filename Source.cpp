#pragma once
#include "SFML/Graphics.hpp"
#include "Sphere.h"
#include "Spring.h"
#include "Vector2f.h"

void handlingEvent(sf::RenderWindow* window)
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
    }
}

int main()
{
    const int X_MAX = 1960;
    const int Y_MAX = 1080;

    float dt = 0.1;

    float coefficientSlowdown = 0;

    sf::Uint8 redFill = 10;
    sf::Uint8 greenFill = 10;
    sf::Uint8 blueFill = 10;
    sf::Color fillColor = sf::Color(redFill, greenFill, blueFill);
    sf::Color lineColor = sf::Color(redFill, greenFill, blueFill);

    int numberOfCicles = 25;

    int radius = 25;
    float SPHERE_MASS = 1;

    sf::Color colorSphere = sf::Color(0, 0, 200);
    sf::Color colorTrackSphere = fillColor;

    const int numberOfSpheres = 3;
    const int numberOfSprings = 3;
    Sphere spheres[numberOfSpheres];

    sf::Clock clock;
    int time = clock.getElapsedTime().asMilliseconds() + 400;
    srand(time);
    std::cout << time;
    std::cout << std::endl;

    spheres[0] = Sphere{ Vector2f(800, 400), Vector2f(200, 400), Vector2f(0, 0) , Vector2f(0, 0), radius, 1, colorSphere, colorTrackSphere };
    spheres[1] = Sphere{ Vector2f(800, 800), Vector2f(200, 400), Vector2f(0, 0) , Vector2f(0, 0), radius, 1, colorSphere, colorTrackSphere };
    spheres[2] = Sphere{ Vector2f(1200, 600), Vector2f(200, 400), Vector2f(0, 0) , Vector2f(0, 0), radius, 1, colorSphere, colorTrackSphere };

    //managerCreatingSpheres(spheres, numberOfSpheres, X_MAX, Y_MAX, radius, SPHERE_MASS, colorSphere, colorTrackSphere, time);

    Spring springs[numberOfSprings];

    springs[0] = Spring{ 0.01, 400, sf::Color::Blue, 0, 1 };
    springs[1] = Spring{ 0.05, 600, sf::Color::Blue, 2, 1 };
    springs[2] = Spring{ 0.01, 800, sf::Color::Blue, 0, 2 };

    /*
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        bool flag = false;
        while (!flag)
        {
            Vector2f position( rand() % X_MAX, rand() % Y_MAX );
            Vector2f velocity{ rand() % 200, rand() % 200 };
            Sphere sphere{ position,     position,     velocity,     Vector2f (0, 0), radius, SPHERE_MASS, colorSphere, colorTrackSphere };

            bool flag = true;

            for (int j = 0; j < i; ++j)
            {
                 int commonRadius = (sphere.radius) + (spheres[j].radius);
                 float dx = (sphere.position.x) - (spheres[j].position.x);
                 float dy = (sphere.position.y) - (spheres[j].position.y);
                 float distance = pow(dx * dx + dy * dy, 0.5);

                 flag = distance > commonRadius;
                 if (!flag)
                 {
                     break;
                 }
            }
            if (flag)
            {
                std::cout << 1;
                spheres[i] = sphere;
            }
        }
    }
    */
    /*
    Spring springs[numberOfSpheres][numberOfSpheres];
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        springs[i][i] = Spring{ 0, 0 };

        for (int j = 0; j < i; ++j)
        {
            float a = (rand() % 100) / 25;
            float b = rand() % 800;
            springs[i][j] = springs[j][i] = Spring{ a, b };
        }
    }
    */


    sf::RenderWindow window(sf::VideoMode(X_MAX, Y_MAX), "Main window");
    window.clear(fillColor);
    window.display();

    float time1 = clock.getElapsedTime().asSeconds();
    float time2 = clock.getElapsedTime().asSeconds();

    while (window.isOpen())
    {
        time1 = clock.getElapsedTime().asSeconds();
        dt = time1 - time2;
        //std::cout << 1.0 / (time1 - time2);
        time2 = time1;
        //std::cout << "/n";

        menegerAcceleration(spheres, numberOfSpheres, coefficientSlowdown);
        managerInfluenceSpringsToSpheres(spheres, springs, numberOfSpheres, numberOfSprings);
        menegerMovingSpheres(spheres, numberOfSpheres, dt);

        handlingEvent(&window);

        window.clear(fillColor);
        managerDrawSprings(springs, spheres, numberOfSprings, numberOfSpheres, &window);
        menegerDrawSpheres(spheres, numberOfSpheres, numberOfCicles, &window);
        window.display();
    }

	return 0;
}