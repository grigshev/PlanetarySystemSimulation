#pragma once
#include "BaseObject.h"

enum class PlanetoidTypes
{
	COMET,
	ASTEROID,
	DWARF_PLANET,
	ROCKY_EARTH,
	ICY_EARTH,
	OCEAN_EARTH,
	HOT_EARTH,
	GAS_SUBGIANT_25,
	HOT_GAS_SUBGIANT_25,
	GAS_SUBGIANT_35,
	HOT_GAS_SUBGIANT_35,
	GAS_GIANT_45,
	GAS_GIANT_55,
	GAS_GIANT_65,
	GAS_GIANT_75,
	GAS_GIANT_85
};

class Planetoid : public BaseObject
{
public:
	Planetoid(Vector2f pos, Vector2f speed, PlanetoidTypes p_type, int id);

	void update(int phase);
	void addMass(float gas, float dust);
	
	PlanetoidTypes getType() const { return type; }
	int getId() const { return m_id; }
	float getTemperature() const { return temperature; }

	float getSubstanceCollectionRadius() { return substance_collection_radius; }
	float getGravityRadius() { return gravity_radius; }
	
private:
	PlanetoidTypes type = PlanetoidTypes::ASTEROID;
	int m_id = 0;
	int substance_collection_radius = 0;
	int gravity_radius = 0;
	float basic_temperature = 0.0f;
	float temperature = 0.0f;

	void updatePosition();
	void updateType();
	void updateTemperature();
	void updateStatus();
};

