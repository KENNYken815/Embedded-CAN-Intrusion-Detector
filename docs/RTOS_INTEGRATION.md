# FreeRTOS Integration

A practical STM32G4/H7 firmware can use:

CAN RX ISR -> fixed-size frame ring buffer -> task notification/semaphore -> CAN IDS processing task -> alert queue -> diagnostic/telemetry task.

Suggested tasks:
- CanRxTask: drains DMA/ISR-produced frames.
- CanIdsTask: feature extraction, baseline and detection.
- CanAlertTask: logging, diagnostics and optional host telemetry.

Do not perform expensive statistical work inside the CAN ISR. Keep interrupt handling bounded and defer analysis to task context.

The repository models the processing layer rather than MCU-specific HAL, FDCAN registers, cache management or FreeRTOS APIs.
