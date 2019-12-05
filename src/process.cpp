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

// Return this process's ID
int Process::Pid() { return this->_Pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(Process::Pid()); }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::Pid()); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid()); }

bool Process::operator<(Process & a) { 
    // Compare based on cpu usage
    float cpu = CpuUtilization();
    if (cpu < a.CpuUtilization()) {
        return true;
    }
    else {
        return false;
    }
}

// Overload the "less than" comparison operator for Process objects
// bool Process::operator<(Process & a) { 
//     // Compare based on memory usage
//     string ram = Ram();
//     if (std::stod(ram) < std::stod(a.Ram())) {
//         return true;
//     }
//     else {
//         return false;
//     }
// }
