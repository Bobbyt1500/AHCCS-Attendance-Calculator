#include "attendance_calendar.h"
#include "attendance_ui.h"
#include "attendance_reports.h"
#include "attendance_dates.h"

#include "date.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <string>

int GuiReportsCalendar(AHCCS_Colors ahccs_colors, AHCCS_Fonts ahccs_fonts, int x_offset, int y_offset, int& selected_month, int& selected_year, std::vector<Report> reports) {
    GuiSetFont(ahccs_fonts.bold_font);
	GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));

    // Map all dates in this month that have reports to their index in "reports" vector
    std::map<int, int> dates = {};
    for (int i = 0; i < reports.size(); ++i) {
        Date date = reports[i].get_report_date();
        if (date.month == selected_month && date.year == selected_year) {
            dates[date.day] = i;
        }
    }

    // Draw month label and buttons
    std::vector<std::string> month_labels = {"January", "Febuary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    GuiLabel({x_offset + 250, y_offset, 200, 25}, (month_labels[selected_month-1] + ' ' + std::to_string(selected_year)).c_str());

    // Draw weekday labels
    std::vector<std::string> weekday_labels = {"Sun", "Mon", "Tue", "Wed", "Thurs", "Fri", "Sat"};

    for (int i = 0; i < weekday_labels.size(); ++i) {
        
        GuiLabel({x_offset+(100*i), y_offset +35, 90, 25}, weekday_labels[i].c_str());
    }

    // Draw the calendar day boxes
    int current_day = 1;

    std::vector<int> days_in_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Account for leapyear
    if (selected_year % 4 == 0 && (selected_year % 400 == 0 || selected_year % 100 != 0)) {
        days_in_month[1] = 29;
    }

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));    
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 7; ++j) {
            // Only draw up to the last day in the month
            if (current_day > days_in_month[selected_month-1]) {
                break;
            }

            // Only draw if the loop is on the correct weekday
            if (date::weekday(date::month{selected_month}/date::day{current_day}/date::year{selected_year}).c_encoding() == j) {

                // Draw box
                int posx = x_offset + (100*j);
                int posy = y_offset + 60 + (60*i);

                // Check if this day has a report
                
                if (dates.find(current_day) != dates.end()) {
                    if (GetMouseX() > posx && GetMouseX() < posx + 90 && GetMouseY() > posy && GetMouseY() < posy+50) {
                        DrawRectangle(posx, posy, 90, 50, ahccs_colors.lighter_blue);
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            // Reset to default styles
                            ahccs_colors.set_AHCCS_style();
                            GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
                            GuiSetFont(ahccs_fonts.regular_font);
                            GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
                            
                            return dates[current_day];
                        }
                    } else {
                        DrawRectangle(posx, posy, 90, 50, ahccs_colors.yellow); 
                    }
                    
                } else {
                    DrawRectangle(posx, posy, 90, 50, ahccs_colors.dark_blue);
                } 

                
                

                // Draw number label
                GuiLabel({ posx, posy, 90, 50 }, std::to_string(current_day).c_str());

                ++current_day;
            }
        }
    }

    if (GuiButton({x_offset + 175, y_offset+5, 50, 25}, "#118#")) {
        if (selected_month > 1) {
            --selected_month;
        } else {
            selected_month = 12;
            --selected_year;
        }
    }
    if (GuiButton({x_offset + 475, y_offset+5, 50, 25}, "#119#")) {
        if (selected_month < 12) {
            ++selected_month;
        } else {
            selected_month = 1;
            ++selected_year;
        }
    }

    // Reset to default styles
    ahccs_colors.set_AHCCS_style();
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    GuiSetFont(ahccs_fonts.regular_font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 22);

    // -1 if no report selected
    return -1;
    
}