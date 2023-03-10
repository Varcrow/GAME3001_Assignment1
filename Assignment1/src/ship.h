#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"
#include "Obstacle.h"

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
	void LookWhereYoureGoing(const glm::vec2 targetDirection);
	void Seek();
	void Flee();
	void Arrive();
	void Avoid();
	void SetObstacle(Obstacle* ob);

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

	void Move(glm::vec2 dir);

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
	glm::vec2 avoidance;
	float m_maxSpeed;
	float m_turnRate;

	Modes m_currentMode;

	GameObject* thingToAvoid;
};


#endif /* defined (__SHIP__) */

