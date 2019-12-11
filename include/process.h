#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // Member variable
  int _Pid = -1;
  // Constructor
  Process(int pid): _Pid(pid) {}; 
  // Methods
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process & a);  
  bool operator>(Process & a);

 private:
};

#endif