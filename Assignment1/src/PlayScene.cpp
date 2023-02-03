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

void randomizeTargetPositionAndEnable(Target* trgt) {
	trgt->GetTransform()->position.x = rand() % 800;
	trgt->GetTransform()->position.y = rand() % 600;
	trgt->SetEnabled(true);

}

void PlayScene::GetKeyboardInput()
{
	/* 1/2 to do stuff */
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_1))
	{
		randomizeTargetPositionAndEnable(m_pTrgt);
		m_pShip->SetTargetPosition(m_pTrgt->GetTransform()->position);
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_2))
	{
		randomizeTargetPositionAndEnable(m_pTrgt);
		m_pShip->SetTargetPosition(m_pTrgt->GetTransform()->position);
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	/* MORE LABELS */
	m_pSeekLabel = new Label("Press 1 to seek", "Consolas");
	m_pSeekLabel->GetTransform()->position = glm::vec2(120, 40);

	AddChild(m_pSeekLabel);

	m_pArrivalLabel = new Label("Press 2 to flee", "Consolas");
	m_pArrivalLabel->GetTransform()->position = glm::vec2(120, 80);

	AddChild(m_pArrivalLabel);

	Label* three = new Label("Press 3 to arrive", "Consolas");
	three->GetTransform()->position = glm::vec2(120, 120);
	AddChild(three);

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
