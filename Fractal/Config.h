#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "Complex.h"
#include "Palette.h"
#include "FractalPattern.h"
class Config
{
private:
	unsigned int framerateLimit = 8;
	const std::string dPattern = "JuliaSet(15, (1,0), (0,0), 2)";
	std::string pattern = "JuliaSet(15, (1,0), (0,0), 2)";
	int scale = 150;
	std::string palette = "default";
	std::string screenSize = "default";
	int style = sf::Style::Default;
public:
	Config();

	unsigned int getFramerateLimit();
	Palette getPalette();
	int getScale();
	std::unique_ptr<FractalPattern> getPattern(const Complex<double>& limit);
	sf::Vector2u getScreenSize();
	int getStyle();
};

