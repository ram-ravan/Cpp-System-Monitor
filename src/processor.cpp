#include <string>
#include <fstream>
#include <iostream>
#include<thread>
#include<chrono>

#include "processor.h"
#include "linux_parser.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int iter = 0;
    while(iter < 2) {
        std::ifstream processor_fstream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
        if(processor_fstream.is_open()) {
            std::getline(processor_fstream, line);
            std::istringstream processor_sstream(line);
            //std::cout << line << "\n";
            if (iter == 0) //could be improved?
                processor_sstream >> cpu >> prev_user >> prev_nice >> prev_system >> prev_idle >> prev_iowait >> prev_irq >> prev_softirq >> prev_steal >> prev_guest >> prev_guest_nice;
            else {
                processor_sstream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            iter++;
        }
    }
    iter = 0;
    // Calculation based on https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    prevIdle = prev_idle + prev_iowait;
    currentIdle = idle + iowait;

    prevNonIdle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
    currentNOnIdle = user + nice + system + irq + softirq + steal;

    PrevTotal = prevIdle + prevNonIdle;
    currentTotal = currentIdle + currentNOnIdle;

    totald = currentTotal - PrevTotal;
    idled = currentIdle - prevIdle;

    cpu_percentage = 1.0 * (totald - idled)/totald; // 1.0 is to implicitly typecast long to float
    return cpu_percentage;
}