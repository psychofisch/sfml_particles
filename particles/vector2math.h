#pragma once

#include <SFML\Graphics.hpp>

#define PI_INVERSE 0.318309886183790671537767526745028724f
#define INVERSE_180 0.0055555555555555555555f

namespace sf {
	const float PI = 3.1415f;

	struct Vector2fLines {
		sf::Vector2f p1;
		sf::Vector2f p2;

		Vector2fLines(sf::Vector2f a, sf::Vector2f b)
			:p1(a),
			p2(b)
		{}

		Vector2fLines()
			:Vector2fLines(sf::Vector2f(), sf::Vector2f())
		{}
	};
}

namespace vectorMath {
	sf::Vector2f normalize(const sf::Vector2f&);
	float magnitude(const sf::Vector2f&);
	float max(const sf::Vector2f& vec);
	sf::Vector2f multiply(const sf::Vector2f& left, const sf::Vector2f& right);
	float dot(const sf::Vector2f& left, const sf::Vector2f& right);
	float angleD(const sf::Vector2f& v);
	float angleR(const sf::Vector2f& v);
	float radToDeg(float r);
	float degToRad(float d);
	sf::Vector2f rotateD(sf::Vector2f v, float aD);
	void getRealCorners(const sf::Sprite& sprite, sf::Vector2f* out);//out -> sf::Vector2f[4]
	sf::Vector2f lineIntersection(sf::Vector2fLines& l1, sf::Vector2fLines& l2);
	float cross2D(sf::Vector2f a, sf::Vector2f b);
	bool isBetween(sf::Vector2f a, sf::Vector2f p, sf::Vector2f b);
	bool isBetween(float a, float p, float b);
	bool isNotZero(sf::Vector2f& v);
	sf::FloatRect growBox(sf::FloatRect& r, float f);
	float circumradius(sf::Vector2f points[3]);
	sf::Vector2f circumcircleCoords(sf::Vector2f points[3]);
	float triangleArea(sf::Vector2f points[3]);
	float pow2(float a);
	float sign(sf::Vector2f point, sf::Vector2f p1, sf::Vector2f p2);
	bool pointInTriangle(sf::Vector2f point, sf::Vector2f t1, sf::Vector2f t2, sf::Vector2f t3);
	sf::Vector2f projectPointOnLine(sf::Vector2f l1, sf::Vector2f l2, sf::Vector2f p);
}
