#include <string>

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

    elapsed_time = to_string(hours) + ":" + to_string(minutes) + ":" + to_string(seconds);
    return elapsed_time;
}