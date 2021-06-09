#ifndef ATTENDANCE_GRAPHS
#define ATTENDANCE_GRAPHS

#include <vector>

#include "attendance_ui.h"
#include "attendance_reports.h"

void GuiReportsGraph(AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts, std::vector<Report> reports, int x_offset, int y_offset, int& range_offset, int& zoom_level);

void GuiPieGraph(int x_offset, int y_offset, int percentage1, int percentage2, std::string title1, std::string title2);

void draw_x_label(std::string label_str, int xpos, AHCCS_Colors ahccs_colors, int x_offset, int y_offset);

void draw_y_label(int label_val, int min, int range, int x_offset, int y_offset);

void reports_graph_mouse_handler(int x_offset, int y_offset, int min, int range, int range_offset, int point_offset, std::vector<Report> reports, AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts);

void reports_graph_draw_lines(int x_offset, int y_offset, int min, int range, int range_offset, int point_offset, int points_in_view, std::vector<Report> reports, AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts);

#endif