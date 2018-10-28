#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "proj.win32/Mole.h"
#include <vector>
#include "GAFWrapper.h"
#include "proj.win32/Question.h"
#include <queue>
#include "proj.win32/DataParser.h"

enum class GameState
{
	Ready,
	Playing
};

class HelloWorld : public cocos2d::ui::Layout
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

protected:
	void onEnter() override;
	std::vector<Mole*> moles;
	std::queue<int> upMoles;
	int gameLevel;
	GameState gameState;
	void triggerReady();
	void triggerPlaying();
	void getData();
	void switchMole();
	void Update(float dt);
	void nextRound();
	void resetGame();

private:
	cocos2d::Label* text;
	Node* rootNode;
	Question* question;
	int currentLevel;
	DataParser* data;
	int score;
};

#endif // __HELLOWORLD_SCENE_H__
