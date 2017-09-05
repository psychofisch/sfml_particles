#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <omp.h>
#include <fstream>

#include <SFML\Graphics.hpp>

#include "RNGesus.h"
#include "particlesystem.h"

class planeWorld
{
public:

	planeWorld();
	~planeWorld();

	void run();
	void setRenderWindow(sf::RenderWindow* wndw);
	bool setWorldDimensions(int size); //true = ERROR; false = successful
	bool setWorldDimensions(int size_x, int size_y);
	void updateSwarm();
	void setNumberOfThreads(unsigned int t);
	void setDebugMode(bool d);

private:
	sf::Vector2u m_dimension;
	sf::RenderWindow* m_window;
	sf::View m_view;
	bool m_vsync;
	RNGesus* m_rng;
	int m_numberOfThreads;
	bool m_debug;
};
