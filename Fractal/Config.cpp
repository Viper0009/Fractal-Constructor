#include "Config.h"

static void ignore(std::istream& is, const char delim) {
	is.ignore(std::numeric_limits<std::streamsize>::max(), delim);
}
static char ignore(std::istream& is, const std::string& delim) {
	char c = is.get();
	while (delim.find(c) == -1) { c = is.get(); }
	return c;
}
Config::Config()
{
	std::unordered_map<std::string, std::string> data;
	std::string key, value;
	std::ifstream fin("Config.txt");
	if (fin.is_open()) {
		while (!fin.eof()) {
			std::getline(fin, key, ':');
			std::getline(fin, value, ';');
			ignore(fin, '\n');
			std::erase(key, ' ');
			std::erase(value, ' ');
			if (!key.empty())
				data[key] = value;
		}
		fin.close();

		if (data.find("scale") != data.end()) {
			scale = std::stoi(data["scale"]);
		}
		if (data.find("pattern") != data.end()) {
			pattern = data["pattern"];
		}
		if (data.find("palette") != data.end()) {
			palette = data["palette"];
			std::erase(palette, '\"');
		}
		if (data.find("framerate-limit") != data.end()) {
			framerateLimit = std::stoi(data["framerate-limit"]);
		}
		if (data.find("screen-size") != data.end()) {
			screenSize = data["screen-size"];
		}
	}
}

unsigned int Config::getFramerateLimit()
{
	return framerateLimit;
}

Palette Config::getPalette()
{
	if (palette.find(".txt") != -1) {
		return Palette(palette.c_str());
	}
	else
		return Palette();
}

int Config::getScale()
{
	return scale;
}
std::unique_ptr<FractalPattern> Config::getPattern(const Complex<double>& limit)
{
	std::unique_ptr<FractalPattern> def = std::make_unique<JuliaSet>(limit, 15, 0, 2);
	std::string name;
	int steps;
	std::stringstream ss(pattern);
	std::getline(ss, name, '(');
	ss >> steps;
	if (name == "JuliaSet") {
		Complex<double> a, b;
		size_t power = 2;
		ignore(ss, ',');
		ss >> a;
		ignore(ss, ',');
		ss >> b;
		char stop = ignore(ss, ",)");
		if (stop == ',') {
			ss >> power;
			ignore(ss, ')');
		}
		return std::make_unique<JuliaSet>(limit, steps, a, b, power);
	}
	else if (name == "MandelbrotSet") {
		size_t power = 2;
		char stop = ignore(ss, ",)");
		if (stop == ',') {
			ss >> power;
			ignore(ss, ')');
		}
		return std::make_unique<MandelbrotSet>(limit, steps, power);
	}
	return def;

}

sf::Vector2u Config::getScreenSize()
{
	if (screenSize == "default" || screenSize == "none") {
		return {
		sf::VideoMode::getFullscreenModes()[0].width / 2,
		sf::VideoMode::getFullscreenModes()[0].height / 2
		};
	}
	else if (screenSize == "full" || screenSize == "fullscreen") {
		style = sf::Style::Fullscreen;
		return {
			sf::VideoMode::getFullscreenModes()[0].width,
			sf::VideoMode::getFullscreenModes()[0].height
		};
	}
	else {
		std::stringstream ss(screenSize);
		ignore(ss, '(');
		sf::Vector2u ret;
		ss >> ret.x;
		ignore(ss, ',');
		ss >> ret.y;
		ignore(ss, ')');
		if (ss.fail()) {
			screenSize = "default";
			return getScreenSize();
		}
		return ret;
	}
}

int Config::getStyle()
{
	return style;
}
