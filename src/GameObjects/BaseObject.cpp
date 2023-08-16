#include "BaseObject.h"

void BaseObject::updateRenderPosition()
{
	rpos = { static_cast<int16_t>(m_pos.x - radius), static_cast<int16_t>(m_pos.y - radius) };
}

void BaseObject::updateDistanceToStar()
{
	distance_to_star = getDistance(m_pos, STAR_POS);
}
