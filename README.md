JSON Serialization for w-M-Bus-related firmware - 


Project Overview -

This project implements a lightweight C library for serializing smart-meter gateway data into a fixed and well-defined JSON format.
The library takes structured meter readings stored in deterministic C data structures and converts them into a JSON representation suitable for transmission to a backend system. The design focuses on predictable memory usage, clear data ownership, and clean separation between data formatting and transport.
While the current implementation is demonstrated using a host-side example application, the same serializer can be integrated into embedded gateway firmware where the generated JSON may be forwarded over UART, SPI, I2C, Ethernet, or wireless interfaces as part of a larger data acquisition and reporting system.
Typical application scenarios include smart-meter gateways, data concentrators, and edge devices where reliable and deterministic data formatting is required before onward transmission or storage.
________________________________________________________________________________

Development Environment -
IDE – STM32CubeIDE
Language – C
Validation – Host-side GCC build

Although the core serialization logic is written in standard C, STM32CubeIDE was chosen as the development environment to work within an embedded-oriented toolchain and build configuration.
STM32CubeIDE is based on the ARM GCC compiler used for STM32 firmware development and enforces constraints typical of embedded systems, such as strict warning levels and explicit control over memory usage. Developing and building the code in this environment ensures that the implementation is suitable for direct integration into STM32-based gateway firmware. 
The serialization library itself remains platform-independent and transport-agnostic, allowing it to be tested using a host-side GCC build while remaining fully compatible with the STM32 toolchain.
________________________________________________________________________________





Programming language choice – C -
Since one of the most important factors to be considered in this project is  controlled and predictable memory usage, it is necessary to know where memory is allocated and how much is used at all times.
In C, memory is allocated only when we create variables and arrays, and there is no automatic or hidden memory allocation. Hence, this deterministic handling of memory makes C well suited for this project.
Additionally, C uses stack memory by default and heap memory is used only when explicitly asked. Stack memory would be preferred due to its predictable nature. The usage of heap memory might lead to memory fragmentation and crashes over long runtimes.
For these reasons, C has been chosen for this project. 

________________________________________________________________________________


Memory usage and constraints - 

The serializer uses fixed-size arrays throughout to ensure predictable memory usage and avoid dynamic allocation.
With the current configuration, one populated GatewayData structure occupies - 
approximately 2.2 KB of RAM.
This includes fixed-size storage for gateway metadata, device readings, and data points.
The maximum number of data points, string lengths and maximum number of devices have been defined based on typical gateway reporting behaviour and identifier sizes expected in practice. They can be easily adjusted later based on system requirements. 

________________________________________________________________________________



Data Model and Internal Datatypes - 

The internal data model mirrors the required JSON structure and is implemented using explicit C structures with fixed-size arrays and counters. This approach ensures predictable memory usage and avoids dynamic allocation, which is desirable in embedded gateway firmware.
 
 
GatewayData -
Contains gateway-level metadata and all measurement values.
Fields:
gatewayId (char[MAX_STRING_LEN]) - Null-terminated string identifying the gateway
date (char[11]) - Date string in YYYY-MM-DD format
deviceType (char[MAX_STRING_LEN]) - Logical device type identifier
interval_minutes (uint16_t) - Reporting interval in minutes
total_readings (uint16_t) - Total number of readings reported
values (Values) - Container for device-related measurement data



Values -
Container for all device readings.
Fields:device_count (uint8_t) - Number of valid devices present 
readings (DeviceReading[MAX_DEVICES]) - Array of per-device measurement data
Only the first device_count entries in the array are considered valid.


DeviceReading -
media (char[MAX_STRING_LEN]) - Media type  
meter (char[MAX_STRING_LEN]) - Meter model or type
deviceId (char[MAX_STRING_LEN]) - Unique device identifier
unit (char[8]) - Measurement unit (e.g., m3)
data_count (uint8_t) - Number of valid data points
data (DataPoint[MAX_DATA_POINTS]) - Array of measurement data points

DataPoint -
timestamp (char[20]) - Measurement timestamp
meter_datetime (char[20]) - Timestamp reported by the meter
total_m3 (float) - Total meter value
status (char[8]) - Status indicator 

 
 
JSON Output -
[
  {
    "gatewayId": "gateway_1234",
    "date": "1970-01-01",
    "deviceType": "stromleser",
    "interval_minutes": 15,
    "total_readings": 1,
    "values": {
      "device_count": 1,
      "readings": [
        {
          "media": "water",
          "meter": "waterstarm",
          "deviceId": "stromleser_50898527",
          "unit": "m3",
          "data": [
            {
              "timestamp": "1970-01-01 00:00",
              "meter_datetime": "1970-01-01 00:00",
              "total_m3": 107.752,
              "status": "OK"
            }
          ]
        }
      ]
    }
  }
]

The output shown above is generated by the provided example application and matches the required format exactly.


Public API – serialize_gateway_data - 

SerializerStatus serialize_gateway_data
(
	const GatewayData *data,
	char *buffer_out, 
	uint16_t buffer_size,
	uint16_t *written_length
);


Parameters
    • data: pointer to populated gateway data structure
    • output_buffer: caller-provided buffer for JSON output
    • buffer_size: size of the output buffer in bytes
    • written_length: number of characters written on success
Return values
    • SERIALIZER_OK
    • SERIALIZER_BUFFER_SMALL
    • SERIALIZER_NULLPTR
The API is transport-agnostic and performs all bounds checking internally to prevent buffer overflows.
The serializer validates all input pointers and ensures that output never exceeds the provided buffer size. Errors are reported using an explicit return status enum, allowing the caller to handle failures cleanly without undefined behavior.

________________________________________________________________________________


Build and Run instructions - 

This repository contains two entry points, each serving a different purpose:
    1. Host-side demo (examples/host_demo.c)
Used to validate the JSON serialization logic and inspect the generated output easily.
    2. Embedded firmware entry point (src/main.c)
Intended for integration into an STM32 firmware project.
1. Host-Side Demo (GCC)
The host-side demo is provided to make it easy to verify correctness of the serializer without requiring embedded hardware. It creates sample input data, calls the serialization API, and prints the generated JSON to the terminal.
Build and run -
gcc examples/host_demo.c src/json_serializer.c -Iinclude -o host_demo
./host_demo
Running this demo prints the serialized JSON output directly to the console, allowing easy comparison with the required JSON format.
This demo is intended only for validation and testing. The serialization library itself does not depend on desktop functionality.
2. Embedded Firmware Entry Point (STM32)
The src/main.c file represents an embedded firmware entry point as typically used in STM32 projects. It demonstrates how the serializer would be invoked from firmware code.
Due to the absence of actual hardware in this assignment, the embedded main.c does not perform output over UART or other peripherals. Instead, it focuses on successful compilation and correct integration of the serialization library within an embedded build environment.
The same serialization source files compile unchanged using the STM32CubeIDE toolchain.
________________________________________________________________________________


Design Decisions and Assumptions - 
Fixed-size arrays are used instead of dynamic allocation to ensure predictable memory usage.
    • Explicit counters (device_count, data_count) describe valid elements in arrays.
    • No heap memory allocation is performed.
    • Serialization is fully deterministic in both time and memory.
    • The serializer is transport-agnostic and unaware of UART, SPI, I²C, or networking layers.
    • Floating-point values are formatted manually to guarantee numeric JSON output and avoid locale issues.
The implementation assumes that the maximum sizes defined in the header file are sufficient for typical gateway reporting scenarios.
________________________________________________________________________________

Notes on possible extensions - 
The current implementation focuses strictly on deterministic serialization logic. Possible future extensions include:
    • Transport-specific adapter layers (UART, SPI, I²C, MQTT) can be added externally to forward the serialized JSON without modifying the core serialization logic.
    • Size limits could be made configurable at compile time to support different gateway profiles without increasing runtime complexity.
    • A helper API could be added to estimate the required JSON buffer size before serialization, improving robustness and simplifying integration
    • Automated unit tests comparing generated JSON against known-good output can be added to ensure long-term stability. 
________________________________________________________________________________
