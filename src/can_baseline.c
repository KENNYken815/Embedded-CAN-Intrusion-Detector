#include "can_baseline.h"
#include <string.h>
void can_baseline_init(can_baseline_t *b, uint32_t learning_frames) {
    if (!b) return;
    memset(b, 0, sizeof(*b));
    b->learning_frames = learning_frames;
}
can_feature_state_t *can_baseline_get(can_baseline_t *b, uint32_t id) {
    if (!b) return NULL;
    for (uint32_t i=0;i<b->count;++i) if (b->states[i].id==id) return &b->states[i];
    if (b->count >= CAN_MAX_TRACKED_IDS) return NULL;
    can_feature_init(&b->states[b->count], id);
    return &b->states[b->count++];
}
const can_feature_state_t *can_baseline_find(const can_baseline_t *b, uint32_t id) {
    if (!b) return NULL;
    for (uint32_t i=0;i<b->count;++i) if (b->states[i].id==id) return &b->states[i];
    return NULL;
}
bool can_baseline_learning_complete(const can_baseline_t *b) {
    if (!b) return false;
    uint32_t frames=0;
    for (uint32_t i=0;i<b->count;++i) frames += b->states[i].count;
    return frames >= b->learning_frames;
}
int can_baseline_update(can_baseline_t *b, const can_frame_t *frame) {
    if (!b || !frame || frame->dlc > CAN_MAX_DATA) return -1;
    can_feature_state_t *s=can_baseline_get(b, frame->id);
    if (!s) return -2;
    can_feature_update(s, frame);
    return 0;
}
