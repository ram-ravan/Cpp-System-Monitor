#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <thread>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line; 
  int line_num = 0;
  string key, kb; 
  float memtotal = 0, memfree = 0, memutil = 0, memutil_percentage = 0;

  std::ifstream stream("/proc/meminfo");
  if(stream.is_open()) {
    while(std::getline(stream, line) && (line.compare(0,9,"MemTotal:") == 0 || line.compare(0,8,"MemFree:") == 0 )) {
      std::istringstream sstream(line);
      line_num++;  
      if(line_num == 1) {
        sstream >> key >> memtotal >> kb;
      }
      else {
        sstream >> key >> memfree >> kb;
        line_num = 0;
      }
    }
  }
  memutil = memtotal - memfree;
  memutil_percentage = memutil / memtotal;
  return memutil_percentage;
}

long LinuxParser::UpTime() {
  string line;
  double active_uptime = 0,
  idle_uptime = 0;
  std::ifstream uptime_fstream(kProcDirectory + kUptimeFilename);
  if(uptime_fstream.is_open()) {
    std::getline(uptime_fstream, line);
    std::istringstream uptime_sstream(line);
    uptime_sstream >> active_uptime >> idle_uptime;
  }
  return active_uptime;
}

int LinuxParser::TotalProcesses() {
  string line;
  string str1 ("processes");
  string processes;
  int total_processes;
  std::ifstream totprocesses_fstream(kProcDirectory + kStatFilename);
  while(std::getline(totprocesses_fstream, line)) {
    std::istringstream totprocesses_sstream(line);
    if(str1.compare(0,9,line,0,9) == 0) {
      totprocesses_sstream >> processes >> total_processes;
    }
  }
  return total_processes;
}

int LinuxParser::RunningProcesses() { 
  string line;
  string str1 ("procs_running");
  string processes;
  int procs_running;
  std::ifstream totprocesses_fstream(kProcDirectory + kStatFilename);
  while(std::getline(totprocesses_fstream, line)) {
    std::istringstream totprocesses_sstream(line);
    if(str1.compare(0,13,line,0,13) == 0) {
      totprocesses_sstream >> processes >> procs_running;
    }
  }
  return procs_running; 
}

string LinuxParser::Command(int pid) { 
  string start_command;

      std::ifstream cmd_fstream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
      if(cmd_fstream.is_open()) {
        std::getline(cmd_fstream, start_command);
      }
  return start_command; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string mem_util;
  string line;
  string key;
  string value;
  float value_mb;
  std::ifstream util_fstream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
  if(util_fstream.is_open()) {
    while(std::getline(util_fstream, line)) {
      std::istringstream util_sstream(line);
      while(util_sstream >> key >> value) {
        if(key == "VmRSS:") { //VmSize corresponds to Virtual Memory Size which is different from the physical RAM Memory. To use physical RAM memory, VmRSS is helpful.
          value_mb = std::stof(value)  * 0.001;
        }
      }
    }
  }
  mem_util = std::to_string(value_mb);
  return mem_util.substr(0,7);
}

float LinuxParser::CpuUtilization(int pid) {
      float total_time = 0.0;
    float seconds = 0.0;
    float cpu_usage = 0.0;
    
    string line;
    float sys_uptime = 0;
    string stream_1, stream_2, stream_3, stream_4, stream_5, stream_6, 
    stream_7, stream_8, stream_9, stream_10, stream_11, stream_12, 
    stream_13, stream_18, stream_19, stream_20, stream_21;
    float utime, stime, cutime, cstime, starttime;
    std::ifstream start_fstream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
    if(start_fstream.is_open()) {
        std::getline(start_fstream, line);
        std::istringstream start_sstream(line);
        start_sstream >> stream_1 >>  stream_2 >> stream_3 >> stream_4 >> stream_5 >> stream_6 >> 
        stream_7 >>  stream_8 >> stream_9 >> stream_10 >>  stream_11 >> stream_12 >> 
        stream_13 >>  utime >> stime >>  cutime >>  cstime >> stream_18 >>  
        stream_19 >>  stream_20 >> stream_21 >> starttime;
    }
    std::ifstream sysstart_fstream("/proc/uptime");
    if(sysstart_fstream.is_open()) {
        std::getline(sysstart_fstream, line);
        std::istringstream sysstart_sstream(line);
        sysstart_sstream >> sys_uptime;
    }
    total_time = utime + stime;
    total_time += cutime + cstime;
    seconds = sys_uptime - (starttime / sysconf(_SC_CLK_TCK));
    cpu_usage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds); //cpu usage in percentage
    return cpu_usage;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string uid_value;
    vector<int> pids = LinuxParser::Pids();
      for(int p : pids) {
        if (pid == p) {
          string line;
          string key;
          string value_1;
          std::ifstream uid_stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
          if(uid_stream.is_open()) {
            while (std::getline(uid_stream, line)) {
              std::istringstream uid_linestream(line);
              while(uid_linestream >> key >> value_1) {
                if(key == "Uid:") {
                  uid_value = value_1;
                }
              }
           }
          }
      
        }
      }
  return uid_value;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user_name;
  string line;
  string key;
  string value_1;
  string uid_value;
  string user_sname;
  string user_randletter;
  string user_uid;

  std::ifstream uid_stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
  if(uid_stream.is_open()) {
    while (std::getline(uid_stream, line)) {
      std::istringstream uid_linestream(line);
      while(uid_linestream >> key >> value_1) {
        if(key == "Uid:") {
          uid_value = value_1;
          std::ifstream user_fstream(LinuxParser::kPasswordPath);
          if(user_fstream.is_open()) {
            while (std::getline(user_fstream, line)) {
              std::replace(line.begin(), line.end(), ':', ' ');
              std::istringstream userstream(line);
              while (userstream >> user_sname >> user_randletter >> user_uid) {
                if(user_uid == uid_value) {
                  user_name = user_sname;
                  return user_name;
                }
              }
                        
            }
          }
        }
      }
    }
  }
  return user_name;
}

long LinuxParser::UpTime(int pid) {
  long uptime;
  string line;
  string stream;
  string stream_1, stream_2, stream_3, stream_4, stream_5, stream_6, 
  stream_7, stream_8, stream_9, stream_10, stream_11, stream_12, 
  stream_13, stream_14, stream_15, stream_16, stream_17, stream_18, 
  stream_19, stream_20, stream_21, starttime;
  std::ifstream start_fstream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if(start_fstream.is_open()) {
    std::getline(start_fstream, line);
    std::istringstream start_sstream(line);
    // I spent time to find a better way to implement this, but no success so far. Would be helpful to know a smart way to implement this easier.
    start_sstream >> stream_1 >>  stream_2 >> stream_3 >> stream_4 >> stream_5 >> stream_6 >> 
    stream_7 >>  stream_8 >> stream_9 >> stream_10 >>  stream_11 >> stream_12 >> 
    stream_13 >>  stream_14 >> stream_15 >>  stream_16 >>  stream_17 >> stream_18 >>  
    stream_19 >>  stream_20 >> stream_21 >> starttime;
    uptime = std::stol(starttime) / sysconf(_SC_CLK_TCK);
  }
  return uptime;
}
