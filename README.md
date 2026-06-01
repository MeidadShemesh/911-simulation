# 911 Emergency Dispatch Simulation

A Discrete-Event Simulation (DES) of a 911 emergency dispatch center, written in C.

## Overview

This simulation models the lifecycle of emergency calls — from arrival, through dispatcher handling, to field unit dispatch and call resolution. It is designed to analyze dispatcher utilization, queue behavior, and response times under different configurations.

## Features

- Discrete-Event Simulation engine with a Future Event List (FEL)
- Priority-based call queue (Critical > High > Normal)
- Multiple dispatcher support with utilization tracking
- Dynamic 2D matrix of field units (Police, Ambulance, Firetruck)
- Exponential distribution for call arrival and service times
- Full event log saved to file
- Save and load simulation configuration
- End-of-simulation statistics report

## Project Structure

```
├── main.c            - Entry point and menu loop
├── simulation.c/h    - Core DES engine and event handlers
├── fel.c/h           - Future Event List (sorted linked list)
├── queue.c/h         - Priority call queue (linked list)
├── units.c/h         - Dynamic 2D field unit matrix
├── stats.c/h         - Statistics tracking and report
├── file_io.c/h       - File logging, config save/load
├── menu.c/h          - User interface and input validation
├── defs.h            - All shared structs, defines, and constants
```

## How to Run

1. Open the `.sln` file in Visual Studio
2. Build the solution (Ctrl+Shift+B)
3. Run the program (Ctrl+F5)
4. Choose from the menu:
   - **1** — Enter parameters manually and run
   - **2** — Load config from file and run
   - **3** — Save current config to file
   - **4** — View last simulation log
   - **5** — Exit

## Configuration Parameters

| Parameter | Description |
|---|---|
| Dispatchers | Number of dispatchers available |
| Police units | Number of police field units |
| Ambulances | Number of ambulance units |
| Firetrucks | Number of firetruck units |
| Simulation time | Total simulation duration (minutes) |
| Average call time | Mean inter-arrival time (minutes) |

## Sample Output

```
--- Simulation Started ---
[T=  2.31] Call C1 arrived. Priority: HIGH | Type: POLICE
[T=  2.31] Dispatcher D0 picked up call C1
[T=  4.55] Unit POL_0 dispatched for call C1.
[T= 12.83] Call C1 resolved. Dispatcher D0 is now available.
--- Simulation Ended ---
```

## Built With

- C (C99)
- Visual Studio
- Standard libraries: stdio, stdlib, math, time

## Authors

Developed by Meidad Shemesh as a final project for the Advanced Programming Workshop course.
