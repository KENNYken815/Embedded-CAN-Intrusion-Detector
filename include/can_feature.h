#ifndef CAN_FEATURE_H
#define CAN_FEATURE_H
#include <stdint.h>
#include "can_frame.h"
typedef struct {
    uint32_t id;
    uint64_t last_timestamp_us;
    uint32_t count;
    double mean_period_us;
    double period_m2;
    double mean_payload_hash;
    double payload_hash_m2;
    uint8_t expected_dlc;
    bool initialized;
} can_feature_state_t;
void can_feature_init(can_feature_state_t *s, uint32_t id);
void can_feature_update(can_feature_state_t *s, const can_frame_t *frame);
double can_feature_period_stddev(const can_feature_state_t *s);
double can_feature_payload_stddev(const can_feature_state_t *s);
