#pragma once
#ifndef CONFIG_CONFIG_HPP_
#define CONFIG_CONFIG_HPP_

#include "Common.hpp"
#include <exception>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace toml {
	
	typedef std::optional<std::string> Value;

	struct Section {
	private:
		std::unordered_map<std::string, Value> properties;
		std::unordered_map<std::string, Section> sections;

	public:
		Value property(const std::string &name);
		std::optional<Section> section(const std::string &name);

		friend Section parse_file(const std::string &fileName);
	};
	extern Section parse_file(const std::string &fileName);
};

class NotImplementedYet : public std::exception {
	std::string reason;
public:
	NotImplementedYet(std::string_view functionName) {
		reason = std::string("Error: ") + functionName.begin() + "is not implemented yet"; 
	}
	const char * what() const noexcept override {
		return reason.c_str();
	}
};

template<class T = std::string>
struct Property {
	using ValueType = T;

	std::string_view section, name;
	T value;

	Property(std::string_view section, std::string_view name, T defaultValue)
		: section(section), name(name), value(defaultValue)
	{}

	T &operator()() {
		return value;
	}
};

class ValueParser {
public:
	template<class T>
	T parse(std::optional<std::string> value, const T &defaultValue) {
		throw NotImplementedYet("ValueParser::parse for T");
	}
};


class Config {
private:

	Config() {};
public:
	Property<u32> screenWidth = {"screen", "width", 800};
	Property<u32> screenHeight = {"screen", "height", 800};
	Property<u32> mapWidth = {"map", "width", 1};
	Property<u32> mapHeight = {"map", "height", 1};
	Property<std::string> mapDefinitionsFilePath = {"map", "definitionsFilePath", ""};

	Property<i32> playerX = {"map.player", "x", 0};
	Property<i32> playerY = {"map.player", "y", 0};

	Property<char> symWall = {"map.console", "symWall", '5'};
	Property<char> symPath = {"map.console", "symPath", '0'};
	Property<char> symRoute = {"map.console", "symRoute", '.'};
	static Config from_toml(std::string_view filePath);

	template<class T>
	friend struct Property;
};



#endif // ! CONFIG_CONFIG_HPP_