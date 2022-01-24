#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

// Return this process's ID
int Process::Pid() {
    return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    cpu_ = LinuxParser::CpuUtilization(Pid());
    return cpu_;
}

string Process::Command() { 
    command_ = LinuxParser::Command(Pid());
    return command_.substr(0,50); 
}

string Process::Ram() {
    ram_ = LinuxParser::Ram(Pid());
    return ram_; 
}

string Process::User() {
    user_ = LinuxParser::User(Pid());
    return user_;
}

long int Process::UpTime() {
    uptime_ =  LinuxParser::UpTime() - LinuxParser::UpTime(Pid()); 
    return uptime_; 
}

bool Process::operator<(Process const& a) const { 
    return (cpu_ < a.cpu_);
 }
 // (ram_ < a.ram_) && (cpu_ < a.cpu_) 