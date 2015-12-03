#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>

using namespace std;

class Utils {
	float Utils::Wpoly6(float r, float h);

	float Utils::Wpoly6Gradient(float r, float h);

	float Utils::Wpoly6Laplacian(float r, float h);

	float Utils::Wspiky(float r, float h);

	float Utils::WspikyGradient(float r, float h);

	float Utils::WspikyLaplacian(float r, float h);

	float Utils::Wviscocity(float r, float h);

	float Utils::WviscocityGradient(float r, float h);

	float Utils::WviscocityLaplacian(float r, float h);
};