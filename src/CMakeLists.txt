add_executable(Attendance_Calculator main.cpp attendance_course.cpp attendance_data.cpp attendance_dates.cpp attendance_filesystem.cpp attendance_graphs.cpp attendance_reports.cpp attendance_ui.cpp attendance_utils.cpp attendance_calendar.cpp)

if(MINGW)
  target_link_libraries(Attendance_Calculator ws2_32)
endif()

target_link_libraries(Attendance_Calculator raylib)
