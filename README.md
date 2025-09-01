AVR Thermostat with High/Low Temperature Control (ATmega32 + LCD + Relays)

This project implements a digital thermostat system using an ATmega32 microcontroller, a temperature sensor (via ADC), a 16x2 LCD, and two relays.

The thermostat allows the user to:

Continuously monitor the current temperature (°C).

Define a High Temperature threshold (H).

Define a Low Temperature threshold (L).

When the measured temperature goes above H, one relay is activated.

When the measured temperature goes below L, another relay is activated.

If the temperature is within the normal range (L ≤ Temp ≤ H), both relays remain off.

🛠 Hardware Requirements

Microcontroller: ATmega32

LCD: 16x2 character LCD (alcd.h library)

Temperature Sensor: Analog type (e.g., LM35, TMP36, or similar connected to ADC0)

Relays (2x):

Relay 1 → Activated when temperature exceeds H

Relay 2 → Activated when temperature drops below L

Buttons (on PORTD):

PIND.0 → Increase value (increment H/L)

PIND.1 → Decrease value (decrement H/L)

PIND.2 → Back/Exit or Confirm selection

  💻 Code Written using CodeVisionAVR.

📽️ Video YouTube video link: https://youtu.be/GP6H9gPxkSc
