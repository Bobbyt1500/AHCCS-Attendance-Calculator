#ifndef ATTENDANCE_DATA
#define ATTENDANCE_DATA

#include <vector>

#include "json.hpp"

#include "attendance_dates.h"
#include "attendance_course.h"
#include "attendance_ui.h"

class Calculator_Data {
public:
	std::vector<Course> courses;
	nlohmann::json json_data;
	GradingPeriods grading_periods;

	void refresh_data(Control_Variables& control_vars);

	void import_reports(Control_Variables& control_vars);
	
	void export_data(Control_Variables& control_vars);

	void add_course(Control_Variables& control_vars);
	void delete_course(Control_Variables& control_vars);

	void load_userdata_json(Control_Variables& control_vars);
	void save_userdata_json();
};

#endif

