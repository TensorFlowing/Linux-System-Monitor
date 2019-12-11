#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for (int pid : pids) {
    Process proc_tmp(pid);
    // proc_tmp._Pid = pid;
    processes_.emplace_back(proc_tmp);
  }
  std::sort(processes_.begin(), processes_.end(), [](Process & a, Process & b){return (a>b);}); 
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }