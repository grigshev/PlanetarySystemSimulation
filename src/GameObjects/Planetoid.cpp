#include "Planetoid.h"


Planetoid::Planetoid(Vector2f pos, Vector2f speed, PlanetoidTypes p_type, int id)
{
	m_pos = pos;
	m_speed = speed;
	m_id = id;
	type = p_type;

	if (type == PlanetoidTypes::COMET)
	{
		addMass(CLOUD_MASS, 0.f);
	}
	else
	{
		addMass(0.f, CLOUD_MASS);
	}

	updateRenderPosition();
}

void Planetoid::update(int phase)
{
	updatePosition();
	updateRenderPosition();
	updateDistanceToStar();
	updateTemperature();
	updateType();
	updateStatus();
}

void Planetoid::updatePosition()
{
	Vector2f gravity_vector = getGravityVector(m_pos, mass, STAR_POS, STAR_MASS);

	m_speed = { m_speed.x + gravity_vector.x, m_speed.y + gravity_vector.y };

	m_pos.x = m_pos.x + m_speed.x;
	m_pos.y = m_pos.y + m_speed.y;
}

void Planetoid::addMass(float gas, float dust)
{
	gas_mass += gas;
	dust_mass += dust;
	mass += (gas + dust);
	density = (gas_mass * GAS_PLANETOID_DENSITY + dust_mass * DUST_PLANETIOD_DENSITY) / mass;

	radius = pow((3.0f * mass) / (4.0f * PI * density), 0.33f);

	float sqrt_mass = sqrt(mass);
	gravity_radius = 10.0f * sqrt_mass;
	substance_collection_radius = (2.0f * sqrt_mass > 5.0f * radius) ? 5.0f * radius : 2.0f * sqrt_mass;

	float added_temperature = 500.f * (gas + dust) / mass;
	temperature += added_temperature;

	updateTemperature();
	updateType();
}

void Planetoid::updateType()
{
	if (radius >= 8.5f)
	{
		type = PlanetoidTypes::GAS_GIANT_85;
	}
	else if (radius >= 7.5f)
	{
		type = PlanetoidTypes::GAS_GIANT_75;
	}
	else if (radius >= 6.5f)
	{
		type = PlanetoidTypes::GAS_GIANT_65;
	}
	else if (radius >= 5.5f)
	{
		type = PlanetoidTypes::GAS_GIANT_55;
	}
	else if (radius >= 4.5f)
	{
		type = PlanetoidTypes::GAS_GIANT_45;
	}
	else if (radius >= 3.5f)
	{
		type = (temperature >= 800.f) ? PlanetoidTypes::HOT_GAS_SUBGIANT_35 : PlanetoidTypes::GAS_SUBGIANT_35;
	}
	else if (radius >= 2.0f)
	{
		type = (temperature >= 800.f) ? PlanetoidTypes::HOT_GAS_SUBGIANT_25 : PlanetoidTypes::GAS_SUBGIANT_25;
	}
	else if (mass >= 10.0f)
	{
		if (temperature >= 800.f)
		{
			type = PlanetoidTypes::HOT_EARTH;
		}
		else if ((gas_mass > 0.1f && gas_mass <= 2.1f && temperature > 273.f && temperature <= 373.f) ||
			(gas_mass > 3.1f && temperature > 273.f))
		{
			type = PlanetoidTypes::OCEAN_EARTH;
		}
		else if (gas_mass > 0.1f && temperature <= 273.f)
		{
			type = PlanetoidTypes::ICY_EARTH;
		}
		else
		{
			type = PlanetoidTypes::ROCKY_EARTH;
		}
	}
	else if (mass >= 5.0f)
	{
		type = PlanetoidTypes::DWARF_PLANET;
	}
	else if (mass <= 2.1f && gas_mass > 0.1f && dust_mass < 0.1f)
	{
		type = PlanetoidTypes::COMET;
	}
	else
	{
		type = PlanetoidTypes::ASTEROID;
	}
}

void Planetoid::updateTemperature()
{
	basic_temperature = 1350.f / pow(distance_to_star, 0.33f);

	// Warmhouse effect
	if (gas_mass > 0.1f)
	{
		basic_temperature += 15;
	}
	
	if (temperature < basic_temperature)
	{
		temperature = basic_temperature;
	}
	else if (temperature > 2500.f)
	{
		temperature = 2500.f;
	}
	else
	{
		temperature -= 0.1f;
	}
}

void Planetoid::updateStatus()
{
	if (distance_to_star < (2 * STAR_RADIUS + radius))
	{
		status = Statuses::SWALLOWED;
	}
}
