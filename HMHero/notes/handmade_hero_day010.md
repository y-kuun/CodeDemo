# Handmade Hero Day010，QueryPerformanceCounter and RDTSC

## 概述

这部分的内容是是基于Day009的代码的，
主要的目标是在程序当中如何获得时间信息。

## 背景

* 时间的种类
  * Wall clock time, 真实世界时间
  * Processor time，how many circles does the processor experienced？
 
* 相关的函数
  * RDTSC，CPU instruction
	- TSC，Time-Stamp Counter，在每一个CPU的时钟周期（clock cycle）之后会加1
	- RDTSC，将TSC的值放入`EDX:EAX`
	- 相关特性
		- CPU相关的一个计数值， 不是进程相关的
		- CPU的主频会变，实际上每一个时钟周期代码的真实时间不一致
		- 多核同步问题
   * [QueryPerformanceCounter](https://docs.microsoft.com/en-us/windows/desktop/api/profileapi/nf-profileapi-queryperformancecounter)
	 * Win API, 用来处理高精度的时间需求
  
  * [QueryPerformanceFrequency](https://docs.microsoft.com/en-us/windows/desktop/api/profileapi/nf-profileapi-queryperformancefrequency), performance counter的频率
	 * 启动的时候被计算出来，在所有的processor中都是一个常量
  
## 改动部分

获取性能计数器的频率
```c++
LARGE_INTEGER PerfCountFrequencyResult;
QueryPerformanceFrequency(&PerfCountFrequencyResult);
int64_t PerfCountFrequency = PerfCountFrequencyResult.QuadPart;
```

获得第帧的相关计数器的值，包括性能计数器，以及Time-Stamp计数器
```c++
LARGE_INTEGER LastCounter;
QueryPerformanceCounter(&LastCounter);
uint64_t LastCycleCount = __rdtsc();

```

```c++
// UNION
uint64_t EndCycleCount = __rdtsc();
LARGE_INTEGER EndCounter;
QueryPerformanceCounter(&EndCounter);

// TODO(casey): Display the value here
uint64_t CyclesElapsed = EndCycleCount - LastCycleCount;
int64_t CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;

// micro second per frame, wall clock time
double MSPerFrame = (((1000.0f*(double)CounterElapsed) / (double)PerfCountFrequency));
double FPS = (double)PerfCountFrequency / (double)CounterElapsed;
// mega circles per frame
double MCPF = ((double)CyclesElapsed / (1000.0f * 1000.0f));
OutputDebugStringAFormat("%.02fms/f,  %.02ff/s,  %.02fmc/f\n", MSPerFrame, FPS, MCPF);

LastCounter = EndCounter;
LastCycleCount = EndCycleCount;

```

## 其他的计算方法

* clock()
* time()
* __rdtsc()
  - An intrinsic is a compiler-specific extension that allows direct invocation of some processor instruction. They generally need to be extensions to the compiler so they can avoid all the expensive niceties compilers have to afford functions.
* QueryPerformanceCounter()

## references

1. [Game Timing and Multicore Processors](https://docs.microsoft.com/en-us/windows/desktop/dxtecharts/game-timing-and-multicore-processors)










