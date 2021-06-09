#ifndef ATTENDANCE_UI
#define ATTENDANCE_UI

#include <string>
#include <vector>

#include "raylib.h"
#include "raygui.h"

#include "attendance_reports.h"
#include "attendance_dates.h"
#include "attendance_course.h"

class AHCCS_Colors {
public:
    Color yellow = { 255, 213, 0, 255 };
    Color dark_blue = { 0, 72, 116, 255 };
    Color lighter_blue = { 0, 128, 178, 255 };
    Color white = { 255, 255, 255, 255 };

    void set_AHCCS_style() {
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(dark_blue));
        GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(lighter_blue));
        GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(dark_blue));
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(dark_blue));
        GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(dark_blue));
        GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(dark_blue));
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(white));
        GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(white));
        GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(white));
    }


};

class AHCCS_Fonts {
public:
    Font regular_font;
    Font bold_font;
};

class Control_Variables {
public:
    enum View {
        STUDENTS,
        REPORTS,
        COURSE_DETAILS
    };

    std::string addcourse_label = "Course Name: ";

    // Window bounds
    const int screenWidth = 900;
    const int screenHeight = 600;

    // views dropdown
    int selected_view = 0;
    bool views_editmode = false;

    // Quarters dropdown
    int selected_quarter = 0;
    bool quarters_editmode = false;
    std::string quarters_list;

    // Course dropdown
    int selected_course = 0;
    bool course_editmode = false;
    std::string course_list;

    // Add course dialog
    char newcourse_name[256];
    bool show_addcourse_box = false;

    // Reports
    int selected_report = -1;

    // Attendees list
    int attendees_scroll = 0;
    int selected_attendee = 0;
    std::string attendees_list;

    // Students list
    int students_scroll = 0;
    int selected_student = 0;
    std::string students_list;

    // Excuses value box
    bool excuses_editmode = false;

    // Course details list
    int details_scroll = 0;
    int selected_detail = 0;

    // Participants graph
    int range_offset = 0;
    int zoom_level = 2;

    // Reports calendar
    int calendar_month;
    int calendar_year;

    void create_attendees_list(Report report);

    void create_students_list(std::vector<Student> students);

    void create_courses_list(std::vector<Course> courses);

    void create_quarters_list(GradingPeriods grading_periods);

};

#endif