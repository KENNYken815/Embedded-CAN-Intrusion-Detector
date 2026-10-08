#ifndef CAN_METRICS_H
#define CAN_METRICS_H
#include <stdint.h>
typedef struct {
    uint32_t total_frames;
    uint32_t alerts;
    uint32_t true_positives;
    uint32_t false_positives;
    uint64_t detection_latency_sum_us;
    uint64_t max_detection_latency_us;
} can_metrics_t;
void can_metrics_init(can_metrics_t *m);
void can_metrics_record_frame(can_metrics_t *m);
void can_metrics_record_alert(can_metrics_t *m, uint64_t latency_us, int classified_correctly);
double can_metrics_false_positive_rate(const can_metrics_t *m);
double can_metrics_average_detection_latency_us(const can_metrics_t *m);
