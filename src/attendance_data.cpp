#include "attendance_data.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "attendance_filesystem.h"
#include "attendance_course.h"
#include "attendance_utils.h"
#include "attendance_ui.h"

void Calculator_Data::refresh_data(Control_Variables &control_vars) {

    // Only if there is a currently loaded course
    if (courses.size() != 0) {

        courses[control_vars.selected_course].grading_periods = grading_periods;

        // Load in reports from the filesystem
        courses[control_vars.selected_course].load_reports();

        courses[control_vars.selected_course].sort_reports();

        courses[control_vars.selected_course].update_students();
        courses[control_vars.selected_course].update_student_LCAs();

        courses[control_vars.selected_course].sort_students();

        control_vars.create_students_list(courses[control_vars.selected_course].students);

        // Update the graph control variables
        control_vars.range_offset = 0;

    }

    control_vars.create_courses_list(courses);

}

void Calculator_Data::import_reports(Control_Variables& control_vars) {

    // Get the exisiting report dates
    std::map<std::string, Date> existing_dates; // Map each filepath to its date

    for (Report report : courses[control_vars.selected_course].get_all_reports()) {

        existing_dates[report.file_path] = report.get_report_date();

    }

    // Get vector of opened file paths
    std::vector<std::string> file_paths = open_csv_files();

    // If there are no new files
    if (file_paths.size() == 0) return;

    for (std::string file_path : file_paths) {
        Report new_report;
        new_report.data = parse_csv(file_path);


        Date date = new_report.get_report_date();

        // Compare and overwrite duplicates
        bool duplicated = false;
        for (std::map<std::string, Date>::iterator it = existing_dates.begin(); it != existing_dates.end(); ++it) {

            if (it->second.day == date.day && it->second.month == date.month && it->second.year == date.year) {
                copyfile(file_path, it->first);
                duplicated = true;
                break;
            }

        }

        // Add the report to the courses folder
        if (!duplicated) {
            int slash_pos = file_path.rfind('\\');
            std::string new_file_name = file_path.substr(slash_pos + 1);
            std::string new_file_path = "Courses/" + courses[control_vars.selected_course].name + '/' + new_file_name;
            copyfile(file_path, new_file_path);
        }

    }

    // Refresh data since there are now new reports
    refresh_data(control_vars);
    save_userdata_json();
}

// Export the quarter_section specific data to a file that the user defines
void Calculator_Data::export_data(Control_Variables &control_vars) {

    Course active_course = courses[control_vars.selected_course];
    int selected_quarter = control_vars.selected_quarter;
    std::string quarter_section_name = active_course.grading_periods.quarter_section_names[selected_quarter];

    std::string file_path = get_csv_save_path();

    // If the user did not define a proper file path
    if (file_path == "") return;

    file_path.append(".csv");

    std::ofstream csv_file;
    csv_file.open(file_path);
    // Write title
    csv_file << quarter_section_name << " Attendance Report\n";
    // Write column headers
    csv_file << "Name,LCA,Excuses,AT,Percentage\n";

    for (int i = 0; i < active_course.get_num_students(); ++i) {

        Student student = active_course.students[i];


        // LCA / Total Class Reports
        std::string formatted_LCA = std::to_string(student.LCAs[selected_quarter]) + "/" + std::to_string(active_course.get_num_reports(selected_quarter));

        // AT / Total Class Reports
        std::string formatted_AT = std::to_string(student.get_adjusted_total(selected_quarter)) + "/" + std::to_string(active_course.get_num_reports(selected_quarter));

        std::string formatted_percentage = std::to_string(student.get_percentage(active_course.get_num_reports(selected_quarter), selected_quarter)) + '%';

        csv_file << student.name << ',' << formatted_LCA << ',' << student.excuses[selected_quarter] << ',' << formatted_AT << ',' << formatted_percentage << '\n';

    }

    csv_file.close();


/*
    Old way using libxlsxwriter
    Course active_course = courses[control_vars.selected_course];
    std::string quarter_section_name = active_course.grading_periods.quarter_section_names[control_vars.selected_quarter];

    std::string file_path = get_xlsx_save_path();

    // If the user did not define a proper file path
    if (file_path == "") return;

    file_path.append(".xlsx");

    lxw_workbook* workbook = new_workbook(file_path.c_str());

    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, (quarter_section_name + " Attendance Data").c_str());


    lxw_format* bold = workbook_add_format(workbook);
    format_set_bold(bold);

    // Write column headers

    worksheet_write_string(worksheet, 0, 0, "Name", bold);
    worksheet_write_string(worksheet, 0, 1, "LCA", bold);
    worksheet_write_string(worksheet, 0, 2, "Excuses", bold);
    worksheet_write_string(worksheet, 0, 3, "AT", bold);
    worksheet_write_string(worksheet, 0, 4, "Percentage", bold);

    worksheet_set_column(worksheet, 0, 0, 32, NULL);
    worksheet_set_column(worksheet, 4, 4, 11, NULL);

    int selected_quarter = control_vars.selected_quarter;

    // Write rows
    for (int i = 0; i < active_course.get_num_students(); ++i) {

        Student student = active_course.students[i];


        // LCA / Total Class Reports
        std::string formatted_LCA = std::to_string(student.LCAs[selected_quarter]) + "/" + std::to_string(active_course.get_num_reports(selected_quarter));

        // AT / Total Class Reports
        std::string formatted_AT = std::to_string(student.get_adjusted_total(selected_quarter)) + "/" + std::to_string(active_course.get_num_reports(selected_quarter));

        std::string formatted_percentage = std::to_string(student.get_percentage(active_course.get_num_reports(selected_quarter), selected_quarter)) + '%';

        worksheet_write_string(worksheet, i + 1, 0, student.name.c_str(), NULL);
        worksheet_write_string(worksheet, i + 1, 1, formatted_LCA.c_str(), NULL);
        worksheet_write_number(worksheet, i + 1, 2, student.excuses[selected_quarter], NULL);
        worksheet_write_string(worksheet, i + 1, 3, formatted_AT.c_str(), NULL);
        worksheet_write_string(worksheet, i + 1, 4, formatted_percentage.c_str(), NULL);

    }

    workbook_close(workbook);
*/
}

void Calculator_Data::add_course(Control_Variables &control_vars) {
    // If this is a duplicate course name, inform the user
    bool duplicate_course = false;
    for (Course existing_course : courses) {
        if (existing_course.name == control_vars.newcourse_name) {
            control_vars.addcourse_label = "Course Name Already Exists";
            duplicate_course = true;
        }
    }
    if (!duplicate_course) {
        Course newcourse;
        newcourse.name = control_vars.newcourse_name;
        newcourse.grading_periods = grading_periods;
        // Create a new directory for the course
        std::string newfilepath = "Courses/" + std::string(newcourse.name);
        _wmkdir(std::wstring(newfilepath.begin(), newfilepath.end()).c_str());

        courses.push_back(newcourse);
        save_userdata_json();

        // Close window and set as active course
        control_vars.selected_course = courses.size() - 1;
        control_vars.show_addcourse_box = false;
    }

    refresh_data(control_vars);
}

void Calculator_Data::delete_course(Control_Variables& control_vars) {
    for (Report report : courses[control_vars.selected_course].get_all_reports()) {
        std::remove(report.file_path.c_str());
    }

    std::string directory_path = "Courses/" + courses[control_vars.selected_course].name;
    std::wstring wdirectory_path(directory_path.begin(), directory_path.end());
    _wrmdir(wdirectory_path.c_str());

    courses.erase(courses.begin() + control_vars.selected_course);
    control_vars.selected_course = 0;

    refresh_data(control_vars);
    save_userdata_json();
}

void Calculator_Data::load_userdata_json(Control_Variables& control_vars) {
    // Load in the configured courses for this user
    json_data = load_json("userdata.json");
    for (std::string course_name : json_data["courses"].get<std::vector<std::string>>()) {
        Course loaded_course;
        loaded_course.name = course_name;
        loaded_course.grading_periods = grading_periods;
        courses.push_back(loaded_course);
    }

    // Load in the saved excuses data
    std::map<std::string, std::map<std::string, std::vector<int>>> excuses_from_json = json_data["excuses"].get<std::map<std::string, std::map<std::string, std::vector<int>>>>();
    for (int i = 0; i < courses.size(); ++i) {
        courses[i].load_reports();
        courses[i].update_students();
        for (int j = 0; j < courses[i].get_num_students(); ++j) {
            std::vector<int> student_excuses = excuses_from_json[courses[i].name][courses[i].students[j].name];
            courses[i].students[j].excuses = student_excuses;
        }
    }
}

void Calculator_Data::save_userdata_json() {
    std::vector<std::string> course_names;
    std::map<std::string, std::map<std::string, std::vector<int>>> excuses_map;

    // Put the data in a format that can be saved to json
    for (Course course : courses) {
        course_names.push_back(course.name);
        for (Student student : course.students) {
            excuses_map[course.name][student.name] = student.excuses;
        }
    }

    json_data["courses"] = course_names;
    json_data["excuses"] = excuses_map;
    save_json("userdata.json", json_data);
}

