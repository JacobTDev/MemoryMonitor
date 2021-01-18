# MemoryMonitor
MacOS utility to view processes real memory usage.

# Build & Run
```
gcc main.c -o MemoryMonitor
sudo ./MemoryMonitor
```
NOTE: MemoryMonitor can't function correctly without being run as root. 

# Why, Root? 
Well, if it wasn't running as root, it would not be able to get information about other users processes.  

# Tested
```
macOS 10.15.7
gcc
```
