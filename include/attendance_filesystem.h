#ifndef ATTENDANCE_FILESYSTEM
#define ATTENDANCE_FILESYSTEM

#include <string>
#include <vector>
#include <map>
#include "json.hpp"

std::vector<std::string> open_csv_files();

std::string open_csv_file();

std::string get_csv_save_path();

std::string get_xlsx_save_path();

std::vector<std::string> get_files(std::string directory);

nlohmann::json load_json(std::string filepath);

void save_json(std::string filepath, nlohmann::json j);

void copyfile(std::string existing_path, std::string destination_path);

#endif
