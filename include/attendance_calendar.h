#ifndef ATTENDANCE_CALENDAR
#define ATTENDANCE_CALENDAR

#include "attendance_ui.h"
#include "attendance_reports.h"

#include <vector>

int GuiReportsCalendar(AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts, int x_offset, int y_offset, int& selected_month, int& selected_year, std::vector<Report> reports);

#endif