#include "Engine.h"

void Engine::partialCompute(const int from, const int until, const sf::Vector2i offset)
{
	for (int i = from; i < until; ++i) {
		Complex<double> point;
		point.imag = (i - offset.y + start.y) / (double)scale;
		for (int j = 0; j < sz.x; ++j) {
			point.real = (j - offset.x + start.x) / (double)scale;

			auto res = fp->ComputePoint(point);
			bool reachedLimit = abs(res.first) >= abs(fp->getLimit());
			image.setPixel(j, i, palette.getColor(res.second, fp->getSteps(), reachedLimit));
		}
	}
}

Engine::Engine() : sz(), scale(1), fp(nullptr)
{
}

Engine::Engine(sf::Vector2u sz, int scale) : sz(sz), scale(scale), fp(nullptr)
{
	image.create(sz.x, sz.y);
	offset = { ((int)sz.x) / 2, ((int)sz.y) / 2 };
}

Engine::Engine(sf::Vector2u sz, int scale, FractalPattern& fp) : sz(sz), scale(scale),  fp(&fp)
{
	image.create(sz.x, sz.y);
	offset = { ((int)sz.x) / 2, ((int)sz.y) / 2 };
}

void Engine::setFractalPattern(FractalPattern& fp)
{
	this->fp = &fp;
}
static const double EPS = 0.1;
void Engine::compute()
{
	if (fp == nullptr)
		throw std::exception("Fractal does not have any pattern");
	multithreadedCompute();
}

void Engine::multithreadedCompute()
{
	const auto processorCount = std::thread::hardware_concurrency();
	if (processorCount == 0)
		throw std::exception("hardware_concurrency equals to 0");
	std::vector<std::thread> threads;
	int step = sz.y / processorCount;
	for (int i = 0; i < processorCount; ++i) {
		threads.push_back(std::thread(&Engine::partialCompute, this, i * step, (i + 1) * step, offset));
	}
	if (sz.y % processorCount)
		threads.push_back(std::thread(&Engine::partialCompute, this, processorCount * step, sz.y, offset));

	for (int i = 0; i < threads.size(); ++i) {
		threads[i].join();
	}
	texture.loadFromImage(image);
	result.setTexture(texture);
}

void Engine::naiveCompute()
{
	for (int i = 0; i < sz.y; ++i) {
		Complex<double> point;
		point.imag = (i - offset.y + start.y) / (double)scale;
		for (int j = 0; j < sz.x; ++j) {
			point.real = (j - offset.x + start.x) / (double)scale;

			auto res = fp->ComputePoint(point);
			bool reachedLimit = abs(res.first) >= abs(fp->getLimit());
			image.setPixel(j, i, palette.getColor(res.second, fp->getSteps(), reachedLimit));
		}
	}
	texture.loadFromImage(image);
	result.setTexture(texture);
}

void Engine::render(sf::RenderWindow& window)
{
	window.draw(result);
}
void Engine::handleEvent(sf::Event& event, const sf::Vector2i& screenSize, sf::View& view)
{
	sf::View defaultView(sf::FloatRect(sf::Vector2f(), (sf::Vector2f)screenSize));
	if (event.type == sf::Event::MouseWheelScrolled) {

		float delta = 1.1 * event.mouseWheelScroll.delta;
		if (event.mouseWheelScroll.delta > 0) {
			scale *= delta;
			start = { start.x * delta, start.y * delta };
			view.zoom(1 / delta);
		}
		else {
			scale /= abs(delta);
			start = { start.x / abs(delta), start.y / abs(delta) };
			view.zoom(abs(delta));
		}
		if (scale == 0) {
			scale = 1;
		}
		zoomed = true;
	}
	bool moving =
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up);

	if (event.type == sf::Event::KeyReleased && !moving || moving && zoomed) {
		if (defaultView.getTransform() != view.getTransform()) {
			view = defaultView;
			compute();
			zoomed = false;
		}
	}
	
}

void Engine::update(const sf::Vector2i& screenSize, sf::View& view)
{
	float step = 10 * screenSize.x / screenSize.y, viewStep = 10 * view.getSize().x / view.getSize().y;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
		step *= 8;
		viewStep *= 8;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
		start -= {step, 0};
		view.move({ -viewStep, 0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		start += {step, 0};
		view.move({ viewStep, 0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
		start -= {0, step};
		view.move({ 0, -viewStep });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
		start += {0, step};
		view.move({ 0, viewStep });
	}
}

void Engine::saveImage()
{
	if (imageIndex == 0)
		image.saveToFile("image.png");
	else
		image.saveToFile("image" + std::to_string(imageIndex) + ".png");
	++imageIndex;
}

void Engine::setScale(const int scale)
{
	this->scale = scale;
}

int Engine::getScale() const
{
	return scale;
}

void Engine::setStartPoint(const sf::Vector2f& start)
{
	this->start = start;
}

const sf::Vector2f& Engine::getStartPoint() const
{
	return start;
}

void Engine::setPalette(const Palette& palette)
{
	this->palette = palette;
}

const Palette& Engine::getPalette() const
{
	return palette;
}
