#include "Cloud.h"

Cloud::Cloud(Vector2f pos, Vector2f speed, CloudTypes new_type)
{
	m_pos = pos;
	m_speed = speed;
	mass = CLOUD_MASS;
	radius = CLOUD_RADIUS;
	type = new_type;

	switch (type)
	{
	case CloudTypes::GAS:
		min_star_distance = GetRandomFloat(SNOW_LINE, 1.5f * SNOW_LINE);
		break;
	case CloudTypes::DUST:
		min_star_distance = GetRandomFloat(DUST_LINE, 1.2f * DUST_LINE);
		break;
	}

	updateRenderPosition();
}

void Cloud::update(int phase)
{
	updatePosition(phase);
	updateRenderPosition();
	updateDistanceToStar();
	updateStatus();
}

void Cloud::updatePosition(int phase)
{
	if (phase != 1 && distance_to_star < min_star_distance) // snow line
	{
		Vector2f transfer_vector = getScalarProduct(getUnitGravityVector(m_pos, STAR_POS), 0.1f);
		m_pos = { m_pos.x - transfer_vector.x, m_pos.y - transfer_vector.y };
		m_speed = getStartObjectSpeed(m_pos, CLOUD_START_SPEED_VARIATION_RANGE);
	}
	else
	{
		Vector2f gravity_vector = getGravityVector(m_pos, mass, STAR_POS, STAR_MASS);
		m_speed = { m_speed.x + gravity_vector.x, m_speed.y + gravity_vector.y };
	}

	m_pos.x = m_pos.x + m_speed.x;
	m_pos.y = m_pos.y + m_speed.y;
}

void Cloud::updateStatus()
{
	if (distance_to_star < (2 * STAR_RADIUS + radius))
	{
		status = Statuses::SWALLOWED;
	}

	if (distance_to_star > CLOUD_SCATTERING_RADIUS)
	{
		status = Statuses::SCATTERED;
	}
}
