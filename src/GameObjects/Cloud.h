#pragma once
#include "BaseObject.h"
#include "../Engine/Global.h"

enum class CloudTypes
{
	GAS,
	DUST
};

class Cloud : public BaseObject
{
public:
	Cloud(Vector2f pos, Vector2f speed, CloudTypes new_type);

	CloudTypes getType() const { return type; }
	void update(int phase);

private:
	CloudTypes type = CloudTypes::GAS;
	float min_star_distance = 0.f;

	void updatePosition(int phase);
	void updateStatus();
};

