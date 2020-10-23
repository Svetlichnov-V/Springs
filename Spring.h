#pragma once
#include "SFML/Graphics.hpp"
#include "Sphere.h"
#include "Vector2f.h"

struct Spring
{
	float k;
	float length;
	sf::Color color;
	//int numberSphere1;
	//int numberSphere2;

	//Spring() :
	//	k(0.1), length(10) {}
};

void acceleratóDueToSpring(Sphere* sphere1, Sphere* sphere2, Spring* spring)
{
	float force = spring->k * ((sphere1->position - sphere2->position).mod() - spring->length);
	sphere1->acceleration += (force / sphere1->MASS) * (sphere2->position - sphere1->position).norm();
	sphere2->acceleration += (force / sphere2->MASS) * (sphere1->position - sphere2->position).norm();
}

void drawSpring(Vector2f a, Vector2f b, sf::RenderWindow* window)
{
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(a.x, a.y);
	line[0].color = sf::Color::Blue;
	line[1].color = sf::Color::Blue;
	line[1].position = sf::Vector2f(b.x, b.y);
	window->draw(line);
}

void managerInfluenceSpringsToSpheres(Sphere* spheres, Spring* springs, int numberOfSpheres)
{
	for (int i = 0; i < numberOfSpheres; ++i)
		for (int j = 0; j < i; ++j)
			acceleratóDueToSpring(&spheres[i], &spheres[j], &springs[i][j]);
}