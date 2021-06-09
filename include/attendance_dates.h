#ifndef ATTENDANCE_DATES
#define ATTENDANCE_DATES
#include <string>
#include <vector>
#include <map>

class Date {
public:
	int day;
	int month;
	int year;
	void from_string(std::string date_str);
	std::string to_string();
};

class DateRange {
public:
	Date begin;
	Date end;
	void from_string(std::string daterng_str);
	std::string to_string();
};

class GradingPeriods {
public:
	void get_grading_periods();
	// Q1, Q2...
	std::vector<DateRange> quarters;
	// Q1A, Q1B, Q2A, Q2B...
	std::vector<DateRange> quarter_sections;

	std::vector<std::string> quarter_section_names = { "Q1A", "Q1B", "Q2A", "Q2B", "Q3A", "Q3B", "Q4A", "Q4B" };
};

#endif
