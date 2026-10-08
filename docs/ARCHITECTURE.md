# Architecture

CAN/CAN-FD peripheral -> interrupt/DMA receive path -> frame normalization -> feature extraction -> learned baseline -> anomaly detector -> alert logger and metrics.

Recommended STM32G4/H7 firmware mapping:
- RX FIFO/interrupt handles arrival with minimal work.
- Optional DMA moves frames into a fixed ring buffer.
- A FreeRTOS task drains the ring and runs detection.
- Alerts are emitted through a diagnostic/event interface.
- Baseline state remains in RAM and can be persisted only after explicit policy decisions.

The reference implementation is platform-neutral C. No MCU register access is embedded in the algorithmic layer.
