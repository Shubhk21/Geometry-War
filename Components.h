#pragma once
#include"Vec2.h"
#include<SFML/Graphics.hpp>
constexpr float PI = 3.14159f;

class CTransform {

public:

	Vec2 position = { 0,0 };
	Vec2 velocity = { 0,0 };
	float angle = 0;

	CTransform(const Vec2& p, const Vec2& v, float a) :position(p), velocity(v), angle(a) {}
};


class CShape {

public:

	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color f_color, const sf::Color o_color, float thickness) : circle(radius, points) {

		circle.setFillColor(f_color);
		circle.setOutlineColor(o_color);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);

	}

};


class CCollision {
public:
	float radius;
	CCollision(float r) :radius(r) {}
};


class CScore {
public:
	int score;
	CScore(int s) :score(s) {}
};


class CLifespan {
public:
	int remaining = 0;
	int total = 0;

	CLifespan(int r, int t) :remaining(r), total(t) {}
};

class CInput {
public:
	bool up = false;
	bool left = false;
	bool down = false;
	bool right = false;
	bool shoot = false;

	CInput() {}
};
