#pragma once
#include <string>
#include <functional>
#include <queue>
#include "Game.h"

namespace eng {

struct ActorMoveInfo {
	eng::Actor& child, & parent;
	bool keepWorldTransform;
};

class Engine {
public: //---------------|Constructor/destructor|---------------------------

	explicit Engine(const std::string& dataPath);
	~Engine();

	Engine(const Engine& other) = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;

public: //---------------|Run method|---------------------------

	void Run(std::function<u_ptr<Game>()> loadGame);


};

}