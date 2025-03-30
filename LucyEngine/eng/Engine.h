#pragma once
#include <string>
#include <functional>
#include "Game.h"

namespace eng {

class Engine {
public: //---------------|Constructor/destructor|---------------------------

	explicit Engine(const std::string& dataPath);
	~Engine();

	Engine(const Engine& other) = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;

public: //---------------|Methods|---------------------------

	void Run(std::function<Game()> loadGame);
};

}