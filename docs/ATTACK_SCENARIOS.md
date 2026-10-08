# Controlled Attack Scenarios

The repository includes a deterministic generator for defensive testing.

## Unknown-ID injection
A frame uses an arbitration ID absent during baseline learning. Expected result: CAN_ALERT_UNKNOWN_ID.

## DLC tampering
A known message ID is transmitted with an unexpected DLC. Expected result: CAN_ALERT_DLC_MISMATCH.

## Payload tampering
A known message changes selected payload bytes while retaining the same ID/DLC. Expected result: payload anomaly when the learned distribution provides enough separation.

## Flood / burst
Traffic frequency is intentionally increased inside the detector short burst window. Expected result: CAN_ALERT_BURST once the configured frame-count threshold is exceeded.

These scenarios are for defensive validation. They do not implement packet injection against a live vehicle or target network.
