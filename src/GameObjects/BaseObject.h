#pragma once

#include "../Engine/Global.h"

enum class Statuses
{
	EXISTENT,
	FORMED_PLANETOID,
	FORMED_COMET,
	SWALLOWED,
	SCATTERED
};

class BaseObject
{
public:
	virtual ~BaseObject() {};

	virtual void update(int phase) = 0;

	Vector2f getPosition() const { return m_pos; }
	Vector2i getRenderPosition() const { return rpos; }
	Statuses getStatus() { return status; }
	bool exists() { return status == Statuses::EXISTENT; }
	bool destroyed() { return status != Statuses::EXISTENT; }
	void setStatus(Statuses new_status) { status = new_status; }
	Vector2f getSpeed() { return m_speed; }
	void setSpeed(Vector2f speed) { m_speed = speed; }
	float getMass() const { return mass; }
	float getRadius() const { return radius; }
	void setMass(float new_mass) { mass = new_mass; }
	void setRadius(float new_radius) { radius = new_radius; }
	float getGasMass() { return gas_mass; }
	float getDustMass() { return dust_mass; }
	float getDistanceToStar() { return distance_to_star; }

protected:
	Vector2f m_pos = { 0, 0 };
	Vector2i rpos = { 0, 0 };
	Vector2f m_speed = { 0, 0 };
	Statuses status = Statuses::EXISTENT;
	
	float mass = 0.0f;
	float radius = 0.0f;
	float density = 0.0f;
	float gas_mass = 0.0f;
	float dust_mass = 0.0f;
	float distance_to_star = 0.0f;

	void updateRenderPosition();
	void updateDistanceToStar();
};

