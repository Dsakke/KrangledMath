#pragma once
namespace KRM
{
	template<typename T>
	struct Rect final
	{
		static_assert(std::is_arithmetic<T>::value);

	public:
		Rect(T x, T y, T width, T height)
			: x{ x }, y{ y }, width{ width }, height{ height }
		{}

		T x;
		T y;
		T width;
		T height;
	};
}