#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "canvas.h"

[[nodiscard]] Canvas input_canvas();
[[nodiscard]] std::list<Sheet> input_sheets();

void print(const Canvas& canvas);
void print_to_file(const Canvas& canvas, const std::string& filename);

int main()
{
	auto canvas{ input_canvas() };
	auto sheets{ input_sheets() };

	std::for_each(sheets.begin(), sheets.end(), [](Sheet& sheet)
		{
			if (sheet.get_length() > sheet.get_width())
				sheet.rotate();
		});

	auto sort_by_width{ [](const Sheet& l, const Sheet& r)
		{
			return l.get_width() > r.get_width() ||
				l.get_width() == r.get_width() && l.get_length() > r.get_length();
		} };
	sheets.sort(sort_by_width);
	canvas.place(sheets);
	std::cout << std::endl << "Sort by width:" << std::endl;
	print(canvas);
	print_to_file(canvas, "sort by width.plt");

	canvas.reset();
	auto sort_by_area{ [](const Sheet& l, const Sheet& r)
		{
			const unsigned l_area{ l.get_length() * l.get_width() };
			const unsigned r_area{ r.get_length() * r.get_width() };

			return l_area > r_area ||
				l_area == r_area && l.get_width() > r.get_width();
		} };
	sheets.sort(sort_by_area);
	canvas.place(sheets);
	std::cout << std::endl << "Sort by area:" << std::endl;
	print(canvas);
	print_to_file(canvas, "sort by area.plt");
}

[[nodiscard]] Canvas input_canvas()
{
	std::cout << "Enter the dimensions of the canvas in centimeters: ";
	unsigned length{}, width{};
	std::cin >> length >> width;

	if (width > length)
		width ^= length ^= width ^= length;

	return { length, width };
}

[[nodiscard]] std::list<Sheet> input_sheets()
{
	std::list<Sheet> sheets;
	unsigned length{}, width{}, count{};
	std::cout << std::endl << "To interrupt input, enter not a number." << std::endl;
	std::cout << "Enter length, width and count of sheets:" << std::endl;

	while (std::cin >> length >> width >> count)
		sheets.insert(sheets.end(), count, { length, width });

	return sheets;
}

void print(const Canvas& canvas)
{
	if (const auto& placed_sheets{ canvas.get_placed_sheets() }; !placed_sheets.empty())
	{
		std::cout << "Count of placed sheets: " << placed_sheets.size() << std::endl;

		for (const auto& sheet : placed_sheets)
		{
			std::cout << "Length = " << sheet->get_length() << ", width = " << sheet->get_width();
			std::cout << ", coordinates of the bottom left corner = { " << sheet->get_bottom_left()->get_x() << ", "
				<< sheet->get_bottom_left()->get_y() << " }" << std::endl;
		}

		std::cout << std::endl << "Required length = " << canvas.get_required_length() << std::endl << std::endl;
	}

	if (const auto& remainder{ canvas.get_remainder() }; !remainder.empty())
	{
		std::cout << "Count of not placed sheets: " << remainder.size() << std::endl;

		for (const auto& sheet : remainder)
			std::cout << "Length = " << sheet->get_length() << ", width = " << sheet->get_width() << std::endl;

		std::cout << std::endl;
	}
}

void print_to_file(const Canvas& canvas, const std::string& filename)
{
	std::ofstream fout(filename);

	fout << "set size ratio -1" << std::endl;
	fout << "set title \"Required length: " << canvas.get_required_length() << '"' << std::endl;
	fout << "set xlabel \"Length\"" << std::endl;
	fout << "set ylabel \"Width\"" << std::endl;
	fout << "set xrange [ 0 : " << canvas.get_length() << ']' << std::endl;
	fout << "set yrange [ 0 : " << canvas.get_width() << ']' << std::endl;

	unsigned index{ 1 };

	for (const auto& sheet : canvas.get_placed_sheets())
	{
		const auto length{ sheet->get_length() };
		const auto width{ sheet->get_width() };

		const auto bottom_left_x{ sheet->get_bottom_left()->get_x() };
		const auto bottom_left_y{ sheet->get_bottom_left()->get_y() };

		const auto top_right_x{ sheet->get_top_right()->get_x() };
		const auto top_right_y{ sheet->get_top_right()->get_y() };

		const auto center_x{ top_right_x - length / 2. };
		const auto center_y{ top_right_y - width / 2. };

		fout << "set object " << index << " rect from " << bottom_left_x
			<< ", " << bottom_left_y << " to " << top_right_x << ", " << top_right_y << std::endl;
		fout << "set object " << index
			<< " front clip lw 1.0 dashtype solid fc rgb \"#009e73\" fillstyle solid 1.00 border lt -1" << std::endl;
		fout << "set label " << index++ << " \"" << length << 'x' << width
			<< "\" at " << center_x << ", " << center_y << " center ";
		fout << (length >= width ? "norotate" : "rotate") << " front nopoint" << std::endl;
	}

	fout << "plot 0";
}