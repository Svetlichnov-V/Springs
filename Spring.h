#pragma once
#include "SFML/Graphics.hpp"
#include "Sphere.h"
#include "Vector2f.h"

struct Spring
{
	float k;
	float length;
	sf::Color color;
	int numberSphere1;
	int numberSphere2;

	//Spring() :
	//	k(0.1), length(10) {}
};

void acceleratóDueToSpring(Sphere* sphere1, Sphere* sphere2, Spring* spring)
{
	float force = spring->k * ((sphere1->position - sphere2->position).mod() - spring->length);
	sphere1->acceleration += (force / sphere1->MASS) * (sphere2->position - sphere1->position).norm();
	sphere2->acceleration += (force / sphere2->MASS) * (sphere1->position - sphere2->position).norm();
}

void drawSpring(Spring* spring, Sphere* spheres, int numberOfSpheres, sf::RenderWindow* window)
{
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(spheres[spring->numberSphere1].position.x, spheres[spring->numberSphere1].position.y);
	line[1].position = sf::Vector2f(spheres[spring->numberSphere2].position.x, spheres[spring->numberSphere2].position.y);
	line[0].color = spring->color;
	line[1].color = spring->color;
	window->draw(line);
}

void managerInfluenceSpringsToSpheres(Sphere* spheres, Spring* springs, int numberOfSpheres, int numberOfSprings)
{
	for (int i = 0; i < numberOfSprings; ++i)
		acceleratóDueToSpring(&spheres[springs[i].numberSphere1], &spheres[springs[i].numberSphere2], &springs[i]);
}

void managerDrawSprings(Spring* springs, Sphere* spheres, int numberOfSprings, int numberOfSpheres, sf::RenderWindow* window)
{
	for (int i = 0; i < numberOfSprings; ++i)
		drawSpring(&springs[i], spheres, numberOfSpheres, window);
}