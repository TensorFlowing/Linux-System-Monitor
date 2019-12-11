#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 1,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
enum ProcessCPUStates {
  kPPid_ = 1,
  kPComm_,
  kPState_,
  kPPpid_,
  kPPgrp_,
  kPSession_,
  kPTty_nr_,
  kPTpgid_,
  kPFlags_,
  kPMinflt_,
  kPCminflt_,
  kPMajflt_,
  kPCmajflt_,
  kPUtime_,
  kPStime_,
  kPCutime_,
  kPCstime_,
  kPPriority_,
  kPNice_,
  kPNum_threads_,
  kPItrealvalue_,
  kPStarttime_,
};
float CpuUtilization(int pid);
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif