#pragma once
#include<windows.h>
#include <vector>
#include <string>
//#include "OlsApi.h" 

#define CONSOLE_PRINT 1
class CPUInfo
{
private:
    int m_Tjmax;    
    int m_cores;

    void _CPUInfoInit();
    void _CPUInfoExit();

    CPUInfo(CPUInfo&) = delete;
    CPUInfo* operator=(CPUInfo&) = delete;

public:
    CPUInfo();
    ~CPUInfo();

    std::string GetCPUName();
    std::vector<int> GetCoreTemp();
    int GetCPUAverageTemp();
    int GetCPUMaxTemp();
    int GetCoreMax();
    uint8_t GetCPULoad();
    uint64_t GetMaxMemory();
    uint64_t GetUsedMemory();
};