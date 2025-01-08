#pragma once
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
class Palette
{
private:
	std::vector<sf::Color> p;
	void initialize();
public:
	Palette();
	Palette(const char* filename);
	sf::Color getColor(const size_t steps, const size_t maxSteps, bool reachedLimit);
};

