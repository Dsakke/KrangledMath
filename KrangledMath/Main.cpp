#include "KRMath\KRMath.h"
#include <string>
#include<math.h>

#define CATCH_CONFIG_MAIN

#include "catch.hpp"


TEST_CASE("Magnitude Tests")
{
	KRM::FVector3 vec3{};
	vec3.x = 1;
	float magnitude = vec3.Magnitude();

	REQUIRE(abs(magnitude - 1) <= FLT_EPSILON);
	REQUIRE(abs(vec3.SqrMagnitude() - magnitude * magnitude) <= FLT_EPSILON);

	KRM::FVector2 vec2{};
	vec2.x = 1;
	vec2.y = 1;
	magnitude = vec2.Magnitude();
	REQUIRE(magnitude - sqrtf(2) <= FLT_EPSILON);
	REQUIRE(vec2.SqrMagnitude() - 2 <= FLT_EPSILON);
}

TEST_CASE("Normalize Tests")
{
	double epsilon = 0.0000001;

	// Vec3 test
	KRM::FVector3 vec3{};
	vec3.x = 7;
	vec3.y = 4;
	vec3.z = 56;

	KRM::FVector3 vec3Normal = vec3.GetNormalized();

	REQUIRE(abs(vec3Normal.Magnitude() - 1.f) < FLT_EPSILON);

	vec3Normal *= vec3.Magnitude();
	REQUIRE(abs(vec3Normal.x - vec3.x) <= FLT_EPSILON);
	REQUIRE(abs(vec3Normal.y - vec3.y) <= FLT_EPSILON);
	REQUIRE(abs(vec3Normal.z - vec3.z) <= FLT_EPSILON);

	// Vec2 test
	KRM::DVector2 vec2{};
	vec2.x = 1;
	vec2.y = 1;

	KRM::DVector2 vec2Normal = vec2.Normalize();

	REQUIRE(abs(vec2Normal.Magnitude() - 1.0) < epsilon);

	vec2Normal *= vec2.Magnitude();
	REQUIRE(abs(vec2Normal.x - vec2.x) < epsilon);
	REQUIRE(abs(vec2Normal.y - vec2.y) < epsilon);


	// largeVec test
	const int size = 5;

	KRM::Vector<float, size> largeVec{};
	for (int i{}; i < size; ++i)
	{
		largeVec.m_Data[i] = (float)(std::rand() % 100);
	}

	KRM::Vector<float, size> largeVecNormal = largeVec.GetNormalized();
	REQUIRE(abs(largeVecNormal.Magnitude() - 1.f) < (float)epsilon);

	largeVecNormal *= largeVec.Magnitude();

	bool allCorrect = true;
	for (int i{}; i < size; ++i)
	{
		allCorrect = (abs(largeVecNormal.m_Data[i] - largeVec.m_Data[i]) < (float)epsilon) && allCorrect;
		if (!allCorrect)
		{
			break;
		}
	}
	REQUIRE(allCorrect);
}

TEST_CASE("Dot Tests")
{
	// Checking the properties described here: https://en.wikipedia.org/wiki/Dot_product#Properties
	// Communicativity test
	{ // just doing an artificial scope to keep intelisense clean
		KRM::FVector2 vec0{ 23.f, 1.342f };
		KRM::FVector2 vec1{ 2.f, 3.534f };

		float dotVec0 = vec0.Dot(vec1);
		float dotVec1 = vec1.Dot(vec0);

		REQUIRE(abs(dotVec0 - dotVec1) <= FLT_EPSILON);

		KRM::DVector3 vec2{ 23, 1.342, 4.43 };
		KRM::DVector3 vec3{ 2, 3.534,  3.61 };

		double dotVec2 = vec2.Dot(vec3);
		double dotVec3 = vec3.Dot(vec2);

		REQUIRE(abs(dotVec2 - dotVec3) <= DBL_EPSILON);
	}


	// Distrubitive over vector addition test
	{
		KRM::DVector2 vec0{ 1.132, 3.34 };
		KRM::DVector2 vec1{ 2.324, 0.342 };
		KRM::DVector2 vec2{ 4.34, 2.254 };

		double dot0 = (vec0 + vec1).Dot(vec2);
		double distributedDot0 = vec0.Dot(vec2) + vec1.Dot(vec2);

		REQUIRE(abs(dot0 - distributedDot0) <= 0.0000001);


		KRM::FVector3 vec3{ 1.324f, 1.806f, 0.04f };
		KRM::FVector3 vec4{ 2.421f, 1.405f, 2.12f };
		KRM::FVector3 vec5{ 1.33f, 1.206f, 1.25f };

		float dot1 = (vec3 + vec4).Dot(vec5);
		float distributedDot1 = vec3.Dot(vec5) + vec4.Dot(vec5);

		REQUIRE(abs(dot1 - distributedDot1) <= FLT_EPSILON);
	}

	// is Bilinear
	{
		float scalar = float(rand() % 10000) / 1000;
		KRM::FVector3 vec0{ 1.234f, 2.534f, 1.4f };
		KRM::FVector3 vec1{ 0.34f, 3.294f, 0.4f };
		KRM::FVector3 vec2{ 3.432f, 0.837f, 1.89f };

		float dot0 = (scalar * vec0 + vec1).Dot(vec2);
		float dot1 = scalar * vec0.Dot(vec2) + (vec1.Dot(vec2));

		REQUIRE(abs(dot0 - dot1) <= FLT_EPSILON);
	}

	// Scalar multiplication
	{
		float scalar0 = float(rand() % 10000) / 1000;
		float scalar1 = float(rand() % 10000) / 1000;

		KRM::FVector3 vec0{ 1.494f, 3.334f, 0.412f };
		KRM::FVector3 vec1{ 2.84f, 4.023f, 1.4f };

		float dot0 = (scalar0 * vec0).Dot(scalar1 * vec1);
		float dot1 = scalar0 * scalar1 * vec0.Dot(vec1);

		REQUIRE(abs(dot0 - dot1) <= 0.0001f);
	}

	// Check orthoganal is correct
	// If the dot of two vectors is 0 they are orthoganal
	{
		KRM::FVector2 vec0{ 1, 0 };
		KRM::FVector2 vec1{ 0, 1 };

		float dot0 = vec0.Dot(vec1);
		REQUIRE(dot0 < FLT_EPSILON);
	}

	// Random test cases
	{
		float expectedResult01 = 1.f;
		KRM::FVector2 vec0{ 1, 0 };
		KRM::FVector2 vec1{ 1, 1 };

		REQUIRE(vec0.Dot(vec1) - expectedResult01 < FLT_EPSILON);


		float expectedResult23 = 19.f;
		KRM::FVector2 vec2{ 5, 3 };
		KRM::FVector2 vec3{ 2, 3 };

		REQUIRE(vec2.Dot(vec3) - expectedResult23 < FLT_EPSILON);
	}
}