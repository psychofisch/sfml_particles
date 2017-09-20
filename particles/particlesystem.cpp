#include "particlesystem.h"

ParticleSystem::ParticleSystem(size_t numOfParticles, vec2u dimension)
	:m_vertices(sf::Points, numOfParticles),
	m_force(0, 0, false, 1.f),
	m_particleSpeed(1.f),
	m_numberOfParticles(numOfParticles),
	m_maxSpeed(500.f),
	m_colorChange(true),
	m_dimension(dimension)
{
	m_position = new vec2f[numOfParticles];
	m_velocity = new vec2f[numOfParticles]{ vec2f(0, 0) };
	m_randoms = new float[numOfParticles];

	for (int i = 0; i < numOfParticles; ++i)
	{
		m_randoms[i] = m_rng.GetZeroToOne();
	}
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_position;
	delete[] m_velocity;
	delete[] m_randoms;
}

void ParticleSystem::setForcePosition(vec2f p)
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

void ParticleSystem::setRandomPositions(RNGesus* rng)
{
	for (size_t i = 0; i < m_numberOfParticles; ++i)
	{
		m_position[i] = vec2f(rng->GetZeroToOne() * m_dimension.x, rng->GetZeroToOne() * m_dimension.y);
		m_velocity[i] = vec2f();
		//m_velocity[i] = vec2(rng->GetZeroToOne() * 10.f - 5.f, rng->GetZeroToOne() * 10.f - 5.f);

		m_vertices[i] = m_position[i];
	}
}

void ParticleSystem::update(float dt)
{
	float maxSpeed = 500.f;
	const sf::Color colorA = sf::Color::Yellow;
	const sf::Color colorB = sf::Color::Red;

	//optimization
	maxSpeed = 1.f / maxSpeed;
	float forceStrength = 1.f / (m_force.strength * 10000.f);

#pragma omp parallel for
	for (int i = 0; i < m_numberOfParticles; ++i)
	{
		vec2f particlePosition = m_vertices[i].position;
		vec2f particleVelocity = m_velocity[i];
		float r = m_randoms[i];
		//m_rng.seed(i * 815, 1337, 420);

		//std::cout << vectorMath::angleD(vectorMath::normalize(newVel) - vectorMath::normalize(m_velocity[i])) << std::endl;
		//std::cout << vectorMath::radToDeg(atan2f(newVel.y - m_velocity[i].y, newVel.x - m_velocity[i].x)) << std::endl;
		
		//mouse force
		if (m_force.isActive)
		{
			float forceFactor = vectorMath::magnitude(particlePosition - m_force) * forceStrength;
			forceFactor *= forceFactor;
			forceFactor = 1.f - forceFactor;
			forceFactor = std::max(0.f, forceFactor);
			//m_velocity[i] += forceFactor * vectorMath::normalize(m_force - m_vertices[i].position) * dt * maxSpeed * m_force.strength;
			particleVelocity += forceFactor * vectorMath::normalize(m_force - particlePosition) * dt * m_force.strength;
		}
		//*** mf

		//static collision
		if ((particlePosition.x > m_dimension.x && particleVelocity.x > 0.f) || (particlePosition.x < 0.f && particleVelocity.x < 0.f))
			particleVelocity.x *= -(.5f + 0.2f * r);

		if ((particlePosition.y > m_dimension.x && particleVelocity.y > 0.f) || (particlePosition.y < 0.f && particleVelocity.y < 0.f))
			particleVelocity.y *= -(.5f + 0.2f * r);
		//*** sc

		//gravity
		if(particlePosition.y < m_dimension.y)
			particleVelocity.y += 9.81f * dt;
		//***g

		particleVelocity -= dt * particleVelocity * 0.25f;//damping
		particlePosition += particleVelocity /** dt*/;

		m_vertices[i].position = particlePosition;
		m_velocity[i] = particleVelocity;

		if (m_colorChange)
		{
			float colorFac = std::max(0.f, 1.0f - vectorMath::magnitude(particleVelocity) * maxSpeed);
			m_vertices[i].color = (colorA * colorFac);
			m_vertices[i].color += (colorB * (1.0f - colorFac));
		}
		else
			m_vertices[i].color = sf::Color::Green;
	}
}

void ParticleSystem::setForceActive(bool b)
{
	m_force.isActive = b;
}

void ParticleSystem::setForceStrength(float s)
{
	m_force.strength = s;
}

void ParticleSystem::activateColor(bool b)
{
	m_colorChange = b;
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

Force & Force::operator=(const vec2f & v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}

sf::Color operator*(const sf::Color & left, const float f)
{
	return sf::Color(left.r * f, left.g * f, left.b * f, left.a * f);
}
