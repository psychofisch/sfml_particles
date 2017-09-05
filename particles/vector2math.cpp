#pragma once
#include "vector2math.h"

sf::Vector2f vectorMath::normalize(const sf::Vector2f& source)
{
	float length = magnitude(source);
	float inverseL = 1.f / length;
	if (length > 0.f && inverseL != 0.f)
		return sf::Vector2f(source.x * inverseL, source.y * inverseL);
	else
		return source;
}

float vectorMath::magnitude(const sf::Vector2f& source) 
{
	if (source.x == 0.f && source.y == 0.f)
		return 0.f;
	return sqrtf((source.x * source.x) + (source.y * source.y));
}

float vectorMath::max(const sf::Vector2f & vec)
{
	if (vec.x > vec.y)
		return vec.x;
	else
		return vec.y;
}

sf::Vector2f vectorMath::multiply(const sf::Vector2f & left, const sf::Vector2f & right)
{
	return sf::Vector2f(left.x*right.x, left.y*right.y);
}

float vectorMath::dot(const sf::Vector2f & left, const sf::Vector2f & right)
{
	return (left.x*right.x) + (left.y*right.y);
}

float vectorMath::angleD(const sf::Vector2f & v)
{
	return radToDeg(angleR(v));
}

float vectorMath::angleR(const sf::Vector2f & v)
{
	return atan2f(v.y, v.x);
}

float vectorMath::radToDeg(float Rad)
{
	return Rad * PI_INVERSE * 180.f;
}

float vectorMath::degToRad(float d)
{
	return (d * INVERSE_180) * sf::PI;
}

sf::Vector2f vectorMath::rotateD(sf::Vector2f v, float angle)
{
	angle = degToRad(angle);
	float length = magnitude(v);
	v = normalize(v);
	float x = (v.x * cosf(angle)) - (v.y * sinf(angle));
	float y = (v.x * sinf(angle)) + (v.y * cosf(angle));
	return sf::Vector2f(x, y) * length;
}

void vectorMath::getRealCorners(const sf::Sprite & sprite, sf::Vector2f* out)
{
	sf::FloatRect srect = sprite.getLocalBounds();

	out[0] = sprite.getTransform().transformPoint(srect.left, srect.top);
	out[1] = sprite.getTransform().transformPoint(srect.width, srect.top);
	out[2] = sprite.getTransform().transformPoint(srect.width, srect.height);
	out[3] = sprite.getTransform().transformPoint(srect.left, srect.height);
}

sf::Vector2f vectorMath::lineIntersection(sf::Vector2fLines& l1, sf::Vector2fLines& l2)
{
	//todo: http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	sf::Vector2f intersection;

	intersection.x = ((((l1.p1.x*l1.p2.y) - (l1.p1.y*l1.p2.x))*(l2.p1.x - l2.p2.x)) - ((l1.p1.x - l1.p2.x)*((l2.p1.x*l2.p2.y) - (l2.p1.y*l2.p2.x))))
						/ (((l1.p1.x - l1.p2.x)*(l2.p1.y - l2.p2.y)) - ((l1.p1.y - l1.p2.y)*(l2.p1.x - l2.p2.x)));

	intersection.y = ((((l1.p1.x*l1.p2.y) - (l1.p1.y*l1.p2.x))*(l2.p1.y - l2.p2.y)) - ((l1.p1.y - l1.p2.y)*((l2.p1.x*l2.p2.y) - (l2.p1.y*l2.p2.x))))
						/ (((l1.p1.x - l1.p2.x)*(l2.p1.y - l2.p2.y)) - ((l1.p1.y - l1.p2.y)*(l2.p1.x - l2.p2.x)));

	return intersection;
}

float vectorMath::cross2D(sf::Vector2f a, sf::Vector2f b)
{
	return (a.x*b.y)-(a.y*b.x);
}

bool vectorMath::isBetween(sf::Vector2f a, sf::Vector2f p, sf::Vector2f b)
{
	if (magnitude(p - a) < magnitude(b - a))
		return true;
	else
		return false;
}

bool vectorMath::isBetween(float a, float p, float b)
{
	if (p > a && p < b)
		return true;
	else
		return false;
}

bool vectorMath::isNotZero(sf::Vector2f & v)
{
	if (v.x == 0.0f && v.y == 0.0f)
		return false;
	else
		return true;
}

sf::FloatRect vectorMath::growBox(sf::FloatRect & r, float f)
{
	return sf::FloatRect(r.left - f, r.top - f, r.width + (f*2.f), r.height + (f*2.f));
}

float vectorMath::circumradius(sf::Vector2f points[3])
{
	float a = magnitude(points[0] - points[1]);
	float b = magnitude(points[0] - points[2]);
	float c = magnitude(points[1] - points[2]);
	float area = triangleArea(points);
	return (a*b*c) / (4.0f * area);
}

sf::Vector2f vectorMath::circumcircleCoords(sf::Vector2f points[3])
{
	float d = 2.0f * (points[0].x*(points[1].y - points[2].y) + points[1].x*(points[2].y - points[0].y) + points[2].x*(points[0].y - points[1].y));
	float x = ((pow2(points[0].x)+ pow2(points[0].y))*(points[1].y-points[2].y) + 
		(pow2(points[1].x) + pow2(points[1].y))*(points[2].y - points[0].y) + 
		(pow2(points[2].x) + pow2(points[2].y))*(points[0].y - points[1].y)) / d;
	
	float y = ((pow2(points[0].x) + pow2(points[0].y))*(points[1].x - points[2].x) +
		(pow2(points[1].x) + pow2(points[1].y))*(points[2].x - points[0].x) +
		(pow2(points[2].x) + pow2(points[2].y))*(points[0].x - points[1].x)) / d;

	return sf::Vector2f(x, -y);//"-y" because reasons...
}

float vectorMath::triangleArea(sf::Vector2f points[3])
{
	float a = magnitude(points[0] - points[1]);
	float b = magnitude(points[0] - points[2]);
	float c = magnitude(points[1] - points[2]);
	float s = (a + b + c) / 2.0f;
	return sqrtf(s*(s - a)*(s - b)*(s - c));
}

float vectorMath::pow2(float a)
{
	return a*a;
}

float vectorMath::sign(sf::Vector2f point, sf::Vector2f p1, sf::Vector2f p2)
{
	//return (point.x - p2.x)*(p1.y - p2.y) - (p1.x - p2.x)*(point.y - p2.y);
	return (p2.x - p1.x)*(point.y - p1.y) - (point.x - p1.x)*(p2.y - p1.y);
}

bool vectorMath::pointInTriangle(sf::Vector2f point, sf::Vector2f t1, sf::Vector2f t2, sf::Vector2f t3)
{
	bool b1, b2, b3;

	b1 = sign(point, t1, t2) < 0.0f;
	b2 = sign(point, t2, t3) < 0.0f;
	b3 = sign(point, t3, t1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

sf::Vector2f vectorMath::projectPointOnLine(sf::Vector2f l1, sf::Vector2f l2, sf::Vector2f p)
{
	float m = (l2.y - l1.y) / (l2.x - l1.x);
	float b = l1.y - (m * l1.x);

	float tmp = 1 / (m * m + 1);
	float x = (m * p.y + p.x - m * b) / tmp;
	float y = (m * m * p.y + m * p.x + b) / tmp;

	return sf::Vector2f(x, y);
}
