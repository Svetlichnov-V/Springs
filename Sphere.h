#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <math.h>
#include <cassert>
#include "Vector2f.h"

//struct Vector2f
//{
//    float x;
 //   float y;   sf::Color colorSphere, sf::Color colorTrackSphere
//}

/*
void managerCreatingSpheres(Sphere* spheres, const int numberOfSpheres, int X_MAX, int Y_MAX, float radius,
    float SPHERE_MASS, sf::Color colorSphere, sf::Color colorTrackSphere, float init)
{
    srand(init)
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        bool flag = false;
        while (!flag)
        {
            Vector2f position(rand() % X_MAX, rand() % Y_MAX);
            Vector2f velocity{ rand() % 200, rand() % 200 };
            Sphere sphere{ position,     position,     velocity,     Vector2f(0, 0), radius, SPHERE_MASS, colorSphere, colorTrackSphere };

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
                    flag = false;
                    break;
                }
                spheres[i] = sphere;
            }
        }
    }
}
*/

struct Sphere
{
    Vector2f position     = Vector2f(0, 0);
    Vector2f oldPosition  = Vector2f(0, 0);
    Vector2f velocity     = Vector2f(0, 0);
    Vector2f acceleration = Vector2f(0, 0);
    int radius;
    float MASS;
    sf::Color colorSphere;
    sf::Color colorTrack;
};

void drawSphere(sf::RenderWindow* window, Sphere* sphere, int numberOfCicles, bool constColour = false)
{
    assert(window != 0);
    assert(sphere != 0);
    assert(numberOfCicles != 0);

    int red = sphere->colorSphere.r;
    int green = sphere->colorSphere.g;
    int blue = sphere->colorSphere.b;

    int currentCircleRed = red;
    int currentCircleGreen = green;
    int currentCircleBlue = blue;

    int sphereRadius = sphere->radius;

    int x0 = sphere->position.x;
    int y0 = sphere->position.y;

    sf::CircleShape circle(sphereRadius);

    for (int i = 0; i < numberOfCicles; i++)
    {
        if (!constColour)
        {
            currentCircleRed = red * i / numberOfCicles;
            currentCircleGreen = green * i / numberOfCicles;
            currentCircleBlue = blue * i / numberOfCicles;
        }

        int currentCircleRadius = sphereRadius - sphereRadius * i / numberOfCicles;
        int x = x0 + (sphereRadius * i / 2) / numberOfCicles;
        int y = y0 - (sphereRadius * i / 2) / numberOfCicles;

        sf::Color currentCircleColor = sf::Color(currentCircleRed, currentCircleGreen, currentCircleBlue);

        //sf::CircleShape circle(currentCircleRadius, (15 * (numberOfCicles - i) / numberOfCicles) + 5);
        circle.setRadius(currentCircleRadius);
        circle.setFillColor(currentCircleColor);
        circle.setOutlineColor(currentCircleColor);
        circle.setPosition(x - currentCircleRadius, y - currentCircleRadius);
        window->draw(circle);
    }
}

void nullAccelSphere(Sphere* sphere)
{
    sphere->acceleration = Vector2f{ 0,0 };
}

void menegerAcceleration(Sphere* spheres, int numberOfSpheres)
{
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        nullAccelSphere(&spheres[i]);
    }
}

void moveSphere(Sphere* sphere, const float DT)
{
    assert(sphere != 0);

    sphere->position.x += sphere->velocity.x * DT + 0.5 * sphere->acceleration.x * DT * DT;
    sphere->position.y += sphere->velocity.y * DT + 0.5 * sphere->acceleration.y * DT * DT;

    sphere->velocity.x += sphere->acceleration.x * DT;
    sphere->velocity.y += sphere->acceleration.y * DT;
}

void collisionSphere(Sphere* sphere, const int X_MAX, const int Y_MAX)
{
    assert(sphere != 0);

    if (sphere->position.x > X_MAX - (sphere->radius) && (sphere->velocity.x) > 0)
    {
        sphere->velocity.x = -sphere->velocity.x;
        sphere->position.x -= 2 * (X_MAX - sphere->position.x - sphere->radius);
    }
    if (sphere->position.x < (sphere->radius) && (sphere->velocity.x) < 0)
    {
        sphere->velocity.x = -sphere->velocity.x;
        sphere->position.x -= 2 * (sphere->position.x - sphere->radius);
    }
    if ((sphere->position.y > Y_MAX - (sphere->radius)) && (sphere->velocity.y) > 0)
    {
        sphere->velocity.y = -sphere->velocity.y;
        sphere->position.y -= 2 * (Y_MAX - sphere->position.y - sphere->radius);
    }
    if ((sphere->position.y < (sphere->radius)) && sphere->velocity.y < 0)
    {
        sphere->velocity.y = -sphere->velocity.y;
        sphere->position.y -= 2 * (sphere->position.y - sphere->radius);
    }
}

bool isCollidedTwoSpheres(Sphere* sphere1, Sphere* sphere2)
{
    assert(sphere1 != 0);
    assert(sphere2 != 0);

    int commonRadius = (sphere1->radius) + (sphere2->radius);
    int commonRadiusSquared = commonRadius * commonRadius;

    float dx = (sphere1->position.x) - (sphere2->position.x);
    float dy = (sphere1->position.y) - (sphere2->position.y);

    float distanceSquared = dx * dx + dy * dy;

    return (distanceSquared < commonRadiusSquared);
}

float projectionVector(float xVector, float yVector, float xAxis, float yAxis)
{
    float scalarComposition = xVector * xAxis + yVector * yAxis;
    float moduleAxis = pow(xAxis * xAxis + yAxis * yAxis, 0.5);

    assert(moduleAxis != 0);

    float pV = scalarComposition / moduleAxis;
    return pV;
}

float reducedMass(const float m1, const float m2)
{
    assert(m1 != 0 && m2 != 0);

    float rm = (m1 * m2) / (m1 + m2);
    return rm;
}

float moduleVector(float x, float y)
{
    return pow(x * x + y * y, 0.5);
}

void resolutionSphereOnCollision(Sphere* sphere1, Sphere* sphere2)
{
    assert(sphere1 != 0);
    assert(sphere2 != 0);
    assert(sphere1->MASS != 0 && sphere2->MASS != 0);

    float xAxis = sphere1->position.x - sphere2->position.x;
    float yAxis = sphere1->position.y - sphere2->position.y;
    float moduleAxis = pow(xAxis * xAxis + yAxis * yAxis, 0.5);

    if (moduleAxis == 0)
        assert(false);

    float projectionVectorOnSpeedSphere1 = projectionVector(sphere1->velocity.x, sphere1->velocity.y, xAxis, yAxis);
    float projectionVectorOnSpeedSphere2 = projectionVector(sphere2->velocity.x, sphere2->velocity.y, xAxis, yAxis);

    float reducedMassSpheres = reducedMass(sphere1->MASS, sphere2->MASS);

    float dv1 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere1->MASS);
    float dv2 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere2->MASS);

    int commonRadius = (sphere1->radius) + (sphere2->radius);

    float dx = (sphere1->position.x) - (sphere2->position.x);
    float dy = (sphere1->position.y) - (sphere2->position.y);

    float distance = pow(dx * dx + dy * dy, 0.5);

    /*
    float dr = commonRadius - distance;
    float dt = dr / (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2);
    if (dt < 0)
        dt = -dt;
    std::cout << projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2;
    std::cout << dr;
    std::cout << dt;
    std::cout << '/n';
    */

    //float commonDiferenceVelocity = dv1 + dv2;

    if (dv1 < 0)
    {
        //sphere1->position.x -= sphere1->velocity.x * dt;
        //sphere1->position.y -= sphere1->velocity.y * dt;

        sphere1->velocity.x += -2 * dv1 * xAxis / moduleAxis;
        sphere1->velocity.y += -2 * dv1 * yAxis / moduleAxis;

        //sphere1->position.x += sphere1->velocity.x * dt;
        //sphere1->position.y += sphere1->velocity.y * dt;
    }

    if (dv2 < 0)
    {
        //sphere2->position.x -= sphere1->velocity.x * dt;
        //sphere2->position.y -= sphere1->velocity.y * dt;

        sphere2->velocity.x += 2 * dv2 * xAxis / moduleAxis;
        sphere2->velocity.y += 2 * dv2 * yAxis / moduleAxis;

        //sphere1->position.x += sphere1->velocity.x * dt;
        //sphere1->position.y += sphere1->velocity.y * dt;
    }
}


void drawTrack(sf::RenderWindow* window, Sphere* sphere, int numberOfCiclesInDrawTrack)
{
    if (window == 0)
        assert(false);
    if (sphere == 0)
        assert(false);
    if (numberOfCiclesInDrawTrack == 0)
        assert(false);

    float xNew = sphere->position.x;
    float yNew = sphere->position.y;
    float xOld = sphere->oldPosition.x;
    float yOld = sphere->oldPosition.y;
    float radius = sphere->radius;

    sf::Color trackColor = sphere->colorTrack;
    sf::CircleShape circle = sf::CircleShape(radius);
    circle.setFillColor(trackColor);
    circle.setOutlineColor(trackColor);

    for (int i = 0; i <= numberOfCiclesInDrawTrack; ++i)
    {
        circle.setPosition(xNew + (xOld - xNew) * i / numberOfCiclesInDrawTrack - radius, yNew + (yOld - yNew) * i / numberOfCiclesInDrawTrack - radius);
        window->draw(circle);
    }

    Sphere sphereOld{ Vector2f { xOld, yOld}, Vector2f { xOld, yOld}, Vector2f { 0, 0 }, Vector2f { 0, 0 },
           radius, 0, sphere->colorTrack, sphere->colorTrack };

    drawSphere(window, &sphereOld, numberOfCiclesInDrawTrack, true);
}

void slowdownSphere(Sphere* sphere, const float coefficientSlowdown)
{
    if (sphere == 0)
        assert(false);

    sphere->acceleration.x += -sphere->velocity.x * coefficientSlowdown;
    sphere->acceleration.y += -sphere->velocity.y * coefficientSlowdown;
}

void controlSphere(Sphere* sphere, const float controllability)
{
    if (sphere == 0)
        assert(false);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i position = sf::Mouse::getPosition();

        float differnceMouseXAndX1 = position.x - sphere->position.x;
        float differnceMouseYAndY1 = position.y - sphere->position.y;
        float modeleDiffernceMouseAndSphere1 = moduleVector(differnceMouseXAndX1, differnceMouseYAndY1);

        sphere->acceleration.x += controllability * differnceMouseXAndX1 / modeleDiffernceMouseAndSphere1;
        sphere->acceleration.y += controllability * differnceMouseYAndY1 / modeleDiffernceMouseAndSphere1;
    }
}

//Менеджеры

void menegerAcceleration(Sphere* spheres, int numberOfSpheres, const float coefficientSlowdown = 0 , const float controllability = 0)
{
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        nullAccelSphere(&spheres[i]);
        slowdownSphere (&spheres[i], coefficientSlowdown);
        controlSphere  (&spheres[i], controllability);
    }
}
