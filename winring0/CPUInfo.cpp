#include "stdafx.h"
#include <Windows.h>
#include <processthreadsapi.h>
#include <sysinfoapi.h>
#include <Intrin.h>
#include <atomic>

#include "include/OlsApiInit.h"
#include "include/CPUInfo.h"
//#include "OlsDef.h"
//#include "OlsApiInit.h"

static std::atomic<int> s_count = 0;

CPUInfo::CPUInfo()
    : m_Tjmax(0)
    , m_cores(0)
{
    //if (s_count == 0)
    //    InitializeOls();
    _CPUInfoInit();
    s_count++;
}

CPUInfo::~CPUInfo()
{
    s_count--;
    if (s_count == 0)
        _CPUInfoExit();
}

std::vector<int> CPUInfo::GetCoreTemp()
{
    std::vector<int> v(m_cores);
    for (size_t i = 0; i < v.size(); i++)
    {
        int mask =  0x01 << i;         
        SetProcessAffinityMask(GetCurrentProcess(), mask);
        DWORD eax, edx;
        Rdmsr(0x19c, &eax, &edx);    //Read temp（eax&0x7f0000）
        v[i] = m_Tjmax - ((eax & 0x7f0000) >> 16);//Real temp= Tjmax - value
    }
    return v;
}

int CPUInfo::GetCPUAverageTemp()
{
    auto temps = GetCoreTemp();
    int temp = 0;
    for (auto& t : temps)
        temp += t;
    temp = temp / (int)temps.size();
    return temp;
}

int CPUInfo::GetCPUMaxTemp()
{
    auto temps = GetCoreTemp();
    int temp = 0;
    for (auto& t : temps)
    {
        if (t > temp)
            temp = t;
    }
    return temp;
}

void CPUInfo::_CPUInfoInit()      
{
    HMODULE m_hOpenLibSys;
 
    m_hOpenLibSys = NULL;
    if (InitOpenLibSys(&m_hOpenLibSys) != TRUE)
    {
        printf("DLL Load Error!");
        return ;
    }
    DWORD eax, edx;
    Rdmsr(0x1A2, &eax, &edx);  
    m_Tjmax = (eax & 0xff0000) >> 16; //Get Tjmax
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    m_cores = sysInfo.dwNumberOfProcessors;
    SetProcessAffinityMask(GetCurrentProcess(), 1);  //Get first cpu
}

void CPUInfo::_CPUInfoExit()
{
    DeinitializeOls();
}

int CPUInfo::GetCoreMax()
{
    return m_cores;
}

uint8_t CPUInfo::GetCPULoad()
{
    FILETIME               ft_sys_idle;
    FILETIME               ft_sys_kernel;
    FILETIME               ft_sys_user;

    ULARGE_INTEGER         ul_sys_idle;
    ULARGE_INTEGER         ul_sys_kernel;
    ULARGE_INTEGER         ul_sys_user;

    static ULARGE_INTEGER     ul_sys_idle_old;
    static ULARGE_INTEGER  ul_sys_kernel_old;
    static ULARGE_INTEGER  ul_sys_user_old;

    uint8_t usage = 0;

    // We cannot directly use GetSystemTimes in the C language
    /* Add this line :: pfnGetSystemTimes */
    GetSystemTimes(&ft_sys_idle,    /* System idle time */
        &ft_sys_kernel,  /* system kernel time */
        &ft_sys_user);   /* System user time */

    CopyMemory(&ul_sys_idle, &ft_sys_idle, sizeof(FILETIME)); // Could been optimized away...
    CopyMemory(&ul_sys_kernel, &ft_sys_kernel, sizeof(FILETIME)); // Could been optimized away...
    CopyMemory(&ul_sys_user, &ft_sys_user, sizeof(FILETIME)); // Could been optimized away...

    usage =
        (uint8_t)(
            (
                (
                    (
                        (ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart) +
                        (ul_sys_user.QuadPart - ul_sys_user_old.QuadPart)
                        )
                    -
                    (ul_sys_idle.QuadPart - ul_sys_idle_old.QuadPart)
                    )
                *
                (100)
                )
            /
            (
                (ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart) +
                (ul_sys_user.QuadPart - ul_sys_user_old.QuadPart)
                )
            );

    ul_sys_idle_old.QuadPart = ul_sys_idle.QuadPart;
    ul_sys_user_old.QuadPart = ul_sys_user.QuadPart;
    ul_sys_kernel_old.QuadPart = ul_sys_kernel.QuadPart;

    return usage;
}

uint64_t CPUInfo::GetMaxMemory()
{
    MEMORYSTATUSEX memstat = { sizeof(memstat) };
    if (!GlobalMemoryStatusEx(&memstat))
        return 0;
    return memstat.ullTotalPhys;
}

uint64_t CPUInfo::GetUsedMemory()
{
    MEMORYSTATUSEX memstat = { sizeof(memstat) };
    if (!GlobalMemoryStatusEx(&memstat))
        return 0;
    return memstat.ullTotalPhys - memstat.ullAvailPhys;
}

std::string CPUInfo::GetCPUName()
{
    int CPUInfo[4] = { -1 };
    char CPUBrandString[0x40];
    __cpuid(CPUInfo, 0x80000000);
    unsigned int nExIds = CPUInfo[0];

    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    // Get the information associated with each extended ID.
    for (unsigned int i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string.
        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }

    return CPUBrandString;
}