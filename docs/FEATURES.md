# Extracted CAN Features

For each CAN frame the project models arbitration ID, DLC, payload bytes, arrival timestamp in microseconds, inter-arrival period for known IDs, running mean/variance of period, and payload hash with running variance.

The baseline uses online Welford-style statistics so it does not need to store every training frame.

Detection rules include unknown arbitration ID, DLC mismatch, statistically unusual inter-arrival period, statistically unusual payload hash, and burst-rate anomaly.

The statistical thresholds are intentionally simple and explainable for an embedded reference implementation.
