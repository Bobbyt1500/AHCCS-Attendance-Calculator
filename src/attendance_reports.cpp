#include "attendance_reports.h"

#include <map>
#include <string>
#include <vector>

#include "attendance_dates.h"

Date Report::get_report_date() {

    Date date;

    bool moderator = false;

    for (std::map<std::string, std::string> row : data) {
        // Get the first join date of a moderator
        if (row["Role"] == "Moderator") {

            moderator = true;
            int space_pos = row["\"First join\""].find(' ');
            date.from_string(row["\"First join\""].substr(1, space_pos - 1)); // NOTE: must remove the first " in the value
            break;
        }
    }

    // If there was no mod take the date of the first user
    if (!moderator) {
        int space_pos = data[0]["\"First join\""].find(' ');
        date.from_string(data[0]["\"First join\""].substr(1, space_pos - 1));
    }

    return date;

}

void Report::get_attendees() {
    attendees = {};
    for (std::map<std::string, std::string> row : data) {
        Attendee new_attendee;
        // NOTE: After accessing the dating, it is substringed to remove the leading and trailing '"'s
        new_attendee.name = row["Name"].substr(1, row["Name"].size() - 2);
        new_attendee.first_join = row["\"First join\""].substr(1, row["\"First join\""].size() - 2);
        new_attendee.last_leave = row["\"Last leave\""].substr(1, row["\"Last leave\""].size() - 2);
        new_attendee.role = row["Role"];
        new_attendee.total_time = row["\"Total time\""];

        attendees.push_back(new_attendee);
    }
}

int Report::get_num_attendees() {
    return attendees.size();
}

int Report::get_num_participants() {
    int count = 0;
    for (Attendee attendee : attendees) {
        if (attendee.role == "Participant") ++count;
    }
    return count;
}

bool Report::in_daterange(DateRange date_range) {
    Date date = get_report_date();

    // Convert each date to a numeric value to compare
    int begin_value = (10000 * date_range.begin.year) + (100 * date_range.begin.month) + date_range.begin.day;
    int end_value = (10000 * date_range.end.year) + (100 * date_range.end.month) + date_range.end.day;
    int report_value = (10000 * date.year) + (100 * date.month) + date.day;

    return report_value >= begin_value && report_value <= end_value ? true : false;
}
