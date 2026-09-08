# CCSDS Telemetry Simulator

A C-based software simulator for space telemetry, implementing the standard **CCSDS Space Packet Protocol** (CCSDS 133.0-B-2).\
The simulation has the objective to model the communication between a satellite and a base on Earth.

## Architecture
* `include/ccsds.h`: Defines the `Telemetry_Packet` struct, which has the purpose of storing the telemetry data correctly in compliance with the CCSDS standards.
* `include/communication.h`: integrates OS specific header guards and communication functions prototypes.
* `src/communication.c`: `send_telemetry`/`get_telemetry` abstractions.
* `src/satellite.c`: Generates values to send, formats the CCSDS header in Big Endian, and sends it using UDP.
* `src/parser.c`: Listens using UDP port, reverts telemetry into Little Endian, extracts the values and prints them to terminal.

## Prerequisites

* **Linux/macOS:** `gcc` and `make`.
* **Windows:** MinGW (`gcc` and `mingw32-make`).

## Build and Run

1. **Compile the project:**
   Open the terminal in the root directory and run:
```bash
make
```
* In case of error on windows machines:
```bash
mingw32-make
```

## Project purpose and AI disclosure
The personal purpose of the project was to learn how the CCSDS standard works and is used, for this reason:

The use of AI chatbots in the sections of this project regarding the CCSDS standard was limited to helping with the documentation understanding.

AI has been used in the sections regarding the communication between the two executables as an assistant due to a lack of knowledge in those fields on my side.

AI has been used to implement the OS header guards and to write `Makefile`.
 
## Future implementations
The next steps of this project are:
* Implementation of a simulated loss of data sequences 
* Implementation of different telemetries simulating realistic values relative to the satellite simulated location
* Ability of the parser to differentiate application process identifications (APIDs) to display telemetries in different ways and put them in a Data Dictionary.
