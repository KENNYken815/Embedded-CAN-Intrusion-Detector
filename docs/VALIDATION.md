# Validation Plan

For portfolio benchmarking, report:
- detection rate per attack scenario;
- false-positive rate on held-out normal traffic;
- mean and maximum detection latency;
- frames processed per second;
- CPU utilization on the target;
- RAM used by baseline/state tables;
- alert-log overflow count.

Recommended experiment:
1. Train the baseline with normal traffic.
2. Replay a separate normal trace.
3. Replay each controlled attack scenario independently.
4. Record alerts and timestamps.
5. Compute false positives, true positives and latency.
6. Repeat with different thresholds and traffic mixes.

The reference demo uses deterministic timestamps and a fixed 25 us example detection latency for metric plumbing; that value is illustrative and is not a hardware measurement.
