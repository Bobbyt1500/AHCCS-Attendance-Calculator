#include "attendance_ui.h"

#include <string>
#include <vector>

#include "attendance_course.h"
#include "attendance_reports.h"
#include "attendance_dates.h"

void Control_Variables::create_attendees_list(Report report) {
    bool first = true;
    attendees_list = "";
    // Create list of attendees seperated by ';'
    for (Report::Attendee attendee : report.attendees) {
        if (first) {
            attendees_list.append(attendee.name);
            first = false;
            continue;
        }
        else {
            attendees_list.append(';' + attendee.name);
        }
    }
}

void Control_Variables::create_students_list(std::vector<Student> students) {
    bool first = true;
    students_list = "";
    // Create list of students seperated by ';'
    for (Student student : students) {

        if (first) {
            students_list.append(student.name);
            first = false;
            continue;
        }
        else {
            students_list.append(';' + student.name);
        }
    }
}

void Control_Variables::create_courses_list(std::vector<Course> courses) {
    bool first = true;
    course_list = "";
    for (Course course : courses) {

        if (first) {
            course_list.append(course.name);
            first = false;
            continue;
        }
        else {
            course_list.append(';' + course.name);
        }
    }
}

void Control_Variables::create_quarters_list(GradingPeriods grading_periods) {
    bool first = true;
    quarters_list = "";

    // Create a list of quarter sections seperated by ';'
    for (int i = 0; i < grading_periods.quarter_sections.size(); ++i) {

        if (first) {
            quarters_list.append(grading_periods.quarter_section_names[i] + ' ' + grading_periods.quarter_sections[i].to_string());
            first = false;
        }
        else {
            quarters_list.append(';' + grading_periods.quarter_section_names[i] + ' ' + grading_periods.quarter_sections[i].to_string());
        }
    }
}