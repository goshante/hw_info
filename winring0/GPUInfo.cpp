#include "stdafx.h"
#include "GPUInfo.h"
#include <stdexcept>
#include <atomic>

static std::atomic<int> s_count = 0;

#define NVML_ASSERT(x, subj) if (x != NVML_SUCCESS) throw std::runtime_error(std::string("Failed to query ") + subj + " , error: " + nvmlErrorString(x))

void GPUInfo::_init()
{
	unsigned int device_count;
	NVML_ASSERT(nvmlDeviceGetCount(&device_count), "device count");
	m_devices.resize(device_count);

	for (unsigned int i = 0; i < device_count; i++)
	{
		nvmlDevice_t device;
		NVML_ASSERT(nvmlDeviceGetHandleByIndex(i, &device), "device handle");
		m_devices[i] = device;
	}
}

GPUInfo::GPUInfo()
{
	if (s_count == 0)
		NVML_ASSERT(nvmlInit(), "nvmlInit");
	s_count++;
	_init();
}

GPUInfo::~GPUInfo()
{
	s_count--;
	if (s_count == 0)
		nvmlShutdown();
}

size_t GPUInfo::GetGPUCount() const
{
	return m_devices.size();
}

std::string GPUInfo::GetGPUName(size_t index) const
{
	char name[NVML_DEVICE_NAME_BUFFER_SIZE];
	NVML_ASSERT(nvmlDeviceGetName(m_devices[index], name, sizeof(name)), "device name");
	return name;
}

nvmlPciInfo_st GPUInfo::GetPCIInfo(size_t index) const
{
	nvmlPciInfo_t pci;
	NVML_ASSERT(nvmlDeviceGetPciInfo(m_devices[index], &pci), "pci info");
	return pci;
}

GPUInfo::uint32_celsius_degrees_t GPUInfo::GetGPUTemperature(size_t index) const
{
	uint32_celsius_degrees_t val;
	NVML_ASSERT(nvmlDeviceGetTemperature(m_devices[index], NVML_TEMPERATURE_GPU, &val), "gpu temp");
	return val;
}

GPUInfo::uint32_rpm_t GPUInfo::GetFanSpeed(size_t index) const
{
	uint32_rpm_t val;
	NVML_ASSERT(nvmlDeviceGetFanSpeed(m_devices[index], &val), "fan speed");
	return val;
}

GPUInfo::uint32_milliwatts_t GPUInfo::GetPowerUsage(size_t index) const
{
	uint32_milliwatts_t val;
	NVML_ASSERT(nvmlDeviceGetPowerUsage(m_devices[index], &val), "power usage");
	return val;
}

GPUInfo::uint32_mhz_t GPUInfo::GetGPUClock(size_t index) const
{
	uint32_mhz_t val;
	NVML_ASSERT(nvmlDeviceGetClockInfo(m_devices[index], NVML_CLOCK_GRAPHICS, &val), "gpu clock");
	return val;
}

GPUInfo::uint32_mhz_t GPUInfo::GetMemClock(size_t index) const
{
	uint32_mhz_t val;
	NVML_ASSERT(nvmlDeviceGetClockInfo(m_devices[index], NVML_CLOCK_MEM, &val), "mem clock");
	return val;
}

GPUInfo::uint32_mhz_t GPUInfo::GetSMClock(size_t index) const
{
	uint32_mhz_t val;
	NVML_ASSERT(nvmlDeviceGetClockInfo(m_devices[index], NVML_CLOCK_SM, &val), "sm clock");
	return val;
}

GPUInfo::uint64_bytes_t GPUInfo::GetMemTotal(size_t index) const
{
	nvmlMemory_t memInfo;
	NVML_ASSERT(nvmlDeviceGetMemoryInfo(m_devices[index], &memInfo), "mem total");
	return memInfo.total;
}

GPUInfo::uint64_bytes_t GPUInfo::GetMemUsed(size_t index) const
{
	nvmlMemory_t memInfo;
	NVML_ASSERT(nvmlDeviceGetMemoryInfo(m_devices[index], &memInfo), "mem total");
	return memInfo.used;
}

GPUInfo::uint64_bytes_t GPUInfo::GetMemFree(size_t index) const
{
	nvmlMemory_t memInfo;
	NVML_ASSERT(nvmlDeviceGetMemoryInfo(m_devices[index], &memInfo), "mem total");
	return memInfo.free;
}

GPUInfo::uint32_percentage_t GPUInfo::GetGPULoad(size_t index) const
{
	nvmlUtilization_t util;
	NVML_ASSERT(nvmlDeviceGetUtilizationRates(m_devices[index], &util), "gpu load");
	return util.gpu;
}

GPUInfo::uint32_percentage_t GPUInfo::GPUInfo::GetMemLoad(size_t index) const
{
	nvmlUtilization_t util;
	NVML_ASSERT(nvmlDeviceGetUtilizationRates(m_devices[index], &util), "mem load");
	return util.memory;
}