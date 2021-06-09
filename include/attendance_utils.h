#ifndef ATTENDANCE_UTILITIES
#define ATTENDANCE_UTILITIES
#include <string>
#include <vector>
#include <map>

#include "json.hpp"

#include "attendance_course.h"

std::vector<std::map<std::string, std::string>> parse_csv(std::string filename);

std::map<std::string, std::string> format_attendee_data(std::map<std::string, std::string> data_row);

#endif