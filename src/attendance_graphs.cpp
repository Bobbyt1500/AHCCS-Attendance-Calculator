#include "attendance_graphs.h"

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#include "raylib.h"
#include "raygui.h"

#include "attendance_reports.h"
#include "attendance_ui.h"

void GuiReportsGraph(AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts, std::vector<Report> reports, int x_offset, int y_offset, int& range_offset, int& zoom_level) {
	// If there is no data, a graph cannot be made
	if (reports.size() == 0) {
		return;
	}

	int points_in_view = zoom_level * 5;
	int point_offset = (500 / points_in_view);

	GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));

	Rectangle outline = { x_offset, y_offset, 600, 450 };
	DrawRectangleLinesEx(outline, 3, ahccs_colors.yellow);

	// Get report dates, Max/Min participants, and range
	std::vector<std::string> dates;
	int max = reports[0].get_num_participants();
	int min = reports[0].get_num_participants();

	for (Report report : reports) {
		dates.push_back(report.get_report_date().to_string());

		int num_participants = report.get_num_participants();
		if (num_participants > max) max = num_participants;
		if (num_participants < min) min = num_participants;
	}
	int range = max - min;

	// Make the range 1 if it is 0 to prevent dividing by 0
	if (range == 0) range = 1;

	// Draw x-axis labels
	DrawLineEx({ float(x_offset + 50), float(y_offset + 300) }, { float(x_offset + 550), float(y_offset + 300) }, 2.0, ahccs_colors.dark_blue);
	for (int i = 0; i < points_in_view && i < dates.size(); i += zoom_level) {
		int xpos = x_offset + 100 + (point_offset * i);
		draw_x_label(dates[i + range_offset], xpos, ahccs_colors, x_offset, y_offset);
	}
	
	// Draw y-axis labels
	for (int i = 0; i < 5; ++i) {
		int increment = std::nearbyint(range / 5.0);

		// If the increment is less than 1, just draw the max and min
		if (increment < 1) {
			draw_y_label(min, min, range, x_offset, y_offset);
			draw_y_label(max, min, range, x_offset, y_offset);
			break;
		}

		int label_val = min + (increment * i);
		draw_y_label(label_val, min, range, x_offset, y_offset);
	}

	// Draw the lines
	reports_graph_draw_lines(x_offset, y_offset, min, range, range_offset, point_offset, points_in_view, reports, ahccs_colors, ahccs_fonts);

	// Mouse position handler
	reports_graph_mouse_handler(x_offset, y_offset, min, range, range_offset, point_offset, reports, ahccs_colors, ahccs_fonts);

	// Range buttons

	GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.yellow));
	GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
	GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
	if (range_offset != 0) {
		if (GuiButton({ float(x_offset + 100), float(y_offset + 350), 100, 30 }, "#118#")) {
			--range_offset;
		}
	}
	if (points_in_view + range_offset + 1 < reports.size()) {
		if (GuiButton({ float(x_offset + 400), float(y_offset + 350), 100, 30 }, "#119#")) {
			++range_offset;
		}
	}

	// Zoom slider
	GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
	GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(ahccs_colors.lighter_blue));
	GuiLabel({ float(x_offset + 100) , float(y_offset + 400), 100 ,30 }, "Zoom:");
	int new_zoom = GuiScrollBar({ float(x_offset + 200), float(y_offset + 400), 200, 30 }, zoom_level, 1, 40);

	if (new_zoom != zoom_level) {
		range_offset = 0;
		zoom_level = new_zoom;
	}
	

	// Reset styling
	ahccs_colors.set_AHCCS_style();
	
}

void GuiPieGraph(int x_offset, int y_offset, int percentage1, int percentage2, std::string title1, std::string title2) {

	int angle1 = (360 * percentage1) / 100;

	// Pie graph circle sectors
	DrawCircleSector({ float(x_offset + 100), float(y_offset + 55) }, 50, 180, angle1 + 180, 20, ORANGE);
	DrawCircleSector({ float(x_offset + 100), float(y_offset + 55) }, 50, angle1 + 180, 540, 20, RED);
	DrawCircleSectorLines({ float(x_offset + 100), float(y_offset + 55) }, 50, 180, angle1 + 180, 20, WHITE);
	DrawCircleSectorLines({ float(x_offset + 100), float(y_offset + 55) }, 50, angle1 + 180 , 540, 20, WHITE);

	// Title 1 label
	DrawRectangle(x_offset + 24, y_offset + 119, 16, 16, WHITE);
	DrawRectangle(x_offset + 25, y_offset + 120, 14, 14, ORANGE);
	GuiLabel({ float(x_offset + 45) , float(y_offset + 127), 1, 1 }, (title1 + ' ' + std::to_string(percentage1) + '%').c_str());

	// Title 2 label
	DrawRectangle(x_offset + 24, y_offset + 149, 16, 16, WHITE);
	DrawRectangle(x_offset + 25, y_offset + 150, 14, 14, RED);
	GuiLabel({ float(x_offset + 45) , float(y_offset + 157), 1, 1 }, (title2 + ' ' + std::to_string(percentage2) + '%').c_str());
}

void draw_x_label(std::string label_str, int xpos, AHCCS_Colors ahccs_colors, int x_offset, int y_offset) {
	DrawLineEx({ float(xpos) , float(y_offset + 300) }, { float(xpos) , float(y_offset + 305) }, 1.5, ahccs_colors.dark_blue);
	GuiLabel({ float(xpos - 30), float(y_offset + 320), 1, 1 }, label_str.c_str());
}

void draw_y_label(int label_val, int min, int range, int x_offset, int y_offset) {
	int ypos = (y_offset + 290) - ((200 * (label_val - min)) / range);
	GuiLabel({ float(x_offset + 20), float(ypos), 1, 1 }, std::to_string(label_val).c_str());
}

void reports_graph_draw_lines(int x_offset, int y_offset, int min, int range, int range_offset, int point_offset, int points_in_view, std::vector<Report> reports, AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts) {
	// Draw the lines
	int first = true;
	int lastx;
	int lasty;
	int x;
	int y;
	int num_participants;
	for (int i = 0; i < points_in_view && i < reports.size(); ++i) {
		num_participants = reports[i + range_offset].get_num_participants();
		x = x_offset + 100 + (point_offset * i);
		y = (y_offset + 290) - ((200 * (num_participants - min)) / range);

		if (first) {
			first = false;
			lastx = x;
			lasty = y;
			continue;
		}

		DrawLineEx({ float(lastx), float(lasty) }, { float(x), float(y) }, 2, ahccs_colors.dark_blue);

		lastx = x;
		lasty = y;

	}

	// If there is only 1 report, draw a single point
	if (reports.size() == 1) {

		DrawCircle(lastx, lasty, 3, ahccs_colors.dark_blue);
	}
}

void reports_graph_mouse_handler(int x_offset, int y_offset, int min, int range, int range_offset, int point_offset, std::vector<Report> reports, AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts) {
	int mousex = GetMouseX();
	int mousey = GetMouseY();

	// if mouse is within graph
	if (mousey <= y_offset + 300 && mousey >= y_offset + 50 && mousex >= x_offset + 50 && mousex <= x_offset + 550) {
		// Get the report index based off mouse position
		int report_index = (mousex - ((x_offset + 100) - (point_offset / 2))) / point_offset;

		// Check that this is a valid index
		if (report_index < reports.size() && report_index >= 0) {

			int num_participants = reports[report_index + range_offset].get_num_participants();

			// Draw point
			int pointx = x_offset + 100 + (point_offset * report_index);
			int pointy = (y_offset + 290) - ((200 * (num_participants - min)) / range);
			DrawCircle(pointx, pointy, 5, ahccs_colors.dark_blue);

			// Draw detail label
			DrawRectangle(x_offset + 100, y_offset + 10, 400, 30, ahccs_colors.dark_blue);
			GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
			GuiSetFont(ahccs_fonts.bold_font);
			GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
			GuiLabel({ float(x_offset + 150),float(y_offset + 25), 1, 1 }, ("Participants: " + std::to_string(num_participants)).c_str());
			GuiLabel({ float(x_offset + 300),float(y_offset + 25), 1, 1 }, ("Date: " + reports[report_index + range_offset].get_report_date().to_string()).c_str());

			// Reset to regular font from bold
			GuiSetFont(ahccs_fonts.regular_font);
			GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
		}
	}
}