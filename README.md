# 911 Emergency Dispatch Simulation

A Discrete-Event Simulation (DES) of a 911 emergency dispatch center, written in C. Developed as the final project for the Advanced Programming Workshop course at the Holon Institute of Technology.

## Overview

The simulation models the full lifecycle of emergency calls — from arrival, through dispatcher handling, to field unit dispatch and call resolution. It is designed to analyze dispatcher utilization, queue behavior, and response times under different configurations.

## Features

- **Discrete-Event Simulation engine** built around a Future Event List (FEL), implemented as a sorted linked list
- **Priority-based call queue** (Critical > High > Normal)
- **Multiple dispatchers** with per-dispatcher utilization tracking
- **Dynamic 2D matrix of field units** — Police, Ambulance, and Firetruck
- **Stochastic modeling** — exponential distribution for call inter-arrival and service times
- **Full event log** written to file for post-run analysis
- **Config save/load** — store a simulation setup and replay it
- **End-of-simulation statistics report** — utilization, queue stats, and response times

## Project structure

```
911 simulation/
├── main.c            Entry point and menu loop
├── simulation.c/h    Core DES engine and event handlers
├── fel.c/h           Future Event List (sorted linked list)
├── queue.c/h         Priority call queue (linked list)
├── units.c/h         Dynamic 2D field unit matrix
├── stats.c/h         Statistics tracking and report
├── file_io.c/h       Event logging, config save/load
├── menu.c/h          User interface and input validation
└── defs.h            Shared structs, defines, and constants
```

## Build & Run

### Linux / macOS (gcc or clang)

```bash
make        # builds ./sim911
make run    # build and run
```

### Windows (Visual Studio)

1. Open `911 simulation.slnx`
2. Build the solution (`Ctrl+Shift+B`)
3. Run (`Ctrl+F5`)

### Menu options

| Option | Action |
|--------|--------|
| 1 | Enter parameters manually and run |
| 2 | Load config from file and run |
| 3 | Save current config to file |
| 4 | View last simulation log |
| 5 | Exit |

## Configuration parameters

| Parameter | Description |
|-----------|-------------|
| Dispatchers | Number of dispatchers available |
| Police units | Number of police field units |
| Ambulances | Number of ambulance units |
| Firetrucks | Number of firetruck units |
| Simulation time | Total simulation duration (minutes) |
| Mean inter-arrival time | Average time between incoming calls (minutes) |

## Sample output

```
--- Simulation Started ---
[T=  2.31] Call C1 arrived. Priority: HIGH | Type: POLICE
[T=  2.31] Dispatcher D0 picked up call C1
[T=  4.55] Unit POL_0 dispatched for call C1.
[T= 12.83] Call C1 resolved. Dispatcher D0 is now available.
--- Simulation Ended ---
```

## Built with

- C (C99), standard libraries only (`stdio`, `stdlib`, `math`, `time`)
- Visual Studio / gcc
- GitHub Actions for continuous build verification
