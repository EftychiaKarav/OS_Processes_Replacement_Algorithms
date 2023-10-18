# Interprocess Communication (IPC) in Linux OS


## Development Environment

The code was tested in Linux systems. More specifically, the software used was:
* Ubuntu 18.04.6 LTS
* gcc 7.5.0
* valgrind 3.13.0
* GNU Make 4.1

## Objective
The purpose of this project is to simulate the behavior of a memory management system based on real trace reports. Two file reports with memory traces are given: **bzip.trace** and **gcc.trace**. Each file contains several lines. These are hexadecimal memory references followed by ***R*** or ***W*** indicating whether it is for writing or reading.

### How it works:
* The simulator will read the file and run the virtual memory mechanism. The virtual memory is represented by a hashed page table. The simulator needs to know which pages have been loaded into main memory. 

* When a reference trace is found, the simulator checks if that page is in main memory. If there is space in the memory, the page is added. If the page is not in the memory and the memory is already full, the simulator decides which page to remove (page replacement). 

* If the page to be removed has been modified, it is "saved" to disk. 

* When a new page is loaded, the page table is updated accordingly. All pages and frames are 4096 bytes in size. 

* The Replacement Algorithms implemented are: ***Least Recently Used*** (LRU) and ***Second Chance***.

* When there is need to write (or read) to disk, the event should be logged in appropriate counters in the simulator. The program will alternately read groups of references (number q) from each trace file, so that there are pages from both files in main memory.


### Compilation and running

There is Makefile for the compilation and running of the program:

```
make clean
make
```
This Makefile directive will produce **test** executable.
Then you can type in the terminal:

```
./test a b c d 

where: 
a: replacement algorithm (type 1 for LRU and 2 for Second Chance)
b: number of frames (e.g 100)
c: number of traces being read from each file alternatively (it is the number q e.g. 30)
d: maximum number of traces being read from both files. (e.g if 10000 traces are given, then the 5000 will be read from bzip.trace and the rest 5000 from gcc.trace) 
```
After the execution of the program you can see the some statistics:

* The number of writes/reads from disk
* Page faults
* Entries examined from the reference trace file
* Number of memory frames