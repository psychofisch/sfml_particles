#pragma once

#include <iostream>

#include <SFML\Graphics.hpp>

#include "vector2math.h"
#include "RNGesus.h"

typedef sf::Vector2f vec2f;
typedef sf::Vector2u vec2u;

sf::Color operator*(const sf::Color& left, const float f);

class Force : public vec2f
{
public:
	Force(float x, float y, bool active, float strength);
	Force &operator=(const vec2f& v);

	bool isActive;
	float strength;
};

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
	ParticleSystem(size_t numOfParticles, vec2u dimension);
	~ParticleSystem();

	void setForcePosition(vec2f p);
	void drawWithShape(sf::RenderWindow* window, sf::CircleShape& shape);
	void setRandomPositions(RNGesus* rng);
	void update(float dt);
	void setForceActive(bool b);
	void setForceStrength(float s);
	void activateColor(bool b);
	vec2f calculatePressureVector(size_t index);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool m_colorChange;
	size_t m_numberOfParticles;
	float m_particleSpeed, m_maxSpeed, m_particleRadius;
	vec2u m_dimension;
	Force m_force;
	RNGesus m_rng;
	vec2f* m_position;
	vec2f* m_velocity;
	vec2f* m_pressure;
	float* m_randoms;
	sf::VertexArray m_vertices;
};

//class ParticleSystemSFML : public sf::Drawable, public sf::Transformable
//{
//public:
//
//	ParticleSystemSFML(unsigned int count) :
//		m_particles(count),
//		m_vertices(sf::Points, count),
//		m_lifetime(sf::seconds(3)),
//		m_emitter(0, 0)
//	{
//	}
//
//	void setEmitter(sf::Vector2f position)
//	{
//		m_emitter = position;
//	}
//
//	void update(sf::Time elapsed)
//	{
//		for (std::size_t i = 0; i < m_particles.size(); ++i)
//		{
//			// update the particle lifetime
//			Particle& p = m_particles[i];
//			p.lifetime -= elapsed;
//
//			// if the particle is dead, respawn it
//			if (p.lifetime <= sf::Time::Zero)
//				resetParticle(i);
//
//			// update the position of the corresponding vertex
//			m_vertices[i].position += p.velocity * elapsed.asSeconds();
//
//			// update the alpha (transparency) of the particle according to its lifetime
//			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
//			m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
//		}
//	}
//
//private:
//
//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
//	{
//		// apply the transform
//		states.transform *= getTransform();
//
//		// our particles don't use a texture
//		states.texture = NULL;
//
//		// draw the vertex array
//		target.draw(m_vertices, states);
//	}
//
//private:
//
//	struct Particle
//	{
//		sf::Vector2f velocity;
//		sf::Time lifetime;
//	};
//
//	void resetParticle(std::size_t index)
//	{
//		// give a random velocity and lifetime to the particle
//		float angle = (std::rand() % 360) * 3.14f / 180.f;
//		float speed = (std::rand() % 50) + 50.f;
//		m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
//		m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);
//
//		// reset the position of the corresponding vertex
//		m_vertices[index].position = m_emitter;
//	}
//
//	std::vector<Particle> m_particles;
//	sf::VertexArray m_vertices;
//	sf::Time m_lifetime;
//	sf::Vector2f m_emitter;
//};

