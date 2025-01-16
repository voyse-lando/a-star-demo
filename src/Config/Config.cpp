#include "Config/Config.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

toml::Value toml::Section::property(const std::string &name) {
	if (properties.find(name) == properties.end()) return std::nullopt;
	return properties[name];
}
std::optional<toml::Section> toml::Section::section(const std::string &name) {
	if (sections.find(name) == sections.end()) return std::nullopt;
	return sections[name];
}

inline void left_trim(std::string &str) {
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](char c) {
		return !std::isspace(c);
	}));
}
inline void right_trim(std::string &str) {
	str.erase(std::find_if(str.rbegin(), str.rend(), [](char c) {
		return !std::isspace(c);
	}).base(), str.end());
}

toml::Section toml::parse_file(const std::string &fileName) {
	Section global;

	std::ifstream ifs(fileName);
	if (!ifs.good()) return global;

	Section *currentSection = &global;
	for (std::string line; std::getline(ifs, line); ) {
		left_trim(line);
		auto b = line.begin();


		if (*b == '[') {
			auto leftBracket = std::find(b+1, line.end(), ']');
			if (leftBracket == line.end()) {
				std::cerr << "Error in config parsing\n";
				return global;
			}

			std::string sectionName = line.substr(1, leftBracket-b-1);
			left_trim(sectionName);
			right_trim(sectionName);

			global.sections[sectionName] = Section();
			currentSection = &global.sections[sectionName];
		}
		else if (std::isalpha(*b)) {
			auto equalSign = std::find(b, line.end(), '=');
			if (equalSign == line.end()) {
				std::cerr << "Error in config parsing\n";
				return global;
			}

			std::string propName = line.substr(0, equalSign-b-1);
			left_trim(propName);
			right_trim(propName);

			std::string value = line.substr(equalSign-b+1, line.end()-b);
			left_trim(value);
			right_trim(value);

			currentSection->properties[propName] = value;
		}
	}

	return global;
}

template<>
std::string ValueParser::parse(std::optional<std::string> value, const std::string &defaultValue) {
	return value.has_value() ? value.value() : defaultValue;
}
template<>
char ValueParser::parse(std::optional<std::string> value, const char &defaultValue) {
	return value.has_value() ? value.value()[0] : defaultValue;
}
template<>
u32 ValueParser::parse(std::optional<std::string> value, const u32 &defaultValue) {
	return value.has_value() ? std::stoul(value.value()) : defaultValue;
}
template<>
i32 ValueParser::parse(std::optional<std::string> value, const i32 &defaultValue) {
	return value.has_value() ? std::stoi(value.value()) : defaultValue;
}
template<>
float ValueParser::parse(std::optional<std::string> value, const float &defaultValue) {
	return value.has_value() ? std::stof(value.value()) : defaultValue;
}


Config Config::from_toml(std::string_view filePath) {
	static ValueParser valueParser;
	auto parsed = toml::parse_file(filePath.begin());

#define SET_PROPERTY(PROP, TYPE) { \
auto section = parsed.section(PROP.section.begin()); \
auto prop = section.has_value() ? section->property(PROP.name.begin()) : std::nullopt; \
PROP.value = valueParser.parse<TYPE>(prop, PROP.value); }

	Config config;
	SET_PROPERTY(config.screenWidth, u32);
	SET_PROPERTY(config.screenHeight, u32);
	SET_PROPERTY(config.mapWidth, u32);
	SET_PROPERTY(config.mapHeight, u32);
	SET_PROPERTY(config.mapDefinitionsFilePath, std::string);

	SET_PROPERTY(config.playerX, i32);
	SET_PROPERTY(config.playerY, i32);

	SET_PROPERTY(config.symWall, char);
	SET_PROPERTY(config.symPath, char);
	SET_PROPERTY(config.symRoute, char);
	return config;

#undef SET_PROPERTY
}