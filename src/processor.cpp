#include "processor.h"
#include "linux_parser.h"
#include "string"
#include "vector"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long Active = LinuxParser::ActiveJiffies();
  long Idle = LinuxParser::IdleJiffies();
  float cpuUtilization = Active / double(Active + Idle);

  return cpuUtilization;
}