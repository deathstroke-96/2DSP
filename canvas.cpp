#include <algorithm>

#include "canvas.h"

Canvas::Canvas(const unsigned length, const unsigned width) :
	length_(length), width_(width) {}

void Canvas::place(std::list<Sheet>& sheets)
{
	for (auto& sheet : sheets)
		if (const auto angle{ choice(sheet) }; angle != angles_.cend())
		{
			const auto top_right_x{ angle->get_x() + sheet.get_length() };
			const auto top_right_y{ angle->get_y() + sheet.get_width() };

			sheet.set_bottom_left(std::make_unique<Coordinates>(*angle));
			sheet.set_top_right(std::make_unique<Coordinates>(top_right_x, top_right_y));
			placed_sheets_.emplace_back(&sheet);

			if (top_right_x > required_length_)
				required_length_ = top_right_x;

			angles_.emplace(angle->get_x(), top_right_y);
			angles_.emplace(top_right_x, angle->get_y());
			angles_.erase(angle);
		}
		else
			remainder_.emplace_back(&sheet);
}

void Canvas::reset()
{
	required_length_ = 0;
	angles_ = { {0,0} };
	placed_sheets_.clear();
	remainder_.clear();
}

[[nodiscard]] std::set<Coordinates>::const_iterator Canvas::choice(Sheet& sheet) const
{
	auto vertical{ try_to_place(sheet) };
	auto horizontal{ try_to_place({sheet.get_width(), sheet.get_length()}) };

	const auto horizontal_is_empty{ horizontal == angles_.cend() };
	const auto both_is_not_empty{ vertical != angles_.cend() && horizontal != angles_.cend() };

	if (horizontal_is_empty ||
		both_is_not_empty && vertical->get_x() + sheet.get_length() <= horizontal->get_x() + sheet.get_width())
		return vertical;

	sheet.rotate();
	return horizontal;
}

[[nodiscard]] std::set<Coordinates>::const_iterator Canvas::try_to_place(const Sheet& sheet) const
{
	return std::find_if(angles_.cbegin(), angles_.cend(),
		[this, &sheet](const Coordinates& angle)
		{
			const auto fits_x{ angle.get_x() + sheet.get_length() <= length_ };
			const auto fits_y{ angle.get_y() + sheet.get_width() <= width_ };
			const auto no_intersects{ not_intersects(sheet, angle) };

			return fits_x && fits_y && no_intersects;
		}
	);
}

[[nodiscard]] bool Canvas::not_intersects(const Sheet& sheet, const Coordinates& angle) const
{
	const auto left_edge_1{ angle.get_x() };
	const auto right_edge_1{ angle.get_x() + sheet.get_length() };
	const auto bottom_edge_1{ angle.get_y() };
	const auto top_edge_1{ angle.get_y() + sheet.get_width() };

	return std::all_of(placed_sheets_.cbegin(), placed_sheets_.cend(),
		[&left_edge_1, &right_edge_1, &bottom_edge_1, &top_edge_1](const Sheet* const placed_sheet)
		{
			const auto left_edge_2{ placed_sheet->get_bottom_left()->get_x() };
			const auto right_edge_2{ placed_sheet->get_top_right()->get_x() };
			const auto bottom_edge_2{ placed_sheet->get_bottom_left()->get_y() };
			const auto top_edge_2{ placed_sheet->get_top_right()->get_y() };

			return right_edge_1 <= left_edge_2 || right_edge_2 <= left_edge_1 ||
				top_edge_1 <= bottom_edge_2 || top_edge_2 <= bottom_edge_1;
		});
}

[[nodiscard]] unsigned Canvas::get_length() const
{
	return length_;
}

[[nodiscard]] unsigned Canvas::get_width() const
{
	return width_;
}

[[nodiscard]] unsigned Canvas::get_required_length() const
{
	return required_length_;
}

[[nodiscard]] const std::list<const Sheet*>& Canvas::get_placed_sheets() const
{
	return placed_sheets_;
}

[[nodiscard]] const std::list<const Sheet*>& Canvas::get_remainder() const
{
	return remainder_;
}