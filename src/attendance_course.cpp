#include "attendance_course.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "attendance_dates.h"
#include "attendance_utils.h"
#include "attendance_filesystem.h"

int Student::get_adjusted_total(int selected_quarter_section) {
    return LCAs[selected_quarter_section] + excuses[selected_quarter_section];
}
int Student::get_percentage(int num_classes, int selected_quarter_section) {
    return (double(get_adjusted_total(selected_quarter_section)) / double(num_classes)) * 100;
}

bool report_date_compare(Report report1, Report report2) {
    Date date1 = report1.get_report_date();
    Date date2 = report2.get_report_date();

    // Convert each date to a numeric value to compare

    int value1 = (10000 * date1.year) + (100 * date1.month) + date1.day;
    int value2 = (10000 * date2.year) + (100 * date2.month) + date2.day;

    return value1 > value2 ? true : false;

}

bool student_lastname_compare(Student student1, Student student2) {

    std::string last_name1 = student1.name.substr(student1.name.find(' ') + 1);
    std::string last_name2 = student2.name.substr(student2.name.find(' ') + 1);

    return std::strcmp(last_name1.c_str(), last_name2.c_str()) == 1 ? false : true;

}

std::vector<Report> Course::get_all_reports(bool increasing) {
    std::vector<Report> all_reports = {};


    for (std::vector<Report> quarter_reports : reports) {
        all_reports.insert(all_reports.begin(), quarter_reports.begin(), quarter_reports.end());
    }

    if (increasing) {
        std::reverse(all_reports.begin(), all_reports.end());
    }

    
    return all_reports;
}


void Course::sort_reports() {
    for (int i = 0; i < grading_periods.quarter_sections.size(); ++i) {
        std::sort(reports[i].begin(), reports[i].end(), report_date_compare);
    }
}

void Course::sort_students() {
    std::sort(students.begin(), students.end(), student_lastname_compare);
}

//TODO: Update to getting from a schoology roster instead of live class reports
void Course::update_students() {
    std::vector<std::string> student_names;
    for (Report report : get_all_reports()) {

        for (std::map<std::string, std::string> row : report.data) {

            // Skip all moderators
            if (row["Role"] == "Moderator") continue;

            // Name without leading and trailing '"'
            std::string name = row["Name"].substr(1, row["Name"].length() - 2);
            student_names.push_back(name);

            // Skip students that are already in the course
            bool existing_student = false;
            for (Student student : students) {
                if (name == student.name) {
                    existing_student = true;
                }
            }

            if (existing_student) continue;

            // Add student to course's students vector
            Student new_student;
            new_student.name = name;

            students.push_back(new_student);
        }
    }

    // Remove any students that are in the list, but no longer in reports
    std::vector<Student> to_remove;
    
    for (Student student : students) {
        std::vector<std::string>::iterator it;
        it = std::find(student_names.begin(), student_names.end(), student.name);

        if (it == student_names.end()) {
            to_remove.push_back(student);
        }
    }

    for (Student student : to_remove) {
        for (std::vector<Student>::iterator it = students.begin(); it != students.end(); ++it) {
            if (it->name == student.name) {
                students.erase(it);
                break;
            }
        }
    }

    sort_students();
}

void Course::update_student_LCAs() {
    
    // For each quarter section
    for (int i = 0; i < grading_periods.quarter_sections.size(); ++i) {
        std::vector<Report> quarter_reports = reports[i];

        // Maps students names to their LCA in this quarter
        std::map<std::string, int> student_LCA_map;

        for (Report report : quarter_reports) {
            report.get_attendees();

            for (Report::Attendee attendee : report.attendees) {

                // If attendee not in the map, add it
                if (student_LCA_map.find(attendee.name) == student_LCA_map.end()) {
                    student_LCA_map[attendee.name] = 1;
                }
                else {
                    student_LCA_map[attendee.name] += 1;
                }

            }
        }

        // Update the student objects in the course with the calculated LCAs

        for (int j = 0; j < students.size(); ++j) {
            if (student_LCA_map.find(students[j].name) != student_LCA_map.end()) students[j].LCAs[i] = student_LCA_map[students[j].name];
        }

    }
}

int Course::get_num_reports_all() {
    int count = 0;
    for (std::vector<Report> quarter_reports : reports) {
        count += quarter_reports.size();
    }
    return count;
}

int Course::get_num_reports(int selected_quarter) {
    return reports[selected_quarter].size();
}

int Course::get_num_students() {
    return students.size();
}

void Course::load_reports() {
    reports = {};

    // add empty report vectors for the quarter sections
    for (int i = 0; i < grading_periods.quarter_sections.size(); ++i) {
        std::vector<Report> empty = {};
        reports.push_back(empty);
    }

    std::vector<std::string> found_reports = get_files("Courses/" + name);

    for (std::string found_report : found_reports) {
        Report new_report;
        new_report.file_path = found_report;
        new_report.data = parse_csv(new_report.file_path);

        new_report.get_attendees();

        // Find which quarter this report is for
        for (int i = 0; i < grading_periods.quarter_sections.size(); ++i) {

            if (new_report.in_daterange(grading_periods.quarter_sections[i])) {
                reports[i].push_back(new_report);
                break;
            }

        }

    }
}