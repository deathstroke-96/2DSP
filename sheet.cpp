#include "sheet.h"

Sheet::Sheet(const unsigned length, const unsigned width) :
	length_(length), width_(width) {}

Sheet::Sheet(const Sheet& sheet) :
	length_(sheet.length_), width_(sheet.width_) {}

void Sheet::rotate()
{
	length_ ^= width_ ^= length_ ^= width_;
}

[[nodiscard]] unsigned Sheet::get_length() const
{
	return length_;
}

[[nodiscard]] unsigned Sheet::get_width() const
{
	return width_;
}

[[nodiscard]] const std::unique_ptr<Coordinates>& Sheet::get_bottom_left() const
{
	return bottom_left_;
}

void Sheet::set_bottom_left(std::unique_ptr<Coordinates> coordinates)
{
	bottom_left_ = std::move(coordinates);
}

[[nodiscard]] const std::unique_ptr<Coordinates>& Sheet::get_top_right() const
{
	return top_right_;
}

void Sheet::set_top_right(std::unique_ptr<Coordinates> coordinates)
{
	top_right_ = std::move(coordinates);
}