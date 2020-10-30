#pragma once
#include "SFML/Graphics.hpp"
#include "Sphere.h"
#include "Vector2f.h"

struct Spring
{
	float k;
	float length;
	sf::Color color;
	Sphere* sphere1;
	Sphere* sphere2;

	void acceleratóDueToSpring()
	{
		float force = k * ((sphere1->position - sphere2->position).mod() - length);
		sphere1->acceleration += (force / sphere1->MASS) * (sphere2->position - sphere1->position).norm();
		sphere2->acceleration += (force / sphere2->MASS) * (sphere1->position - sphere2->position).norm();
	}

	void drawSpring(sf::RenderWindow* window)
	{
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(sphere1->position.x, sphere1->position.y);
		line[1].position = sf::Vector2f(sphere2->position.x, sphere2->position.y);
		line[0].color = color;
		line[1].color = color;
		window->draw(line);
	}
};

void managerInfluenceSpringsToSpheres(Spring* springs, int numberOfSprings)
{
	for (int i = 0; i < numberOfSprings; ++i)
		springs[i].acceleratóDueToSpring();
}

void managerDrawSprings(Spring* springs, int numberOfSprings, sf::RenderWindow* window)
{
	for (int i = 0; i < numberOfSprings; ++i)
		springs[i].drawSpring(window);
}