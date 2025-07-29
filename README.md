README - CAN Bus Coding & Decoding Validation

Author: Imed  
Date: July 29, 2025  

📌 Description:  
This embedded C project verifies and validates CAN bus message encoding and decoding between an STM32F446 microcontroller and an ECU. It also integrates data from agricultural sensors (e.g., soil moisture) and simulates fuel actuator behavior with diagnostics and GPS processing.

📁 Files Included:
- Core/Src/main.c: Main logic including CAN command handling, GPS scaling, diagnostics encoding, and console output.
- Core/Inc/main.h: Header declarations.
- vehiculefulelectric.h: Fuel actuator signal definitions and utilities.
- agrisensorcan.h: CAN-based agri sensor diagnostics.
- README.txt: Documentation.
- Drivers/: STM32 HAL and CMSIS drivers.

🛠️ How to Build:
1. Requirements:  
   - STM32CubeIDE or compatible ARM GCC toolchain  
   - STM32F446 board with CAN transceiver  
   - UART terminal for debugging  

2. Steps:  
   - Import the project into STM32CubeIDE.  
   - Ensure correct clock configuration and peripheral initialization.  
   - Build the project with IDE or run `make` if using CLI.

▶️ How to Run:
- Flash the firmware to the STM32F446 target.  
- Connect the CAN transceiver and ECU.  
- Open UART console (115200 baud) to view runtime data: soil moisture, fuel level, GPS coordinates, diagnostic flags, and fuel economy index.

🔗 Features Demonstrated:
- Encoding fuel actuator messages using structured data.
- GPS coordinate scaling and conversion.
- Real-time diagnostics flag transmission.
- CAN command dispatch with HAL CAN driver.
- Formatted UART output using `printf`.

⚠️ Notes & Warnings:
- Ensure `stdio.h` is included to support `printf` for UART debug output.
- The CAN interface should be properly configured with bus timing parameters (Prescaler, SJW, Segments).
- ECU should be powered and communicating over the correct CAN ID (0x321).
- Sensor values and GPS data are currently hardcoded for demonstration.

📬 Contact:
For collaboration or technical queries, reach out to https://github.com/mraihiimed/.

📜 License:
See LICENSE file. Default usage under STMicroelectronics terms or AS-IS if none present.
