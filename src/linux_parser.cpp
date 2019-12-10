#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read OS information from the filesystem
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

// Read Kernel information from the filesystem
string LinuxParser::Kernel() {
  string tmp;
  string kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> tmp >> tmp >> kernel;
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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string tmp;
  string totalMem;
  string freeMem;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> tmp >> totalMem;
    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> tmp >> freeMem;
  }
  float f_totalMem = std::stof(totalMem);
  float f_freeMem = std::stof(freeMem);
  return (f_totalMem - f_freeMem) / f_totalMem;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

// Read and return the number of jiffies for the system (since boot)
// jiffies = number of clock ticks
long LinuxParser::Jiffies() { return LinuxParser::UpTime()*sysconf(_SC_CLK_TCK); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  return LinuxParser::UpTime(pid)/sysconf(_SC_CLK_TCK); 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpu_info = LinuxParser::CpuUtilization();
  long user = std::stol(cpu_info[0]);
  long nice = std::stol(cpu_info[1]);
  long system = std::stol(cpu_info[2]);
  long irq = std::stol(cpu_info[5]);
  long softirq = std::stol(cpu_info[6]);
  long steal = std::stol(cpu_info[7]);
  long Active = user + nice + system + irq + softirq + steal;
  return Active;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
    vector<string> cpu_info = LinuxParser::CpuUtilization();
    long idle = std::stol(cpu_info[3]);
    long iowait = std::stol(cpu_info[4]);
    long Idle = idle + iowait;
    return Idle;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_info;
  string tmp;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);  
    std::istringstream linestream(line);
    linestream >> tmp;  // skip string "cpu"
    for (int i = kUser_; i != kGuestNice_; i++) {
      linestream >> tmp;
      cpu_info.push_back(tmp);
    }
  }
  return cpu_info;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
	string line;
  string tmp;
  string totalProc;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while ( std::getline(stream, line) ) {
        std::istringstream linestream(line);
        linestream >> tmp;
        if (tmp=="processes") {
          linestream >> totalProc;
          break;
        }
      }
    }
    return std::stod(totalProc);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string tmp;
  string runProc;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while ( std::getline(stream, line) ) {
      std::istringstream linestream(line);
      linestream >> tmp;
      if (tmp=="procs_running") {
        linestream >> runProc;
        break;
      }
    }
  }
  return std::stod(runProc);
}

// Read and return the CPU usage of a process
float LinuxParser::CpuUtilization(int pid) {
  string line;
  string tmp;
  string utime;
  string stime;
  string starttime;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);  
    std::istringstream linestream(line);
    int i = kPPid_; // First entry
    while(linestream >> tmp) {
      if (i==kPUtime_) {
        utime = tmp; // in clock ticks
      }
      else if (i==kPStime_) {
        stime = tmp; // in clock ticks
      }
      else if (i==kPStarttime_) {
        starttime = tmp; // in clock ticks
        break;
      }
      i++;
    }
  }
  long total_time = std::stol(utime) + std::stol(stime); // total time used by this process (in clock ticks)
  long elapsed_time = LinuxParser::Jiffies() - std::stol(starttime);
  float cpu_usage = total_time/double(elapsed_time);

  return cpu_usage;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string Command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> Command;
  }
  return Command;  
}

// // Read and return the memory used by a process
// string LinuxParser::Ram(int pid) { 
//   string tmp;
//   string line;
//   string ram_kB;
//   std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
//   if (stream.is_open()) {
//     while (std::getline(stream, line)) {
//       std::istringstream linestream(line);
//       linestream >> tmp;
//       if (tmp=="VmSize:") {
//         linestream >> ram_kB;
//         break;
//       }
//     }
//   }
//   long ram_MB = 0;
//   try {
//     ram_MB = std::stol(ram_kB)/1000;
//   }
//   catch(const std::invalid_argument) {
//     // std::cerr << "Invalid argument\n"; 
//   }
//   return std::to_string(ram_MB);
// }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string tmp;
  string line;
  string ram_kB;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    for (int i = 0; i < 18; i++) {
      std::getline(stream, line); 
    }
    std::istringstream linestream(line);
    linestream >> tmp;  // read "VmSize"
    linestream >> ram_kB;
  }
  // int ram_MB = std::stod(ram_kB)/1000;
  // return std::to_string(ram_MB);
  return ram_kB;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string tmp;
  string line;
  string Uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tmp;
      if (tmp=="Uid:") {
        linestream >> Uid;
        break;
      }
    }
  }
  return Uid;
}


// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string Uid = LinuxParser::Uid(pid);
  string tmp;
  string line;
  string User;
  string Uid_tmp;
  const std::string kPasswordPath{"/etc/passwd"};
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      std::getline(linestream, User, ':');
      std::getline(linestream, tmp, ':');
      std::getline(linestream, Uid_tmp, ':');
      if (Uid==Uid_tmp) {
        return User;
      }
    }
  }
  return "N/A"; // Corner case: no user found
}


// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string tmp;
  string pUptime;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);  
    std::istringstream linestream(line);
    int i = kPPid_; // First entry
    while(linestream >> tmp) {
      if (i==kPStarttime_) {
        pUptime = tmp; // in clock ticks
        break;
      }
      i++;
    }
  }
  return std::stol(pUptime)/sysconf(_SC_CLK_TCK); 
}

