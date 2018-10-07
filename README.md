# PepperOS
![PepperOS Logo](https://github.com/hamidrm/PepperOS/blob/master/pepper_os.jpg?raw=true)  
PepperOS is a simple and lightweight real time kernel.  It’s under development yet and I’m working on it just for entertainment.
Highlights Include:
- RR scheduler – preemptive with optional time slicing.
- RTOS objects (tasks, ipc, semaphores, software timers, mutexes , console and external interrupts).
- Small footprint.
- It runs on STM32F030F4 (Cortex-M0) currently, but it's portable to other MCU's.
- Designed to be small and simple. Typically, the kernel binary image will be in the region of around 10K bytes.
- Efficient software timers.
- Inter-process communication by message passing.
- Heap memory management by bitmap method.
- No software restriction on the number of real time tasks that can be created.
- CPU sleeps while tasks are blocked.


There are tri-level priority in this RTOS :
1. High Priority : It has 125% of quantum time and will yields immediately on signaling semaphores.
2. Normal Priority : 100% of quantum time.
3. Low Priority : 75% of quantum time.


------------


[![PepperOS Examples](http://img.youtube.com/vi/son4uOCHAqU/0.jpg)](http://www.youtube.com/watch?v=son4uOCHAqU "PepperOS Examples")