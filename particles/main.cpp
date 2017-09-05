#include <iostream>
#include <omp.h>

#include <SFML\Graphics.hpp>

#include "planeWorld.h"

#define DEBUG(x) std::cout << #x << ": " << x << std::endl;

void main(int argc, char* argv[])
{
	unsigned int threads = 1;
	bool debug = false;

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--debug") == 0)
		{
			debug = true;
			std::cout << "DEBUG MODE ACTIVATED" << std::endl;
		}
		else if (strcmp(argv[i], "--threads") == 0)
		{
			threads = atoi(argv[++i]);
			if (debug)
				DEBUG(threads);
		}
	}

	sf::VideoMode vm;
	vm.width = 720;
	vm.height = 720;
	vm.bitsPerPixel = 32;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;

	sf::RenderWindow window(vm, "Particles", sf::Style::Titlebar | sf::Style::Close | sf::Style::Default, settings);
	window.setFramerateLimit(144);
	//window.setVerticalSyncEnabled(true);

	planeWorld pw;

	pw.setRenderWindow(&window);
	pw.setWorldDimensions(1000, 1000);
	pw.setNumberOfThreads(threads);
	pw.setDebugMode(debug);
	pw.run();
}
