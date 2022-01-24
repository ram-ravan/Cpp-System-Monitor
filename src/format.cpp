#include <string>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long uptime) {
    long minutes, hours, seconds;
    string elapsed_time;
    hours = uptime / 3600;
    uptime %= 3600;
    minutes = uptime / 60;
    uptime %= 60;
    seconds = uptime;

    std::ostringstream stream;
    stream << std::setw(2) << std::setfill('0') << hours << ":" 
     << std::setw(2) << std::setfill('0') << minutes << ":"
     << std::setw(2) << std::setfill('0') << seconds;
  return stream.str();

    // elapsed_time = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
    // return elapsed_time;
}