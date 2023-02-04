#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetKeyboardInput();
}

void randomizeTargetAndShip(Target* trgt, Ship* ship) {
	trgt->GetTransform()->position.x = rand() % 800;
	trgt->GetTransform()->position.y = rand() % 600;
	trgt->SetEnabled(true);

	ship->SetTargetPosition(trgt->GetTransform()->position);
}

void PlayScene::GetKeyboardInput()
{
	/* 1/2 to do stuff */
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_1))
	{
		m_pShip->SetMode(Ship::SEEKING);
		randomizeTargetAndShip(m_pTrgt, m_pShip);
	}
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_2))
	{
		m_pShip->SetMode(Ship::FLEEING);
		randomizeTargetAndShip(m_pTrgt, m_pShip);
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_3)) {
		m_pShip->SetMode(Ship::ARRIVING);
		randomizeTargetAndShip(m_pTrgt, m_pShip);
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_4)) {
		m_pShip->SetMode(Ship::AVOIDING);
		randomizeTargetAndShip(m_pTrgt, m_pShip);
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	/* MORE LABELS */
	Label *one = new Label("Press 1 to seek", "Consolas");
	one->GetTransform()->position = glm::vec2(120, 40);

	AddChild(one);

	Label* two = new Label("Press 2 to flee", "Consolas");
	two->GetTransform()->position = glm::vec2(120, 80);

	AddChild(two);

	Label* three = new Label("Press 3 to arrive", "Consolas");
	three->GetTransform()->position = glm::vec2(120, 120);
	AddChild(three);

	Label* four = new Label("Press 4 to avoid obstacle", "Consolas");
	four->GetTransform()->position = glm::vec2(120, 160);
	AddChild(four);

	/* SOUNDS */
	SoundManager::Instance().Load("../Assets/audio/Klingon.mp3", "music", SoundType::SOUND_MUSIC);
	SoundManager::Instance().PlayMusic("music");

	SoundManager::Instance().Load("../Assets/audio/yay.ogg", "sound", SoundType::SOUND_SFX);
	SoundManager::Instance().PlaySound("sound");

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });

	/* TARGET STUFF*/
	m_pTrgt = new Target();
	m_pTrgt->SetEnabled(false);
	AddChild(m_pTrgt);

	m_pShip = new Ship();
	AddChild(m_pShip);
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Text("Player Input");
	ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	if(ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}
	
	ImGui::End();
}
