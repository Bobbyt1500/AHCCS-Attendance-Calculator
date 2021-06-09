#ifndef ATTENDANCE_REPORTS
#define ATTENDANCE_REPORTS

#include <vector>
#include <string>
#include <map>

#include "attendance_dates.h"

class Report {
public:
	class Attendee {
	public:
		std::string name;
		std::string first_join;
		std::string last_leave;
		std::string role;
		std::string total_time;
	};

	std::vector<Report::Attendee> attendees;
	std::string file_path;
	std::vector<std::map<std::string, std::string>> data;
	Date get_report_date();
	void get_attendees();
	int get_num_attendees();
	int get_num_participants();
	bool in_daterange(DateRange date_range);
};

#endif
