#pragma once
#include <nvml.h>
#include <vector>
#include <string>

class GPUInfo
{
private:
    std::vector<nvmlDevice_t> m_devices;

    GPUInfo(GPUInfo&) = delete;
    GPUInfo* operator=(GPUInfo&) = delete;

    using uint32_celsius_degrees_t = unsigned int;
    using uint32_rpm_t = unsigned int;
    using uint32_milliwatts_t = unsigned int;
    using uint32_mhz_t = unsigned int;
    using uint32_percentage_t = unsigned int;
    using uint64_bytes_t = unsigned long long;

    void _init();
public:
    GPUInfo();
    ~GPUInfo();

    size_t GetGPUCount() const;
    std::string GetGPUName(size_t index = 0) const;
    nvmlPciInfo_st GetPCIInfo(size_t index = 0) const;
    uint32_celsius_degrees_t GetGPUTemperature(size_t index = 0) const;
    uint32_rpm_t GetFanSpeed(size_t index = 0) const;
    uint32_milliwatts_t GetPowerUsage(size_t index = 0) const;
    uint32_mhz_t GetGPUClock(size_t index = 0) const;
    uint32_mhz_t GetMemClock(size_t index = 0) const;
    uint32_mhz_t GetSMClock(size_t index = 0) const;
    uint64_bytes_t GetMemTotal(size_t index = 0) const;
    uint64_bytes_t GetMemUsed(size_t index = 0) const;
    uint64_bytes_t GetMemFree(size_t index = 0) const;
    uint32_percentage_t GetGPULoad(size_t index = 0) const;
    uint32_percentage_t GetMemLoad(size_t index = 0) const;
};