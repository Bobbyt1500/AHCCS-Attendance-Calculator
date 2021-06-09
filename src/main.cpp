/*
    Developed For: Achievement House Cyber Charter School
    Original Author: Robert Thierry
    Date Created: 12/19/2020
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <ctime>

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION

#include "json.hpp"

#include "attendance_filesystem.h"
#include "attendance_dates.h"
#include "attendance_course.h"
#include "attendance_reports.h"
#include "attendance_utils.h"
#include "attendance_ui.h"
#include "attendance_data.h"
#include "attendance_graphs.h"
#include "attendance_calendar.h"





AHCCS_Colors ahccs_colors;
AHCCS_Fonts ahccs_fonts;
Control_Variables control_vars;

Calculator_Data data;



void draw_reports_view(Course active_course) {
    if (control_vars.selected_report  == -1) {

        control_vars.selected_report = GuiReportsCalendar(ahccs_colors, ahccs_fonts, 100, 150, control_vars.calendar_month, control_vars.calendar_year, active_course.get_all_reports());

        // Import reports button
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.yellow));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        if (GuiButton({ 375,530,150,40 }, "Import Reports")) data.import_reports(control_vars);

    } else {
    
        // Set styling
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.lighter_blue));
        GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(ahccs_colors.lighter_blue));
        GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(ahccs_colors.lighter_blue));


        std::vector<Report> all_course_reports = active_course.get_all_reports();
        Report& active_report = all_course_reports[control_vars.selected_report];

        control_vars.create_attendees_list(active_report);
        control_vars.selected_attendee = GuiListView({ 50, 130, 250, 300 }, control_vars.attendees_list.c_str(), &control_vars.attendees_scroll, control_vars.selected_attendee);
        GuiLabel({ 50, 80, 250, 75 }, "Attendees:");

        // Attendee Details
        if (control_vars.selected_attendee != -1 && control_vars.selected_attendee < active_report.get_num_attendees()) {

            Report::Attendee& active_attendee = active_report.attendees[control_vars.selected_attendee];

            // Rectangle
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
            GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
            DrawRectangle(350, 130, 300, 200, ahccs_colors.dark_blue);

            // Title
            GuiSetFont(ahccs_fonts.bold_font);
            GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
            GuiLabel({ 425, 130, 200, 75 }, "Attendee Details:");

            // Details
            GuiSetFont(ahccs_fonts.regular_font);
            GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
            GuiLabel({ 370, 180, 200, 75 }, ("Role: " + active_attendee.role).c_str());
            GuiLabel({ 370, 205, 200, 75 }, ("Total Time Attended: " + active_attendee.total_time).c_str());
            GuiLabel({ 370, 230, 200, 75 }, ("First Join: " + active_attendee.first_join).c_str());
            GuiLabel({ 370, 255, 200, 75 }, ("Last Leave: " + active_attendee.last_leave).c_str());

        }

        // back  button
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.yellow));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        if (GuiButton({ 100,510,150,40 }, "Back")) control_vars.selected_report = -1;

        // Delete report button
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(RED));
        GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(MAROON));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));

        if (GuiButton({ 100, 450, 150, 40 }, "Delete Report") && data.courses[control_vars.selected_course].get_num_reports_all() != 0) {

            remove(active_course.get_all_reports()[control_vars.selected_report].file_path.c_str());
            control_vars.selected_report = -1;

            data.refresh_data(control_vars);

        }
        
        /*
        // Delete all reports for this course
        if (GuiButton({ 50, 540, 150, 40 }, "Delete All")) {
            for (Report report : active_course.get_all_reports()) {
                std::remove(report.file_path.c_str());
            }

            data.refresh_data(control_vars);
        }
        */

    }

    ahccs_colors.set_AHCCS_style();
}

void draw_students_view(Course active_course) {
    // set styling
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.lighter_blue));
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(ahccs_colors.lighter_blue));
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(ahccs_colors.lighter_blue));

    // Students list and label
    control_vars.selected_student = GuiListView({ 50, 130, 250, 300 }, control_vars.students_list.c_str(), &control_vars.students_scroll, control_vars.selected_student);
    GuiLabel({ 50, 80, 150, 75 }, "Students:");

    // If there are no reports for the selected quarter, inform the user
    if (active_course.get_num_reports(control_vars.selected_quarter) == 0) {
        GuiSetFont(ahccs_fonts.bold_font);
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(RED));
        GuiLabel({ 50, 450, 150, 75 }, "No live class reports found for this quarter");
        GuiSetFont(ahccs_fonts.regular_font);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
    }

    if (active_course.get_num_students() != 0 && active_course.get_num_reports(control_vars.selected_quarter) != 0) {
        // Export Student Data button
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.yellow));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        if (GuiButton({ 50,440,250,40 }, "Export Student Data")) {

            data.export_data(control_vars);

        }
    }

    // Student details
    if (active_course.get_num_students() != 0 && control_vars.selected_student != -1 && active_course.get_num_reports(control_vars.selected_quarter) != 0) {
        
        Student& active_student = active_course.students[control_vars.selected_student];

        // Rectangle
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
        DrawRectangle(350, 130, 500, 300, ahccs_colors.dark_blue);

        // Title
        GuiSetFont(ahccs_fonts.bold_font);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
        GuiLabel({ 425, 130, 100, 75 }, "Student Details:");
        GuiLabel({ 630, 130, 100, 75 }, "Attendance Chart:");

        // Details
        GuiSetFont(ahccs_fonts.regular_font);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 22);

        int num_classes_quarter_section = active_course.get_num_reports(control_vars.selected_quarter); 
        int num_excuses = active_student.excuses[control_vars.selected_quarter];

        std::string LCA_label = "Live Classes Attended: " + std::to_string(active_student.LCAs[control_vars.selected_quarter]) + '/' + std::to_string(num_classes_quarter_section);
        GuiLabel({ 370, 200, 200, 25 }, LCA_label.c_str());

        GuiLabel({ 370, 240, 200, 25 }, "Excused Days: ");
        if (GuiValueBox({ 500, 240, 50, 25 }, NULL, &num_excuses, 0, 1000, &control_vars.excuses_editmode)) {
            control_vars.excuses_editmode = !control_vars.excuses_editmode;
        }


        // If the number of excuses is being editing, change the value in the data
        if (num_excuses != data.courses[control_vars.selected_course].students[control_vars.selected_student].excuses[control_vars.selected_quarter]) {
            data.courses[control_vars.selected_course].students[control_vars.selected_student].excuses[control_vars.selected_quarter] = num_excuses;
            data.save_userdata_json();
        }

        int adjusted_total = active_student.get_adjusted_total(control_vars.selected_quarter);
        std::string adjusted_total_label = "Adjusted Total: " + std::to_string(adjusted_total) + '/' + std::to_string(num_classes_quarter_section);
        GuiLabel({ 370, 280, 200, 25 }, adjusted_total_label.c_str());

        int percentage = active_student.get_percentage(num_classes_quarter_section, control_vars.selected_quarter);
        std::string percentage_label = "Percentage: " + std::to_string(percentage) + '%';
        GuiLabel({ 370, 320, 200, 25 }, percentage_label.c_str());

        GuiPieGraph(620, 200, percentage,  100 - percentage, "Attended", "Missed");


        

    }

    // Quarters dropdown
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
    GuiLabel({ 450, 60, 50, 30 }, "Quarter: ");

    // Draw the box and change the mode if it is pressed
    if (control_vars.quarters_editmode) {
        GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
    }
    else {
        GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
    }
    if (GuiDropdownBox({ 530, 60, 300, 30 }, control_vars.quarters_list.c_str(), &control_vars.selected_quarter, control_vars.quarters_editmode)) {

        // Data will have to be updated with new quarter selection
        control_vars.quarters_editmode = !control_vars.quarters_editmode;
        data.refresh_data(control_vars);
    }

    ahccs_colors.set_AHCCS_style();
}

void draw_details_view(Course active_course) {
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
    control_vars.selected_detail = GuiListView({ 50, 125, 150, 300 }, "Course Details;Reports Graph", &control_vars.details_scroll, control_vars.selected_detail);

    if (control_vars.selected_detail == 0) {
        // Rectangle
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
        DrawRectangle(250, 130, 300, 200, ahccs_colors.dark_blue);

        // Title
        GuiSetFont(ahccs_fonts.bold_font);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
        GuiLabel({ 325, 130, 200, 75 }, "Course Details:");

        // Details
        GuiSetFont(ahccs_fonts.regular_font);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
        GuiLabel({ 270, 180, 200, 75 }, ("Number of Reports: " + std::to_string(active_course.get_num_reports_all())).c_str());
        GuiLabel({ 270, 205, 200, 75 }, ("Number of Students: " + std::to_string(active_course.get_num_students())).c_str());

        // Delete Course button
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(RED));
        GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(MAROON));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));

        if (GuiButton({ 325, 340, 150, 40 }, "Delete Course")) {
            data.delete_course(control_vars);
        }
        ahccs_colors.set_AHCCS_style();
    } else if (control_vars.selected_detail == 1) {
        GuiReportsGraph(ahccs_colors, ahccs_fonts, active_course.get_all_reports(true), 250, 125, control_vars.range_offset, control_vars.zoom_level);
    }

}


int main()
{
    data.grading_periods.get_grading_periods();
    control_vars.create_quarters_list(data.grading_periods);
    data.load_userdata_json(control_vars);

    time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm* parts = std::localtime(&time);
    control_vars.calendar_month = parts->tm_mon+1;
    control_vars.calendar_year = parts->tm_year+1900;

    // Create the window 
    InitWindow(control_vars.screenWidth, control_vars.screenHeight, "Attendance Calculator");
    SetTargetFPS(30);

    // Load in and set the fonts
    ahccs_fonts.regular_font = LoadFont("Resources/fonts/OpenSans-Regular.ttf");
    ahccs_fonts.bold_font = LoadFont("Resources/fonts/OpenSans-Bold.ttf");
    SetTextureFilter(ahccs_fonts.regular_font.texture, FILTER_BILINEAR);
    SetTextureFilter(ahccs_fonts.bold_font.texture, FILTER_BILINEAR);
    GuiSetFont(ahccs_fonts.regular_font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 22);

    // Load in the Achievement House logo and convert it to a texture for drawing
    Image logo = LoadImage("Resources/logo.png");
    Texture2D logo_tex = LoadTextureFromImage(logo);

    // Color each of the styles
    ahccs_colors.set_AHCCS_style();

    // Unload all resources that can be unloaded
    UnloadImage(logo);

    // Calculate and refresh data for the first time
    data.refresh_data(control_vars);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        BeginDrawing();

        // Draw the background color
        ClearBackground(ahccs_colors.white);

        // Draw the header bar with the logo
        DrawRectangle(0, 0, 900, 50, ahccs_colors.dark_blue);
        DrawTexture(logo_tex, 20, 7, ahccs_colors.white);

        // "Add course" button
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.yellow));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));

        if (GuiButton({ 200, 10, 200, 30 }, "#8#Add Course")) {
            control_vars.addcourse_label = "Course Name:";
            control_vars.show_addcourse_box = true;
            control_vars.selected_student = -1;
        }
        ahccs_colors.set_AHCCS_style();

        // Body
        if (data.courses.size() != 0) {
            Course& active_course = data.courses[control_vars.selected_course];

            if (control_vars.selected_view == control_vars.REPORTS) {

                draw_reports_view(active_course);
            

            } else if (control_vars.selected_view == control_vars.STUDENTS) {

                draw_students_view(active_course);

            } else if (control_vars.selected_view == control_vars.COURSE_DETAILS) {
                
                draw_details_view(active_course);

            }


            // Views dropdown
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
            GuiLabel({ 50, 60, 50, 30 }, "View: ");

            // Draw the box and change the mode if it is pressed
            if (control_vars.views_editmode) {
                GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
            }
            else {
                GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
            }
            if (GuiDropdownBox({ 100, 60, 300, 30 }, "Students;Reports;Course Details", &control_vars.selected_view, control_vars.views_editmode)) {
                control_vars.views_editmode = !control_vars.views_editmode;
                data.refresh_data(control_vars);
            }
            

        }
        else {
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(RED));
            GuiLabel({ 350, 100, 100, 50 }, "No Course Selected");
            ahccs_colors.set_AHCCS_style();
        }

        // Course Selector
        // Checks if the dropdown is in the dropped down state
        if (control_vars.course_editmode) {
            GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
        }
        else {
            GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.white));
        }

        // Draw the box and change the mode if it is pressed
        if (GuiDropdownBox({ 400,10, 480, 30 }, control_vars.course_list.c_str(), &control_vars.selected_course, control_vars.course_editmode)) {
            control_vars.course_editmode = !control_vars.course_editmode;
            data.refresh_data(control_vars);
        }

        // Add course popup
        if (control_vars.show_addcourse_box) {

            // Set the styles for this text box
            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ahccs_colors.dark_blue));
            GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(ahccs_colors.dark_blue));
            GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(ahccs_colors.dark_blue));
            GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ahccs_colors.yellow));
            GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(ahccs_colors.white));
            GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(ahccs_colors.white));
            DrawRectangle(0, 0, control_vars.screenWidth, control_vars.screenHeight, Fade(ahccs_colors.white, 0.8f));

            int result = GuiTextInputBox({ control_vars.screenWidth / 2.0f - 175, control_vars.screenHeight / 2.0f - 75, 350, 150 }, "Add New Course", control_vars.addcourse_label.c_str(), "Add;Cancel", control_vars.newcourse_name);

            if (result == 1)
            {
                data.add_course(control_vars);
            }
            else if (result == 0 || result == 2) {
                // Close the window
                control_vars.show_addcourse_box = false;
            }

            ahccs_colors.set_AHCCS_style(); // Go back to normal styling

        }




        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
    
    return 0;
}