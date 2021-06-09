#include "attendance_utils.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>

#include "attendance_filesystem.h"


std::vector<std::map<std::string, std::string>> parse_csv(std::string filename) {
    /*
    Returned data is a vector of rows
    Each row is a map mapping a column to its value
    */
    std::ifstream input_file(filename);
    std::vector<std::map<std::string, std::string>> data;
    std::string line;

    // The columns that will be used to describe values in the map
    std::vector<std::string> columns;

    // Read in the columns
    if (std::getline(input_file, line)) {

        if (line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') {
            line.erase(0, 3);
        }

        std::string column_name;
        std::istringstream iss(line);

        while (std::getline(iss, column_name, ',')) {
            columns.push_back(column_name);
        }

    }

    // Read in the values
    while (true) {
        if (std::getline(input_file, line)) {

            std::string cell;
            int count = 0;
            std::map<std::string, std::string> row;

            std::istringstream iss(line);
            while (std::getline(iss, cell, ',')) {

                row[columns[count]] = cell;
                ++count;
            }

            data.push_back(row);

        }
        else if (input_file.eof()) {
            break;
        }
    }

    input_file.close();
    return data;
}

// Get the role, first join, last leave, and total time attended for this attendee
std::map<std::string, std::string> format_attendee_data(std::map<std::string, std::string> data_row) {
    std::map<std::string, std::string> attendee_data;

    std::string first_join = data_row["\"First join\""].substr(1, data_row["\"First join\""].size() - 2);
    std::string last_leave = data_row["\"Last leave\""].substr(1, data_row["\"Last leave\""].size() - 2);

    attendee_data["first_join"] = first_join;
    attendee_data["last_leave"] = last_leave;
    attendee_data["role"] = data_row["Role"];
    attendee_data["total_time"] = data_row["\"Total time\""];

    return attendee_data;

}