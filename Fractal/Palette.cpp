#include "Palette.h"

void Palette::initialize()
{
	p =
	{
		sf::Color::Black,
		sf::Color::Red,
		sf::Color::Yellow,
		sf::Color::Green,
		sf::Color::Cyan,
		sf::Color::Blue,
		sf::Color::Magenta
	};
}

Palette::Palette()
{
	initialize();
}

Palette::Palette(const char* filename)
{
	std::ifstream fin(filename);
	if (fin.is_open()) {
		while (!fin.eof()) {
			int r, g, b;
			fin >> r >> g >> b;
			p.emplace_back(r, g, b);
		}
		fin.close();
	}
	else
		initialize();
}
sf::Color Palette::getColor(const size_t steps, const size_t maxSteps, bool reachedLimit)
{
	if (reachedLimit) {
		int index = steps * (p.size() - 1) / maxSteps;
		return p[p.size() - index - 1];
	}
	else {
		return p.front();
	}
}
