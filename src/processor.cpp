#include "processor.h"
#include "linux_parser.h"
#include "string"
#include "vector"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
double Processor::Utilization() {
  long Active = LinuxParser::ActiveJiffies();
  long Idle = LinuxParser::IdleJiffies();
  double cpuUtilization = Active / double(Active + Idle);

  return cpuUtilization;
}