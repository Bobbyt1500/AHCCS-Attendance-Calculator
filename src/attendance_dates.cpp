#include "attendance_dates.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "httplib.h"
#include "SimpleIni.h"

void Date::from_string(std::string date_str) {
    int pos1 = date_str.find('/');
    int pos2 = date_str.find('/', pos1 + 1);

    month = std::stoi(date_str.substr(0, pos1));
    day = std::stoi(date_str.substr(pos1 + 1, pos2 - (pos1 + 1)));
    year = std::stoi(date_str.substr(pos2 + 1));
}

std::string Date::to_string() {
    return std::to_string(month) + '/' + std::to_string(day) + '/' + std::to_string(year);
}

void DateRange::from_string(std::string daterng_str) {
    int pos = daterng_str.find('-');
    begin.from_string(daterng_str.substr(0, pos));
    end.from_string(daterng_str.substr(pos + 1));
}

std::string DateRange::to_string() {
    return begin.to_string() + '-' + end.to_string();
}

void GradingPeriods::get_grading_periods() {

    // Get grading periods data from cloud storage
    httplib::Client cli("http://71.185.235.96:9076");
    httplib::Result res = cli.Get("/grading_periods.ini");

    // Parse ini file
    CSimpleIniA ini;
    ini.LoadData(res->body);

    std::string keys[4] = { "QUARTERONE", "QUARTERTWO", "QUARTERTHREE", "QUARTERFOUR" };

    for (int i = 0; i < 4; ++i) {
        DateRange quarter;
        quarter.from_string(ini.GetValue("QUARTERS", keys[i].c_str()));
        quarters.push_back(quarter);

        // Quarter section A; example: Q1A
        DateRange quarter_sectionA;

        quarter_sectionA.begin = quarters[i].begin;
        quarter_sectionA.end.from_string(ini.GetValue("MIDQUARTER_DEADLINES", keys[i].c_str()));

        quarter_sections.push_back(quarter_sectionA);

        // Quarter section B; example: Q1B
        DateRange quarter_sectionB;

        quarter_sectionB.begin.from_string(ini.GetValue("MIDQUARTER_DEADLINES", keys[i].c_str()));
        // Add 1 to the begin day for the 'B' section since it starts 1 day after the midqaurter deadline
        ++quarter_sectionB.begin.day;
        quarter_sectionB.end = quarters[i].end;

        quarter_sections.push_back(quarter_sectionB);

    }

}