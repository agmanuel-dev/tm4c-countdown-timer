# TM4C123 countdown timer

A bare-metal C countdown demo using register-level GPIO, SysTick delays and software PWM. A button starts a seven-segment countdown, followed by an LED fade and hold.

**Focus:** embedded C, bit manipulation, peripheral registers and timing.

## Sequence

1. Detect a high button input, wait 20 ms and check it again.
2. Display 9 through 0 for one second each: ten displayed seconds in total.
3. Fade an indicator LED over approximately two seconds using 100 PWM periods of 20 ms.
4. Hold the LED on for five seconds, turn it off and wait for button release.

## Wiring and assumptions

| MCU pins | Function |
|---|---|
| PF1–PF3 | Seven-segment A–C |
| PB0–PB3 | Seven-segment D–G |
| PB4 | Indicator LED |
| PF4 | Active-high external pushbutton input |

Use current-limiting resistors and a compatible active-high/common-cathode display. The original external button circuit uses a 10 kΩ pull-down and a switch to 3.3 V. This is not the default active-low on-board SW1 arrangement; PF1–PF3 also share the LaunchPad RGB LED. Account for those connections in the wiring.

The code assumes a **16 MHz system clock**. Do not enable a PLL or change the clock without updating SysTick reload values.

## Build

Create a Keil µVision project for **TM4C123GH6PM** using your installed device pack and its device startup/linker configuration. Add `main.c` and `SysTick.c`; add this directory to the include path. The supplied header contains only the register definitions needed by this demo. Device startup and flashing tools are supplied by the toolchain, not this repository.

A source-level compiler check can also be run with Arm Compiler 6:

```text
armclang --target=arm-arm-none-eabi -mcpu=cortex-m4 -mthumb -std=c99 -Wall -Wextra -Werror -fsyntax-only main.c SysTick.c
```

## Verification and limitations

The command above passed without warnings on September 12, 2026. That checks C syntax and types for Cortex-M4; it is not a linked firmware image or a hardware test. Clock configuration, brightness, pin loading and electrical behavior need board verification.

Portfolio preparation corrected the original approximately ten-second fade to approximately two seconds, explicitly turns the LED on for the hold interval and adds a 20 ms button recheck. The blocking delay design intentionally does not handle concurrent tasks. Software PWM has millisecond quantization and a 50 Hz carrier; hardware PWM would be an extension. [Provenance](PROVENANCE.md).
