#include "can_feature.h"
#include <math.h>
#include <string.h>

static void update_mean_m2(double x, uint32_t n, double *mean, double *m2) {
    double delta = x - *mean;
    *mean += delta / (double)n;
    *m2 += delta * (x - *mean);
}
void can_feature_init(can_feature_state_t *s, uint32_t id) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
    s->id = id;
}
void can_feature_update(can_feature_state_t *s, const can_frame_t *frame) {
    if (!s || !frame) return;
    if (!s->initialized) {
        s->initialized = true;
        s->expected_dlc = frame->dlc;
        s->last_timestamp_us = frame->timestamp_us;
        s->count = 1;
        s->mean_payload_hash = (double)can_hash_payload(frame);
        return;
    }
    uint64_t period = frame->timestamp_us >= s->last_timestamp_us ?
                      frame->timestamp_us - s->last_timestamp_us : 0;
    s->last_timestamp_us = frame->timestamp_us;
    s->count++;
    update_mean_m2((double)period, s->count - 1u, &s->mean_period_us, &s->period_m2);
    update_mean_m2((double)can_hash_payload(frame), s->count, &s->mean_payload_hash, &s->payload_hash_m2);
}
double can_feature_period_stddev(const can_feature_state_t *s) {
    if (!s || s->count < 3) return 0.0;
    return sqrt(s->period_m2 / (double)(s->count - 2u));
}
double can_feature_payload_stddev(const can_feature_state_t *s) {
    if (!s || s->count < 2) return 0.0;
    return sqrt(s->payload_hash_m2 / (double)(s->count - 1u));
}
