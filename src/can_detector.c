#include "can_detector.h"
#include <math.h>
#include <string.h>

static uint32_t clamp_score(double x) {
    if (x < 0.0) return 0;
    if (x > 100.0) return 100;
    return (uint32_t)x;
}
void can_detector_init(can_detector_t *d, can_detector_config_t config) {
    if (!d) return;
    memset(d, 0, sizeof(*d));
    d->config=config;
}
can_alert_type_t can_detector_process(can_detector_t *d, const can_baseline_t *b,
                                      const can_frame_t *frame, can_alert_t *alert) {
    if (!d || !b || !frame || !alert) return CAN_ALERT_NONE;
    *alert=(can_alert_t){0};
    alert->id=frame->id; alert->timestamp_us=frame->timestamp_us;

    const can_feature_state_t *s=can_baseline_find(b, frame->id);
    if (!s) { alert->type=CAN_ALERT_UNKNOWN_ID; alert->score=100; return alert->type; }
    if (frame->dlc != s->expected_dlc) { alert->type=CAN_ALERT_DLC_MISMATCH; alert->score=100; return alert->type; }

    double period=frame->timestamp_us >= s->last_timestamp_us ?
                  (double)(frame->timestamp_us-s->last_timestamp_us) : 0.0;
    double pstd=can_feature_period_stddev(s);
    if (s->count >= 4 && pstd > 1.0) {
        double z=fabs(period-s->mean_period_us)/pstd;
        if (z >= 3.0) { alert->type=CAN_ALERT_PERIOD_ANOMALY; alert->score=clamp_score(z*20.0); return alert->type; }
    }

    double payload=(double)can_hash_payload(frame);
    double hstd=can_feature_payload_stddev(s);
    if (s->count >= 4 && hstd > 1.0) {
        double z=fabs(payload-s->mean_payload_hash)/hstd;
        if (z >= 3.0) { alert->type=CAN_ALERT_PAYLOAD_ANOMALY; alert->score=clamp_score(z*20.0); return alert->type; }
    }

    if (d->window_start_us == 0 || frame->timestamp_us-d->window_start_us > 10000ULL) {
        d->window_start_us=frame->timestamp_us; d->recent_frames=0;
    }
    d->recent_frames++;
    if (d->recent_frames > d->config.burst_score) {
        alert->type=CAN_ALERT_BURST; alert->score=100; return alert->type;
    }
    return CAN_ALERT_NONE;
}
