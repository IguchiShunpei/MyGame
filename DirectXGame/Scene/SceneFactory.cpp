#include "SceneFactory.h"

GameBaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // éüÇÃÉVÅ[Éìê∂ê¨
    GameBaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new GameTitleScene();
    }
    else if (sceneName == "GAMEPLAY") {
        newScene = new GamePlayScene();
    }
    else if (sceneName == "CLEAR") {
        newScene = new GameClearScene();
    }
    else if (sceneName == "GAMEOVER") {
        newScene = new GameOverScene();
    }

    return newScene;
}
