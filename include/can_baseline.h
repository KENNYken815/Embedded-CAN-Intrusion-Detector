#ifndef CAN_BASELINE_H
#define CAN_BASELINE_H
#include <stdint.h>
#include <stdbool.h>
#include "can_feature.h"
#define CAN_MAX_TRACKED_IDS 64u
typedef struct {
    can_feature_state_t states[CAN_MAX_TRACKED_IDS];
    uint32_t count;
    uint32_t learning_frames;
} can_baseline_t;
void can_baseline_init(can_baseline_t *b, uint32_t learning_frames);
can_feature_state_t *can_baseline_get(can_baseline_t *b, uint32_t id);
const can_feature_state_t *can_baseline_find(const can_baseline_t *b, uint32_t id);
bool can_baseline_learning_complete(const can_baseline_t *b);
int can_baseline_update(can_baseline_t *b, const can_frame_t *frame);
