#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <math.h>
#include <cassert>
#include "Vector2f.h"

struct Sphere
{
    Vector2f position = Vector2f(0, 0);
    Vector2f oldPosition = Vector2f(0, 0);
    Vector2f velocity = Vector2f(0, 0);
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

void moveSphere(Sphere* sphere, const float DT)
{
    assert(sphere != 0);

    sphere->position += sphere->velocity * DT + 0.5 * sphere->acceleration * DT * DT;

    sphere->velocity += sphere->acceleration * DT;
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

    Vector2f dp = sphere1->position - sphere2->position;

    float distanceSquared = dp.mod();

    return (distanceSquared < commonRadiusSquared);
}

float reducedMass(const float m1, const float m2)
{
    assert(m1 != 0 && m2 != 0);

    float rm = (m1 * m2) / (m1 + m2);
    return rm;
}

void resolutionSphereOnCollision(Sphere* sphere1, Sphere* sphere2)
{
    assert(sphere1 != 0);
    assert(sphere2 != 0);
    assert(sphere1->MASS != 0 && sphere2->MASS != 0);


    Vector2f axis = sphere1->position - sphere2->position;
    float moduleAxis = axis.mod();

    if (moduleAxis == 0)
        assert(false);

    float projectionVectorOnSpeedSphere1 = sphere1->velocity.projection(axis);
    float projectionVectorOnSpeedSphere2 = sphere2->velocity.projection(axis);

    float reducedMassSpheres = reducedMass(sphere1->MASS, sphere2->MASS);

    float dv1 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere1->MASS);
    float dv2 = reducedMassSpheres * (projectionVectorOnSpeedSphere1 - projectionVectorOnSpeedSphere2) / (sphere2->MASS);

    int commonRadius = (sphere1->radius) + (sphere2->radius);

    float distance = (sphere1->position - sphere2->position).mod();

    if (dv1 < 0)
    {
        sphere1->velocity -= 2 * dv1 * axis.norm();
    }

    if (dv2 < 0)
    {
        sphere2->velocity -= 2 * dv2 * axis.norm();
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

    sphere->acceleration -= sphere->velocity * coefficientSlowdown;
}

void controlSphere(Sphere* sphere, const float controllability)
{
    if (sphere == 0)
        assert(false);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        Vector2f mousePosition (sf::Mouse::getPosition());

        Vector2f difPositionSphereAndMouse = mousePosition - sphere->position;

        sphere->acceleration += controllability * difPositionSphereAndMouse.norm();
    }
}

//Ìåíåäæåðû

void managerOfColisionOnSpheres(Sphere* spheres, int numberOfSpheres, const int X_MAX, const int Y_MAX)
{
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        collisionSphere(&spheres[i], X_MAX, Y_MAX);
    }
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        for (int j = i + 1; j < numberOfSpheres; ++j)
        {
            if (isCollidedTwoSpheres(&spheres[i], &spheres[j]))
                resolutionSphereOnCollision(&spheres[i], &spheres[j]);
        }
    }
}

void managerAcceleration(Sphere* spheres, int numberOfSpheres, const float coefficientSlowdown = 0, const float controllability = 0)
{
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        nullAccelSphere(&spheres[i]);
        slowdownSphere(&spheres[i], coefficientSlowdown);
        controlSphere(&spheres[i], controllability);
    }
}

void managerMovingSpheres(Sphere* spheres, int numberOfSpheres, float dt)
{
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        moveSphere(&spheres[i], dt);
    }
}

void managerDrawSpheres(Sphere* spheres, int numberOfSpheres, int numberOfCicles, sf::RenderWindow* window)
{
    for (int i = 0; i < numberOfSpheres; ++i)
    {
        drawSphere(window, &spheres[i], numberOfCicles);
    }
}