#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Config.h"
using namespace sf;
void program(RenderWindow& window, Vector2i& screenSize, Engine& engine);
int main() {
	Config config;

	VideoMode fullscreen = VideoMode::getFullscreenModes()[0];
	VideoMode vm(fullscreen.width / 2, fullscreen.height / 2);
	//Vector2i screenSize = { int(vm.width), int(vm.height) };
	Vector2i screenSize = Vector2i(config.getScreenSize());
	RenderWindow window(VideoMode(screenSize.x, screenSize.y), L"Fractal Constructor", config.getStyle());
	window.setFramerateLimit(config.getFramerateLimit());
	window.setKeyRepeatEnabled(false);

	Complex<double> fractalLimit(screenSize.x / 2, screenSize.y / 2);
	std::unique_ptr<FractalPattern> fp = config.getPattern(fractalLimit);
	Palette palette = config.getPalette();
	Engine engine(Vector2u(screenSize.x, screenSize.y), config.getScale());
	engine.setPalette(palette);
	engine.setFractalPattern(*fp);
	
	program(window, screenSize, engine);
}
void program(RenderWindow& window, sf::Vector2i& screenSize, Engine& engine) {
	View view(FloatRect{ Vector2f(), (Vector2f)window.getSize() });
	Clock clock;
	Time elTime = Time::Zero;
	int fps = 0;
	engine.compute();
	while (window.isOpen()) {
		elTime += clock.getElapsedTime();
		clock.restart();
		if (elTime.asSeconds() >= 1) {
			window.setTitle("Fractal Constructor: " + std::to_string(fps) + " FPS");
			fps = 0;
			elTime = Time::Zero;
		}
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case Event::Closed: {
				window.close();
			} break;
			case Event::KeyPressed: {
				if(event.key.scancode == Keyboard::Scancode::S && Keyboard::isKeyPressed(Keyboard::Scancode::LControl))
					engine.saveImage();
			} break;
			}
			engine.handleEvent(event, screenSize, view);
		}
		window.clear();
		engine.update(screenSize, view);
		window.setView(view);
		engine.render(window);
		window.display();
		++fps;
	}
}