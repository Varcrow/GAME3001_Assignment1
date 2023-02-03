#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class Ship : public Agent
{
public:
	//movement behaviour
	enum Modes 
	{
		STATIC,
		SEEKING,
		FLEEING,
		ARRIVING,
		AVOIDING,
	};
	void SetMode(Modes mode);
	void Seek();
	void Flee();
	void Arrive();
	void Avoid();

	Ship();
	~Ship() override;

	// Inherited via GameObject
	void Draw() override;
	void Update() override;
	void Clean() override;

	void TurnRight();
	void TurnLeft();
	void MoveForward();
	void MoveBack();

	void Move();

	// getters
	[[nodiscard]] float GetMaxSpeed() const;

	// setters
	void SetMaxSpeed(float new_speed);

private:
	void CheckBounds();
	void Reset();

	glm::vec2 m_desiredVelocity;
    float distance;
	glm::vec2 steering;
	float m_maxSpeed;
	float m_turnRate;

	Modes m_currentMode;
};


#endif /* defined (__SHIP__) */

