#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Complex.h"
#include "FractalPattern.h"
#include "Palette.h"
#include "thread"
class Engine
{
private:
	sf::Vector2u sz;
	sf::Vector2i offset;
	int scale;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite result;
	FractalPattern* fp;
	sf::Vector2f start = { 0, 0 };
	Palette palette;
	bool zoomed = false;
	int imageIndex = 0;
	void partialCompute(const int from, const int until, const sf::Vector2i offset);
public:
	Engine();
	Engine(sf::Vector2u sz, int scale);
	Engine(sf::Vector2u sz, int scale, FractalPattern& fp);

	void setFractalPattern(FractalPattern& fp);
	void setScale(const int scale);
	int getScale() const;
	void setStartPoint(const sf::Vector2f& start);
	const sf::Vector2f& getStartPoint() const;
	void setPalette(const Palette& palette);
	const Palette& getPalette() const;
	
	void compute();
	void multithreadedCompute();
	void naiveCompute();
	void render(sf::RenderWindow& window);

	void handleEvent(sf::Event& event, const sf::Vector2i& screenSize, sf::View& view);
	void update(const sf::Vector2i& screenSize, sf::View& view);
	void saveImage();
};
