#pragma once

#include <list>
#include <set>

#include "sheet.h"

class Canvas
{
	const unsigned length_{};
	const unsigned width_{};
	unsigned required_length_{};

	std::list<const Sheet*> placed_sheets_;
	std::list<const Sheet*> remainder_;

	inline static auto compare_angles_{ [](const Coordinates& l, const Coordinates& r)
	{
		return l.get_x() < r.get_x() ||
			l.get_x() == r.get_x() && l.get_y() < r.get_y();
	} };
	std::set<Coordinates, decltype(compare_angles_)> angles_{ {{0, 0}}, compare_angles_ };

public:
	Canvas(const unsigned length, const unsigned width);

	void place(std::list<Sheet>& sheets);
	void reset();

	[[nodiscard]] unsigned get_length() const;
	[[nodiscard]] unsigned get_width() const;
	[[nodiscard]] unsigned get_required_length() const;

	[[nodiscard]] const std::list<const Sheet*>& get_placed_sheets() const;
	[[nodiscard]] const std::list<const Sheet*>& get_remainder() const;

private:
	[[nodiscard]] std::set<Coordinates>::const_iterator choice(Sheet& sheet) const;
	[[nodiscard]] std::set<Coordinates>::const_iterator try_to_place(const Sheet& sheet) const;
	[[nodiscard]] bool not_intersects(const Sheet& sheet, const Coordinates& angle) const;
};