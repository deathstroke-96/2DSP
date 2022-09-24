#pragma once

#include <memory>

#include "coordinates.h"

class Sheet
{
	unsigned length_{};
	unsigned width_{};

	std::unique_ptr<Coordinates> bottom_left_;
	std::unique_ptr<Coordinates> top_right_;

public:
	Sheet(const unsigned length, const unsigned width);
	Sheet(const Sheet& sheet);

	void rotate();

	[[nodiscard]] unsigned get_length() const;
	[[nodiscard]] unsigned get_width() const;

	[[nodiscard]] const std::unique_ptr<Coordinates>& get_bottom_left() const;
	void set_bottom_left(std::unique_ptr<Coordinates> coordinates);

	[[nodiscard]] const std::unique_ptr<Coordinates>& get_top_right() const;
	void set_top_right(std::unique_ptr<Coordinates> coordinates);
};