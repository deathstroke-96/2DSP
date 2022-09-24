#include "coordinates.h"

Coordinates::Coordinates(const unsigned x, const unsigned y) :
	x_(x), y_(y) {}

Coordinates::Coordinates(const Coordinates& coordinates) = default;

[[nodiscard]] unsigned Coordinates::get_x() const
{
	return x_;
}

[[nodiscard]] unsigned Coordinates::get_y() const
{
	return y_;
}