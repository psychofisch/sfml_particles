#include "particlesystem.h"

ParticleSystem::ParticleSystem(size_t numOfParticles)
	:m_vertices(sf::Points, numOfParticles),
	m_force(0, 0, false, 0.f),
	m_particleSpeed(1.f),
	m_numberOfParticles(numOfParticles),
	m_maxSpeed(500.f)
{
	m_position = new vec2[numOfParticles];
	m_velocity = new vec2[numOfParticles]{ vec2(0, 0) };
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_position;
	delete[] m_velocity;
}

void ParticleSystem::setForcePosition(vec2 p)
{
	m_force = p;
}

void ParticleSystem::drawWithShape(sf::RenderWindow* window, sf::CircleShape & shape)
{
	for (size_t i = 0; i < m_numberOfParticles; ++i)
	{
		shape.setPosition(m_position[i]);
		window->draw(shape);
	}
}

void ParticleSystem::setRandomPositions(RNGesus* rng, const vec2& limits)
{
	for (size_t i = 0; i < m_numberOfParticles; ++i)
	{
		m_position[i] = vec2(rng->GetZeroToOne() * limits.x, rng->GetZeroToOne() * limits.y);
		m_velocity[i] = vec2(rng->GetZeroToOne() * 10.f - 5.f, rng->GetZeroToOne() * 10.f - 5.f);

		m_vertices[i] = m_position[i];
	}
}

void ParticleSystem::update(float dt)
{
	float a = .2f, b = .6f, c = .2f;

#pragma omp parallel for
	for (int i = 0; i < m_numberOfParticles; ++i)
	{
		sf::Vector2f newVel;

		//m_rng.seed(i * 815, 1337, 420);

		//std::cout << vectorMath::angleD(vectorMath::normalize(newVel) - vectorMath::normalize(m_velocity[i])) << std::endl;
		//std::cout << vectorMath::radToDeg(atan2f(newVel.y - m_velocity[i].y, newVel.x - m_velocity[i].x)) << std::endl;
		//m_velocity[i] = vectorMath::normalize(newVel) * m_maxSpeed;

		m_position[i] += m_velocity[i] * dt;

		m_vertices[i].position = m_position[i];
	}
}

void ParticleSystem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// our particles don't use a texture
	states.texture = NULL;

	// draw the vertex array
	target.draw(m_vertices, states);
}

Force::Force(float x, float y, bool active, float strength)
{
	this->x = x;
	this->y = y;
	this->isActive = active;
	this->strength = strength;
}

Force & Force::operator=(const vec2 & v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}
