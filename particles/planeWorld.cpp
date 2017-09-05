#include "planeWorld.h"



planeWorld::planeWorld()
	:m_debug(true),
	m_vsync(true),
	m_rng(new RNGesus(1, 2, 3))
{
}

planeWorld::~planeWorld()
{
}

void planeWorld::run()
{
	int mouseMoveRectSize = 200;
	sf::IntRect mouseMoveRect = sf::IntRect((m_window->getSize().x - mouseMoveRectSize) / 2, (m_window->getSize().x - mouseMoveRectSize) / 2, mouseMoveRectSize, mouseMoveRectSize);
	sf::Vector2f windowCenter(m_window->getSize());
	windowCenter /= 2.0f;

	sf::Clock time;
	float dt = 0.16f;

	//DEBUG
	sf::Font debug_font;
	debug_font.loadFromFile("./NotoSans-Regular.ttf");
	sf::Text debug_text;
	debug_text.setFont(debug_font);
	debug_text.setCharacterSize(24);
	//debug_text.setScale(sf::Vector2f(1.f, 1.f));
	debug_text.setOutlineColor(sf::Color::Black);
	debug_text.setOutlineThickness(1.f);
	debug_text.setPosition(20.f, 20.f);
	bool measure = true,
		pause = false;

	std::stringstream debugString;
	//***d

	int numberOfParticles = 100;
	ParticleSystem* particleSwarm = new ParticleSystem(numberOfParticles);
	particleSwarm->setRandomPositions(m_rng, sf::Vector2f(m_dimension.x, m_dimension.y));

	sf::CircleShape particle;
	particle.setRadius(5.f);
	particle.setFillColor(sf::Color::Green);

	unsigned int fps = 0, fpsCount = 0;
	float fpsTimer = 0.f;

	bool quit = false;
	while (!quit)
	{
		time.restart();

		sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
		sf::Vector2f mousePos_mapped = m_window->mapPixelToCoords(mousePos, m_view);

		//controls
		sf::Event eve;
		while (m_window->pollEvent(eve))
		{
			if (eve.type == sf::Event::Closed)
			{
				quit = true;
				break;
			}
			else if (eve.type == sf::Event::MouseButtonPressed && eve.mouseButton.button == sf::Mouse::Left)
			{
				break;
			}
			else if (eve.type == sf::Event::MouseButtonPressed && eve.mouseButton.button == sf::Mouse::Right)
			{
				break;
			}
			else if (eve.type == sf::Event::MouseWheelScrolled)
			{
				if (eve.mouseWheelScroll.delta < 0)
				{
					m_view.zoom(1.1f);
				}
				else if (eve.mouseWheelScroll.delta > 0)
				{
					m_view.zoom(0.9f);
				}
				break;
			}
			else if (eve.type == sf::Event::KeyPressed)
			{
				switch (eve.key.code)
				{
				case sf::Keyboard::H: std::cout << "no one can help you :)" << std::endl;
					break;
				case sf::Keyboard::R:
					particleSwarm->setRandomPositions(m_rng, sf::Vector2f(500.f, 500.f));
					break;
				case sf::Keyboard::M:
					break;
				case sf::Keyboard::V:
					if (m_vsync)
						//m_window->setFramerateLimit(0);
						m_window->setVerticalSyncEnabled(false);
					else
						//m_window->setFramerateLimit(60);
						m_window->setVerticalSyncEnabled(true);
					m_vsync = !m_vsync;
					break;
				case sf::Keyboard::N:
					break;
				case sf::Keyboard::P:
					pause = !pause;
					break;
				case sf::Keyboard::Equal:
					break;
				case sf::Keyboard::Dash:
					break;
				case sf::Keyboard::Q:
					numberOfParticles /= 10;
					delete particleSwarm;
					particleSwarm = new ParticleSystem(numberOfParticles);
					particleSwarm->setRandomPositions(m_rng, sf::Vector2f(m_dimension.x, m_dimension.y));
					break;
				case sf::Keyboard::E:
					numberOfParticles *= 10;
					delete particleSwarm;
					particleSwarm = new ParticleSystem(numberOfParticles);
					particleSwarm->setRandomPositions(m_rng, sf::Vector2f(m_dimension.x, m_dimension.y));
					break;
				case sf::Keyboard::U:
					break;
				case sf::Keyboard::T:
					break;
				case sf::Keyboard::Escape:
					quit = true;
					break;
				}
			}
			else if (eve.type == sf::Event::Resized)
			{
				m_view = sf::View(sf::FloatRect(0, 0, static_cast<float>(eve.size.width), static_cast<float>(eve.size.height)));
				//sf::Vector2f view_center((m_grid[0].getPosition() + m_grid[m_grid.size() - 1].getPosition()) / 2.0f);
				m_view.setCenter(sf::Vector2f(0, 0));
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && !mouseMoveRect.contains(mousePos))
		{
			m_view.move((sf::Vector2f(mousePos) - windowCenter)*dt);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			m_view.move(sf::Vector2f(0.f, -1.f)*50.0f*dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			m_view.move(sf::Vector2f(-1.f, 0.f)*50.0f*dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			m_view.move(sf::Vector2f(0.f, 1.f)*50.0f*dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			m_view.move(sf::Vector2f(1.f, 0.f)*50.0f*dt);

		//*** controls

		//updates
		particleSwarm->setForcePosition(mousePos_mapped);
		particleSwarm->update(dt);

		++fpsCount;

		if (fpsTimer < .5f)
			fpsTimer += dt;
		else
		{
			fps = fpsCount * 2;
			fpsCount = 0;
			fpsTimer = 0.f;
		}

		//render
		m_window->clear(sf::Color(69, 69, 69));

		m_window->setView(m_view);

		m_window->draw(*particleSwarm);
		//particleSwarm->drawWithShape(m_window, particle);

		// debug text
		m_window->setView(m_window->getDefaultView());

		debugString.str(std::string());//to clean string
		//int fps = int(1.f / dt);
		debugString << fps << std::endl;
		debugString << static_cast<int>(mousePos_mapped.x) << ":" << static_cast<int>(mousePos_mapped.y) << std::endl;
		debugString << "VSYNC O" << ((m_vsync) ? "N" : "FF") << std::endl;
		debugString << "Particles: " << numberOfParticles << std::endl;
		debug_text.setString(debugString.str());

		m_window->draw(debug_text);
		//*** dt
		//m_window->draw(m_currentTile);

		m_window->display();
		//*** render

		dt = time.getElapsedTime().asSeconds();
	}
}

void planeWorld::setRenderWindow(sf::RenderWindow * wndw)
{
	m_window = wndw;
}

bool planeWorld::setWorldDimensions(int size)
{
	return setWorldDimensions(size, size);
}

bool planeWorld::setWorldDimensions(int size_x, int size_y)
{
	if(size_x < 0 || size_y < 0)
		return true;

	m_dimension = sf::Vector2u(size_x, size_y);

	return false;
}

void planeWorld::updateSwarm()
{
}

void planeWorld::setDebugMode(bool d)
{
	m_debug = d;
}
