#include "can_frame.h"
uint64_t can_hash_payload(const can_frame_t *frame) {
    if (!frame) return 0;
    uint64_t h = 1469598103934665603ULL;
    for (uint8_t i = 0; i < frame->dlc && i < CAN_MAX_DATA; ++i) {
        h ^= frame->data[i];
        h *= 1099511628211ULL;
    }
    return h;
}
