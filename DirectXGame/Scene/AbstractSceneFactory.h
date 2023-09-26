#pragma once

#include "GameBaseScene.h"
#include <string>

class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;

	virtual GameBaseScene* CreateScene(const std::string& sceneName) = 0;
};