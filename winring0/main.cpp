#include "stdafx.h"
#include "include/CPUInfo.h"
#include "GPUInfo.h"
//#include "OlsApi.h"
#include <iostream>


int main(int argc, char* argv[])
{
    CPUInfo cpu;
    GPUInfo gpu;

    while (1)
    {
        std::cout << "-------------- CPU ---------------" << std::endl;
        std::cout << "CPU Name: " << cpu.GetCPUName() << std::endl;
        std::cout << "CPU Temp: " << cpu.GetCPUMaxTemp() << " C" << std::endl;
        std::cout << "CPU Load: " << (int)cpu.GetCPULoad() << " %" << std::endl;
        std::cout << "Memory used: " << cpu.GetUsedMemory() / 1000 / 1000 << " MB / " << cpu.GetMaxMemory() / 1000 / 1000 << " MB" << std::endl;
        std::cout << "-------------- GPU ---------------" << std::endl;
        std::cout << "GPU Name: " << gpu.GetGPUName() << std::endl;
        std::cout << "GPU Temp: " << gpu.GetGPUTemperature() << " C" << std::endl;
        std::cout << "GPU Load: " << gpu.GetGPULoad() << " %" << std::endl;
        std::cout << "GPU Clock: " << gpu.GetGPUClock() << " MHz" << std::endl;
        std::cout << "GPU Mem clock: " << gpu.GetMemClock() << " MHz" << std::endl;
        std::cout << "Memory used: " << gpu.GetMemUsed() / 1000 / 1000 << " MB / " << gpu.GetMemTotal() / 1000 / 1000 << " MB (" << gpu.GetMemLoad() << "%)" << std::endl;
        try
        {
            std::cout << "Fan speed: " << gpu.GetFanSpeed() << " RPM" << std::endl;
        }
        catch (...)
        {
            std::cout << "Fan speed: <No fan detected>" << std::endl;
        }
        
        
        std::cout << "GPU Power: " << gpu.GetPowerUsage() << " mW" << std::endl;
        Sleep(1000);
        system("cls");
    }
    return 0;
}
