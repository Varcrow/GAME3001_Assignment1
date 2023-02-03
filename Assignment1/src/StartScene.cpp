#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::Start();
}

StartScene::~StartScene()
= default;

void StartScene::Draw()
{
	DrawDisplayList();
}

void StartScene::Update()
{
	UpdateDisplayList();
}

void StartScene::Clean()
{
	RemoveAllChildren();
}

void StartScene::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if(EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if(EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::PLAY);
	}
}

void StartScene::Start()
{
	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pAssignmentLabel = new Label("Assignment 1", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_pEthan = new Label("Ethan Minns: 10145424", "Consolas", 50, blue, glm::vec2(400.0f, 120.0f));
	m_pKarl = new Label("karl Bogomlov: 00000000", "Consolas", 50, blue, glm::vec2(400.0f, 170.0f));
	m_pAssignmentLabel->SetParent(this);
	m_pEthan->SetParent(this);
	m_pKarl->SetParent(this);
	AddChild(m_pAssignmentLabel);
	AddChild(m_pEthan);
	AddChild(m_pKarl);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->GetTransform()->position = glm::vec2(400.0f, 400.0f); 

	m_pStartButton->AddEventListener(Event::CLICK, [&]()-> void
	{
		m_pStartButton->SetActive(false);
		Game::Instance().ChangeSceneState(SceneState::PLAY);
	});
	
	m_pStartButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
		m_pStartButton->SetAlpha(128);
	});

	m_pStartButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
		m_pStartButton->SetAlpha(255);
	});
	AddChild(m_pStartButton);

	ImGuiWindowFrame::Instance().SetDefaultGuiFunction();
}

