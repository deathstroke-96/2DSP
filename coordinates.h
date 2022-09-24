#pragma once

class Coordinates
{
	const unsigned x_{};
	const unsigned y_{};

public:
	Coordinates(const unsigned x, const unsigned y);
	Coordinates(const Coordinates& coordinates);

	[[nodiscard]] unsigned get_x() const;
	[[nodiscard]] unsigned get_y() const;
};