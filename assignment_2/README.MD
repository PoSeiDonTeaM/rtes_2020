# ASSIGNMENT 2

**Description:** 
This specific assignment has the same logic as its predecessor, solving the producer-consumer problem employing the pthread functions created in assignment 1. The difference now is that the execution is based on a timer struct that will execute a number of functions, pass data from producer to consumer. 

Our goal is to capture specific timestamps and compare them for each iteration with different system specifications, mainly on various period times that the program runs. For example, the durations we are requested to measure are:
* **jobAliveTime:** Measures how long did it take by the time that the task was created till the time it was executed.
* **jobExecutionTime:** Measures how long did it take for the task to be executed by the time it got fetched from the queue.
* **inputDuration:** Measures how long did it take for the task to be added in the queue by the time it was created.

Let's note that the computer that the software was created does not support multi-threading. For that reason, we are utilizing the capabilities of the raspberry pi 3, which supports multi-threading. However, in order to run the program in the rpi, we cannot just compile it in our computer and pass the executable to be run as the rpi uses different CPU architecture (ARM) as the computer does. So, we are utilizing the cross-compiler of abhiTronix, found in his [github page](https://github.com/abhiTronix/raspberry-pi-cross-compilers).

## How to run:

**1.** Compile the program using the following command: `arm-linux-gnueabihf-gcc -g -o <executableName> <externalFunction1.c> <externalFunction2.c> <main.c> -lpthread -lm` (-lm is used to execute the math operations we have specifically in this software).

**2.** Resolve any compilation issues

**3.** Check if there are any Segmentation faults (usually due to bad memory allocations)

**4.** Set the number of consumers, the queue size, the runtime duration and select the desired period using the proper definitions `#define` at the top of the program.

**5.** Now we need to send the cross-compiled executable to the rpi device. The most popular way to do this is to connect to the rpi using `SSH`, so use the respective, following commands:
 * `scp <executableName> <rpiName>@<rpiIP>:/location/to/be/saved`
 * `ssh 192.168.1.<IP assigned to RPI from router> -lroot`
 * Run the executable from the rpi using `./<executableName>`
 
 ## Lessons Learned
 
 * Compile as many times as you can with high frequency. I did the mistake to write the whole software and then start compiling. You can't imagine the hell I've been to resolve all errors.
 
 * Make sure that you have allocated correctly all memory needed to matrices.
 
 * Careful when using the pthread functions. 
 
