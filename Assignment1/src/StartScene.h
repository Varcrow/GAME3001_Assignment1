#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"

//testing******************
#include "Ship.h"
#include "Target.h"

class StartScene final : public Scene
{
public:
	StartScene();
	~StartScene() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
	
private:
	//testing*********************
	Ship* ship;
	Target* target;

	Label* m_pAssignmentLabel{};
	Label* m_pEthan{};
	Label* m_pKarl{};
	Button* m_pStartButton{};
};

#endif /* defined (__START_SCENE__) */