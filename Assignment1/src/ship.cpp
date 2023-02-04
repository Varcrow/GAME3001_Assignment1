#include "Ship.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include "CollisionManager.h"
#include "Game.h"

Ship::Ship() : m_maxSpeed(5.0f)
{
	TextureManager::Instance().Load("../Assets/textures/ship3.png", "ship");

	const auto size = TextureManager::Instance().GetTextureSize("ship");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));

	GetTransform()->position = glm::vec2(400.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	SetType(GameObjectType::AGENT);
	SetMode(STATIC);

	SetCurrentHeading(0.0f);// current facing angle
	SetCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 10.0f; // 5 degrees per frame

	SetLOSDistance(420.0f); // 5 ppf x 80 feet
	SetLOSColour(glm::vec4(1, 0, 0, 1));
}


Ship::~Ship()
= default;

void Ship::SetMode(Modes mode) 
{
	m_currentMode = mode;
}

void Ship::Draw()
{
	// draw the ship
	TextureManager::Instance().Draw("ship", GetTransform()->position, GetCurrentHeading(), 255, true);

	// draw LOS
	Util::DrawLine(GetTransform()->position, GetTransform()->position + GetCurrentDirection() * GetLOSDistance(), GetLOSColour());
}

void Ship::Update()
{
	if (m_currentMode == SEEKING) {
		Seek();
	}
	if (m_currentMode == FLEEING) {
		Flee();
	}
	if (m_currentMode == ARRIVING) {
		Arrive();
	}
	if (m_currentMode == AVOIDING) {
		Avoid();
	}
}

void Ship::Clean()
{
}


void Ship::Move(glm::vec2 dir) {
	GetRigidBody()->acceleration = GetCurrentDirection() *= 5;

	GetRigidBody()->velocity = GetCurrentDirection();

	if (Util::Distance(GetTransform()->position, GetTargetPosition()) > 40) {
		GetTransform()->position = (GetTransform()->position) + (GetRigidBody()->velocity * Game::Instance().GetDeltaTime()) + (GetRigidBody()->acceleration);
	}

	GetRigidBody()->velocity += GetRigidBody()->acceleration;

	steering = -(m_desiredVelocity - GetRigidBody()->velocity);
}

void Ship::Seek() 
{
//	m_desiredVelocity = GetTargetPosition() - GetTransform()->position;
//	m_desiredVelocity = Util::Normalize(m_desiredVelocity);
//	steering = m_desiredVelocity - GetRigidBody()->velocity;

//	Move();

	glm::vec2 wishDir = GetTargetPosition() - GetTransform()->position;
	wishDir = Util::Normalize(wishDir);

	LookWhereYoureGoing(wishDir);
	Move({});
	
}

void Ship::Flee() 
{
	glm::vec2 wishDir = GetTargetPosition() - GetTransform()->position;
	wishDir = Util::Normalize(wishDir);
	wishDir = -wishDir;

	LookWhereYoureGoing(wishDir);

	Move({});
}

void Ship::Arrive() 
{
	glm::vec2 wishDir = GetTargetPosition() - GetTransform()->position;
	wishDir = Util::Normalize(wishDir);

	LookWhereYoureGoing(wishDir);

	distance = Util::Distance(GetTransform()->position, GetTargetPosition());
	GetRigidBody()->acceleration = GetCurrentDirection() *= 5;
	if(distance < 200) 
		GetRigidBody()->acceleration = GetCurrentDirection() *= distance / 25;

	GetRigidBody()->velocity = GetCurrentDirection();

	if (Util::Distance(GetTransform()->position, GetTargetPosition()) > 10) {
		GetTransform()->position = (GetTransform()->position) + (GetRigidBody()->velocity * Game::Instance().GetDeltaTime()) + (GetRigidBody()->acceleration);
	}

	GetRigidBody()->velocity += GetRigidBody()->acceleration;

	steering = -(m_desiredVelocity - GetRigidBody()->velocity);
}

void Ship::Avoid()
{
	Seek();
}

void Ship::LookWhereYoureGoing(const glm::vec2 targetDirection)
{

	const float targetRotation = Util::SignedAngle(GetCurrentDirection(), targetDirection);

	bool hchange = false;

	if (thingToAvoid != nullptr && thingToAvoid->IsEnabled())
	{
		if (CollisionManager::LineAABBCheck(this, thingToAvoid)) {
			SetCurrentHeading(GetCurrentHeading() + m_turnRate);
			hchange = true;
		}
		
	}
	if (abs(targetRotation) > m_turnRate && hchange == false)
	{
		if (targetRotation > 0.0f)
		{
			SetCurrentHeading(GetCurrentHeading() + m_turnRate);
		}
		else if (targetRotation < 0.0f)
		{
			SetCurrentHeading(GetCurrentHeading() - m_turnRate);
		}
	}
}

void Ship::SetObstacle(Obstacle* ob)
{
	thingToAvoid = ob;
}

void Ship::TurnRight()
{
	SetCurrentHeading(GetCurrentHeading() + m_turnRate);
	if (GetCurrentHeading() >= 360)
	{
		SetCurrentHeading(GetCurrentHeading() - 360.0f);
	}
}

void Ship::TurnLeft()
{
	SetCurrentHeading(GetCurrentHeading() - m_turnRate);
	if (GetCurrentHeading() < 0)
	{
		SetCurrentHeading(GetCurrentHeading() + 360.0f);
	}
}

void Ship::MoveForward()
{
	GetRigidBody()->velocity = GetCurrentDirection() * m_maxSpeed;
}

void Ship::MoveBack()
{
	GetRigidBody()->velocity = GetCurrentDirection() * -m_maxSpeed;
}

float Ship::GetMaxSpeed() const
{
	return m_maxSpeed;
}

void Ship::SetMaxSpeed(const float new_speed)
{
	m_maxSpeed = new_speed;
}

void Ship::CheckBounds()
{

	if (GetTransform()->position.x > Config::SCREEN_WIDTH)
	{
		GetTransform()->position = glm::vec2(0.0f, GetTransform()->position.y);
	}

	if (GetTransform()->position.x < 0)
	{
		GetTransform()->position = glm::vec2(800.0f, GetTransform()->position.y);
	}

	if (GetTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		GetTransform()->position = glm::vec2(GetTransform()->position.x, 0.0f);
	}

	if (GetTransform()->position.y < 0)
	{
		GetTransform()->position = glm::vec2(GetTransform()->position.x, 600.0f);
	}

}

void Ship::Reset()
{
	GetRigidBody()->isColliding = false;
	const int half_width = static_cast<int>(GetWidth() * 0.5);
	const auto x_component = rand() % (640 - GetWidth()) + half_width + 1;
	const auto y_component = -GetHeight();
	GetTransform()->position = glm::vec2(x_component, y_component);
}

