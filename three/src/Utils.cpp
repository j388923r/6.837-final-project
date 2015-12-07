#include "Utils.h"

float Utils::Wpoly6(float r, float h)
{
	float leadingValue = 315 / (64 * M_PI * pow(h, 9));
	float radialValue = pow((h * h - r * r), 3);
	return leadingValue * radialValue;
}

float Utils::Wpoly6Gradient(float r, float h)
{
	float leadingValue = -r * 945 / (32 * M_PI * pow(h, 9));
	float radialValue = pow((h * h - r * r), 2);
	return leadingValue * radialValue;
}

float Utils::Wpoly6Laplacian(float r, float h)
{
	if ( h > r )
		return 0;
	float leadingValue = 945 / (8 * M_PI * pow(h, 9));
	float h2r2 = (h * h - r * r);
	float finalValue = r * r - 3 * h2r2 / 4;
	return leadingValue * h2r2 * finalValue;
}

float Utils::Wspiky(float r, float h)
{
	float leadingValue = 15 / (M_PI * pow(h, 6));
	float radialValue = pow((h - r), 3);
	return leadingValue * radialValue;
}

float Utils::WspikyGradient(float r, float h)
{
	float leadingValue = -45 / (M_PI * pow(h, 6) * r);
	float radialValue = pow((h - r), 2);
	return leadingValue * radialValue;
}

float Utils::WspikyLaplacian(float r, float h)
{
	return 0.0f;
}

float Utils::Wviscocity(float r, float h)
{
	float leadingValue = 15 / (2 * M_PI * pow(h, 3));
	float radialValue = -pow(r, 3) / (2 * pow(h, 3)) + r * r / (h * h) + h / (2 * r) - 1;
	return leadingValue * radialValue;
}

float Utils::WviscocityGradient(float r, float h)
{
	float leadingValue = r * 15 / (2 * M_PI * pow(h, 3));
	float radialValue = -3 * r / (2 * pow(h, 3)) + 2 / (h * h) - h / (2 * pow(r, 3));
	return leadingValue * radialValue;
}

float Utils::WviscocityLaplacian(float r, float h)
{
	float leadingValue = 45 / (M_PI * pow(h, 5));
	float radialValue = 1 - r / h;
	return leadingValue * radialValue;
}

const float Particle::stretcher = 10.0f;
