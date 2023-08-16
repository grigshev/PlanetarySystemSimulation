#include "Global.h"

float GetRandomFloat(float min_value, float max_value)
{
	return min_value + static_cast<float>(rand()) * static_cast<float>(max_value - min_value) / RAND_MAX;
}

Vector2f GetRandomDirection()
{
	float angle = GetRandomFloat(0, 2*PI);

	float dir_x = sin(angle);
	float dir_y = cos(angle);

	return { dir_x, dir_y };
}

Vector2f getScalarProduct(Vector2f v, float f)
{
	return { v.x * f, v.y * f };
}

bool checkCircleToCircleCollision(float x1, float y1, float radius1, float x2, float y2, float radius2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float radius_sum = radius1 + radius2;

	if ((dx * dx) + (dy * dy) < (radius_sum * radius_sum))
	{
		return true;
	}

	return false;
}

bool checkCircleToCircleCollision(Vector2f pos1, float radius1, Vector2f pos2, float radius2)
{
	return checkCircleToCircleCollision(pos1, radius1, pos2, radius2);
}

float getDistance(float x1, float y1, float x2, float y2)
{
	Vector2f dir = getDirection(x1, y1, x2, y2);
	float distance = sqrt(dir.x * dir.x + dir.y * dir.y);

	return distance;
}

float getDistance(Vector2f pos1, Vector2f pos2)
{
	return getDistance(pos1.x, pos1.y, pos2.x, pos2.y);
}

Vector2f getDirection(float x1, float y1, float x2, float y2)
{
	return { x2 - x1, y2 - y1 };
}

Vector2f getDirection(Vector2f pos1, Vector2f pos2)
{
	return getDirection(pos1.x, pos1.y, pos2.x, pos2.y);
}

Vector2f getDirection()
{
	return Vector2f();
}

Vector2f getUnitVector(Vector2f vector)
{
	float length = sqrt(vector.x * vector.x + vector.y * vector.y);

	return { vector.x / length, vector.y / length };
}

Vector2f getUnitGravityVector(Vector2f pos1, Vector2f pos2)
{
	Vector2f dir = getDirection(pos1, pos2);
	float length = sqrt(dir.x * dir.x + dir.y * dir.y);

	return { dir.x / length, dir.y / length };
}

Vector2f getGravityVector(Vector2f pos1, float mass1, Vector2f pos2, float mass2)
{
	Vector2f dir = getDirection(pos1, pos2);
	float length = sqrt(dir.x * dir.x + dir.y * dir.y);
	Vector2f gravity_unite_vector = { dir.x / length, dir.y / length };

	float gravity = ((GRAVITY_CONSTANT * 1.0f * mass2) / (length * length));

	Vector2f gravity_vector = { gravity_unite_vector.x * gravity, gravity_unite_vector.y * gravity };

	return gravity_vector;
}

Vector2f getStartObjectPosition()
{
	float modifier = 1.0f;
	int random = rand() % 5;

	switch (random)
	{
	case 0:
		modifier = 0.75f;
		break;
	case 1:
		modifier = 0.85f;
		break;
	case 2:
		modifier = 0.95f;
		break;
	case 3:
		modifier = 1.0f;
		break;
	case 4:
		modifier = CLOUD_SCATTERING_RADIUS / PLANETARY_DISK_RADIUS;
		break;
	}

	float angle = GetRandomFloat(0, 2 * PI);

	Vector2f pos = {
		STAR_POS.x + sin(angle) * sqrt(GetRandomFloat(pow(STAR_RADIUS, 2), pow(modifier * PLANETARY_DISK_RADIUS, 2))),
		STAR_POS.y + cos(angle) * sqrt(GetRandomFloat(pow(STAR_RADIUS, 2), pow(modifier * PLANETARY_DISK_RADIUS, 2)))
	};

	return pos;
}

/*
	V = sqrt(GM/R),
	V - speed, G - gravity constant, M - star mass, R — distance from the star.
	*/
Vector2f getStartObjectSpeed(Vector2f pos, float variation_range = 0.0f)
{
	float mass = 1.0f;
	Vector2f gravity_vector = getGravityVector(pos, mass, STAR_POS, STAR_MASS);
	Vector2f gravity_unit_vector = getUnitVector(gravity_vector);

	float speed_const = 1.0f;
	float linear_speed = sqrt(speed_const * GRAVITY_CONSTANT * STAR_MASS / getDistance(pos, STAR_POS));
	Vector2f result_speed_vector = { gravity_unit_vector.y * linear_speed, -gravity_unit_vector.x * linear_speed };
	
	float random_variation_x = (variation_range > 0.0f) ? GetRandomFloat(-variation_range, variation_range) : 0.0f;
	float random_variation_y = (variation_range > 0.0f) ? GetRandomFloat(-variation_range, variation_range) : 0.0f;
	Vector2f speed = {
		gravity_vector.x - result_speed_vector.x + random_variation_x,
		gravity_vector.y - result_speed_vector.y + random_variation_y
	};

	return speed;
}



