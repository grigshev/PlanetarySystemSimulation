#pragma once

#include <SDL.h>
#include <cmath>

// Screen and performance constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int VIEWPORT_WIDTH = 960;
const int VIEWPORT_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

// Math constants
const float PI = 3.14159f;

// Structures
struct Vector2f
{
	float x;
	float y;
};

struct Vector2i
{
	int16_t x;
	int16_t y;
};

// Game constants
const float GRAVITY_CONSTANT = 0.0002f;
const float STAR_MASS = 2500000.f;
const float PROTOSTAR_RADIUS = 16.0f;
const float STAR_RADIUS = 24.0f;
const float STAR_SURFASE_TEMPERATURE = 6000.f; // K
const Vector2f STAR_POS = { static_cast<float>(VIEWPORT_WIDTH / 2), static_cast<float>(VIEWPORT_HEIGHT / 2) };
const int GAS_CLOUDS_NUMBER = 7500;
const int DUST_CLOUDS_NUMBER = 2500;
const float CLOUD_MASS = 1.0f;
const float CLOUD_RADIUS = 2.5f;
const float CLOUD_START_SPEED_VARIATION_RANGE = 0.02f; //0.05
//const float CLOUD_SPEED_SLOWDOWN = 0.99999f;
const float DUST_LINE = 55.0f;
const float SNOW_LINE = 130.f;
const float PLANETARY_DISK_RADIUS = 400.f;
const float CLOUD_SCATTERING_RADIUS = 450.f;
const float GAS_PLANETOID_DENSITY = 1.5f;
const float DUST_PLANETIOD_DENSITY = 5.0f;

// Game enums

// General helper functions
float GetRandomFloat(float min_value, float max_value);
Vector2f GetRandomDirection();

Vector2f getScalarProduct(Vector2f v, float f);

Vector2f getDirection(float x1, float y1, float x2, float y2);
Vector2f getDirection(Vector2f pos1, Vector2f pos2);

float getDistance(float x1, float y1, float x2, float y2);
float getDistance(Vector2f pos1, Vector2f pos2);

Vector2f getUnitVector(Vector2f vector);

bool checkCircleToCircleCollision(float x1, float y1, float radius1, float x2, float y2, float radius2);
bool checkCircleToCircleCollision(Vector2f pos1, float radius1, Vector2f pos2, float radius2);

// Game specific helper functions
Vector2f getStartObjectPosition();
Vector2f getStartObjectSpeed(Vector2f pos, float variation_range);
Vector2f getUnitGravityVector(Vector2f pos1, Vector2f pos2);
Vector2f getGravityVector(Vector2f pos1, float mass1, Vector2f pos2, float mass2);

