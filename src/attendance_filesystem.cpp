#include "attendance_filesystem.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <dirent.h>

#include "json.hpp"

#include "attendance_course.h"

std::vector<std::string> open_csv_files() {
	
    // The open file dialog

    OPENFILENAMEA ofn = { sizeof ofn };
    char files_size[10000]; // Allocating a lot of space so that a large amount of reports can be opened
    files_size[0] = '\0';
    ofn.lpstrFile = files_size;
    ofn.nMaxFile = 10000;
    ofn.lpstrFilter = "CSV\0*.csv;"; // Filter to only csv files
    ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_LONGNAMES | OFN_NOCHANGEDIR; // Allow multiselecting

    GetOpenFileNameA(&ofn);

    // Interpret the opened files and create a vector of the filepaths

    std::vector<std::string> filepaths;
    char* temp = ofn.lpstrFile;
    bool no_filepath = *temp == '\0';

    if (no_filepath) return filepaths;

    // NOTE: If only one file is selected directory is the path of that file
    std::string directory = temp; // Get the directory of the files
    temp = temp + directory.length() + 1;

    // Loop through all of the selected files added their paths to the vector
    while (*temp) {
        std::string filename = temp;
        temp = temp + filename.length() + 1;
        filepaths.push_back(directory + '\\' + filename); 
    }

    // If there are no filepaths, that means directory is the only file and it is a filepath
    if (filepaths.size() == 0) {
        filepaths.push_back(directory);
    }

    return filepaths;

}

std::string open_csv_file() {
    OPENFILENAMEA ofn = { sizeof ofn };
    char file_size[1000];
    file_size[0] = '\0';
    ofn.lpstrFile = file_size;
    ofn.nMaxFile = 1000;
    ofn.lpstrFilter = "CSV\0*.csv;"; // Filter to only csv files
    ofn.Flags = OFN_EXPLORER | OFN_LONGNAMES | OFN_NOCHANGEDIR;

    GetOpenFileNameA(&ofn);

    char* filepath = ofn.lpstrFile;
    bool no_filepath = *filepath == '\0';

    if (no_filepath) {
        return ""; 
    }
    else {
        return filepath;
    }
 

}

std::string get_csv_save_path() {
    
    // The save file dialog

    OPENFILENAMEA sfn = { sizeof sfn };
    char filename_buffer[MAX_PATH] = { 0 };
    sfn.lpstrFile = filename_buffer;
    sfn.nMaxFile = MAX_PATH;
    sfn.lpstrFilter = "CSV (.csv)\0*.csv;"; // Only allow saving as a csv file
    sfn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (!GetSaveFileNameA(&sfn)) return "";

    std::string file_path = sfn.lpstrFile;
    
    return file_path;
}


std::string get_xlsx_save_path() {
    
    // The save file dialog

    OPENFILENAMEA sfn = { sizeof sfn };
    char filename_buffer[MAX_PATH] = { 0 };
    sfn.lpstrFile = filename_buffer;
    sfn.nMaxFile = MAX_PATH;
    sfn.lpstrFilter = "Excel Workbook (.xlsx)\0*.xlsx;"; // Only allow saving as a xlsx file
    sfn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (!GetSaveFileNameA(&sfn)) return "";

    std::string file_path = sfn.lpstrFile;
    
    return file_path;
}

std::vector<std::string> get_files(std::string directory) {
    DIR* dir;
    struct dirent* ent;
    std::vector<std::string> files = {};

    // Open directory and read in csv files to a vector
    if ((dir = opendir(directory.c_str())) != NULL) {

        while ((ent = readdir(dir)) != NULL) {

            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..") {
                files.push_back(directory + '/' + std::string(ent->d_name));
            }

        }

        closedir(dir);
    }

    return files;
}

nlohmann::json load_json(std::string filepath) {
    std::ifstream i(filepath);
    nlohmann::json j;
    i >> j;
    i.close();
    return j;
}

void save_json(std::string filepath, nlohmann::json j) {
    std::ofstream o(filepath);
    o << j << std::endl;
    o.close();
}

void copyfile(std::string existing_path, std::string destination_path) {
    std::ifstream input(existing_path);
    std::ofstream output(destination_path);

    std::string line;
    while (std::getline(input, line)) {
        output << line << std::endl;
    }

    input.close();
    output.close();

}
