# PepperOS
![PepperOS Logo](https://github.com/hamidrm/PepperOS/blob/master/pepper_os.jpg?raw=true)  
PepperOS is a simple and lightweight real time kernel.  It�s under development yet and I�m working on it just for entertainment.
Highlights Include:
- RR scheduler � preemptive with optional time slicing.
- RTOS objects (tasks, ipc, semaphores, software timers, mutexes , console and external interrupts).
- Small footprint.
- It runs on STM32F030F4 (Cortex-M0) currently, but it's portable to other MCU's.
![STM32F030F4 Board](https://github.com/hamidrm/PepperOS/blob/master/stm32f030f4.jpg?raw=true)  
- Designed to be small and simple. Typically, the kernel binary image will be in the region of around 10K bytes.
- Efficient software timers.
- Inter-process communication by message passing.
- Heap memory management by bitmap method.
- No software restriction on the number of real time tasks that can be created.
- CPU sleeps while tasks are blocked.


There are tri-level priority in this RTOS :
1. High Priority : With 125% of quantum time and will yields immediately on signaling semaphores.
2. Normal Priority : 100% of quantum time.
3. Low Priority : 75% of quantum time.

# PepperOS Video
[![PepperOS Examples](http://img.youtube.com/vi/son4uOCHAqU/1.jpg)](http://www.youtube.com/watch?v=son4uOCHAqU "PepperOS Examples")


Current example projects :
- /examples/stm32f030x4/blink_1 : One task does LED blinking by a software timer.
- /examples/stm32f030x4/blink_2 : Two tasks does blinking on 3 LED. One task sends blinking message every 200 ms , and another one, does LEDs blinking.
- /examples/stm32f030x4/button : Two tasks does blinking on 3 LED. One task sends blinking message every 200 ms , and another one, does LEDs blinking and receiving Button message and reverse blinking sequence.
- /examples/stm32f030x4/mutex : Will be shown Mutex effect on two tasks.
- /examples/stm32f030x4/ping : One task will measures elapsed time while sending ping and receiving pong by another task and will print the its value.
- /examples/stm32f030x4/uart_1 : Two tasks prints two text (By UART with software timers).
- /examples/stm32f030x4/uart_2 : A whole console that recognize LED on/off commands (By two tasks).
