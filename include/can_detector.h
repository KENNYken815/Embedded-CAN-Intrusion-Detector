#ifndef CAN_DETECTOR_H
#define CAN_DETECTOR_H
#include <stdint.h>
#include <stdbool.h>
#include "can_baseline.h"
typedef enum {
    CAN_ALERT_NONE = 0,
    CAN_ALERT_UNKNOWN_ID,
    CAN_ALERT_DLC_MISMATCH,
    CAN_ALERT_PERIOD_ANOMALY,
    CAN_ALERT_PAYLOAD_ANOMALY,
    CAN_ALERT_BURST
} can_alert_type_t;
typedef struct {
    can_alert_type_t type;
    uint32_t id;
    uint32_t score;
    uint64_t timestamp_us;
} can_alert_t;
typedef struct {
    uint32_t period_score;
    uint32_t payload_score;
    uint32_t burst_score;
    uint32_t threshold;
} can_detector_config_t;
typedef struct {
    can_detector_config_t config;
    uint32_t recent_frames;
    uint64_t window_start_us;
} can_detector_t;
void can_detector_init(can_detector_t *d, can_detector_config_t config);
can_alert_type_t can_detector_process(can_detector_t *d, const can_baseline_t *b,
                                      const can_frame_t *frame, can_alert_t *alert);
