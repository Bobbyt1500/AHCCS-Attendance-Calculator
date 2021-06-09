#ifndef ATTENDANCE_COURSE
#define ATTENDANCE_COURSE

#include <string>
#include <vector>

#include "attendance_reports.h"

class Student {
public:
	std::string name;
	std::vector<int> excuses = { 0,0,0,0,0,0,0,0 };
	std::vector<int> LCAs = { 0,0,0,0,0,0,0,0 };
	int get_adjusted_total(int selected_quarter);
	int get_percentage(int num_classes, int selected_quarter);
};

class Course {
public:
	GradingPeriods grading_periods;

	std::vector<Student> students;
	std::vector<std::vector<Report>> reports;
	std::string name;

	std::vector<Report> get_all_reports(bool increasing = false);
	void sort_reports();
	void sort_students();
	int get_num_reports(int selected_quarter);
	int get_num_reports_all();
	int get_num_students();
	void update_students();
	void update_student_LCAs();
	void load_reports();
};
#endif