#pragma once
#include "KRVector.h"
#include "KRRect.h"

namespace KRM
{
	// Vector types
	using FVector2 = Vector<float, 2>;
	using DVector2 = Vector<double, 2>;
	using FVector3 = Vector<float, 3>;
	using DVector3 = Vector<double, 3>;
	using FVector4 = Vector<float, 4>;
	using DVector4 = Vector<double, 4>;

	// Rect types
	using IRect = Rect<int>;
	using FRect = Rect<float>;
	using DRect = Rect<double>;
}