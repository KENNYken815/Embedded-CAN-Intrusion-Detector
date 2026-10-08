# Embedded CAN Intrusion Detector

An embedded C reference implementation for a defensive CAN/CAN-FD security node. The project passively models CAN traffic, learns a baseline of normal message behavior, extracts timing and payload features, detects anomalies, logs security alerts, and exposes metrics for detection latency and false-positive analysis.

> **Scope:** Defensive research and portfolio reference implementation. It does not attack a live CAN network, inject traffic into vehicles, or claim automotive cybersecurity certification.

## Architecture

    CAN / CAN-FD
         |
    ISR / DMA RX path
         |
    Frame ring buffer
         |
    Feature extraction
         |
    Normal baseline
         |
    Anomaly detector
       /   |    |    \
    ID  DLC  timing payload  burst
       \   |    |    /
         Alert engine
             |
       +-----+------+
       |            |
    Alert log    Metrics

The portable C core is deliberately separated from STM32-specific HAL/FDCAN code so the detector can later be connected to an STM32G4/H7 target.

## Implemented Features

### CAN frame modeling
Each frame contains arbitration ID, DLC, payload bytes, timestamp in microseconds, and a CAN-FD flag.

### Baseline learning
During a configurable learning phase, the detector learns which message IDs exist and maintains per-ID statistics without storing every frame.

Tracked features include:
- message ID presence;
- expected DLC;
- inter-arrival period;
- mean/variance of message timing;
- payload fingerprint statistics.

### Statistical anomaly detection
The reference detector checks for:
- unknown message IDs;
- DLC mismatches;
- abnormal inter-arrival timing;
- abnormal payload fingerprints;
- short-window traffic bursts.

The statistical logic is intentionally explainable and lightweight enough to map onto an MCU processing task.

### Security alert logging
Detected anomalies become structured alerts containing:
- alert type;
- CAN ID;
- anomaly score;
- timestamp.

A bounded alert log tracks overflow instead of allocating memory dynamically.

### Detection metrics
The metrics module tracks:
- total frames;
- alerts;
- true positives;
- false positives;
- average detection latency;
- maximum detection latency.

This gives the project a direct portfolio benchmarking path rather than stopping at 'anomaly detected'.

## Controlled Attack Scenarios

The repository contains a deterministic scenario generator for defensive testing:

- **Unknown-ID injection:** introduces an ID absent from the learned baseline.
- **DLC tampering:** changes the expected payload length.
- **Payload tampering:** modifies payload bytes while preserving ID/DLC.
- **Flood/burst:** increases traffic density inside a short detector window.

These scenarios are synthetic and intended for offline validation only.

## Repository Structure

    Embedded-CAN-Intrusion-Detector/
    +-- docs/
    |   +-- ARCHITECTURE.md
    |   +-- ATTACK_SCENARIOS.md
    |   +-- FEATURES.md
    |   +-- RTOS_INTEGRATION.md
    |   +-- VALIDATION.md
    +-- examples/
    |   +-- canids_demo.c
    +-- include/
    |   +-- can_baseline.h
    |   +-- can_detector.h
    |   +-- can_feature.h
    |   +-- can_frame.h
    |   +-- can_idsim.h
    |   +-- can_logger.h
    |   +-- can_metrics.h
    +-- src/
    |   +-- can_baseline.c
    |   +-- can_detector.c
    |   +-- can_feature.c
    |   +-- can_frame.c
    |   +-- can_idsim.c
    |   +-- can_logger.c
    |   +-- can_metrics.c
    +-- tests/
    |   +-- test_canids.c
    +-- tools/
    |   +-- generate_attack_data.py
    +-- .gitignore
    +-- Makefile
    +-- README.md

## Build and Run

Requirements:
- C11 compiler such as GCC or Clang;
- GNU Make;
- Python 3 for offline dataset generation.

Build the demo and unit tests:

    make

Run the security-monitor demonstration:

    make demo

Run unit tests:

    make test

Generate synthetic attack-data CSV:

    python3 tools/generate_attack_data.py attack_data.csv

Clean generated files:

    make clean

No CI workflow is included, as requested.

## Typical Demo Flow

1. Train the baseline from deterministic normal traffic.
2. Replay normal frames.
3. Introduce controlled anomalous scenarios.
4. Convert anomalies into structured alerts.
5. Record example detection latency and classification metrics.
6. Report false-positive rate and alert count.

The demo intentionally uses a fixed example detection latency for metric plumbing. It is not an MCU timing measurement.

## STM32G4/H7 + FreeRTOS Integration

A target firmware can map the pipeline as:

    FDCAN RX interrupt / DMA
              |
        fixed frame queue
              |
          CanRxTask
              |
          CanIdsTask
          /       \
     baseline    detector
                    |
               CanAlertTask

Keep interrupt handlers short. Copy/queue the received frame and perform statistical analysis in task context. This avoids putting variable detector work directly inside the CAN ISR.

Suggested target components:
- STM32G4/H7 FDCAN peripheral;
- interrupt-driven RX FIFO;
- DMA where supported by the chosen data path;
- FreeRTOS queues/task notifications;
- timer or hardware timestamp source;
- optional UART/CAN-Ethernet bridge for host analysis.

The current repository does not claim a specific STM32 board or HAL implementation.

## Resource-Conscious Design

The baseline uses a fixed maximum number of tracked IDs and bounded alert storage. Statistics are updated online, avoiding a full historical frame database in RAM.

This makes the architecture suitable for evaluating:
- RAM footprint from per-ID state;
- CPU cost per received frame;
- maximum sustained frame rate;
- alert queue behavior under bursts.

A real deployment should additionally account for FDCAN filter configuration, cache/DMA coherency on the selected MCU, interrupt priority, watchdog behavior, and persistent logging policy.

## Validation / Portfolio Measurements

For a strong experiment, report:

- detection rate for each controlled scenario;
- false-positive rate using held-out normal traffic;
- mean/max detection latency;
- frames processed per second;
- CPU utilization;
- RAM usage;
- alert-log overflow count.

The repository's software metrics plumbing is ready for these measurements, but target CPU/RAM/timing numbers must be collected on actual hardware.

## Security Boundary

This project is a defensive CAN-monitoring reference. It is intended for controlled lab datasets and authorized testing. It does not contain tooling for compromising or disrupting real vehicle networks.

## License

MIT-style educational use; add your preferred license file before public release.
