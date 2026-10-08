#include "can_metrics.h"
#include <string.h>
void can_metrics_init(can_metrics_t *m) { if (m) memset(m,0,sizeof(*m)); }
void can_metrics_record_frame(can_metrics_t *m) { if (m) m->total_frames++; }
void can_metrics_record_alert(can_metrics_t *m, uint64_t latency_us, int classified_correctly) {
    if (!m) return;
    m->alerts++; m->detection_latency_sum_us+=latency_us;
    if (latency_us > m->max_detection_latency_us) m->max_detection_latency_us=latency_us;
    if (classified_correctly) m->true_positives++; else m->false_positives++;
}
double can_metrics_false_positive_rate(const can_metrics_t *m) {
    uint32_t negatives=m ? m->total_frames-m->true_positives : 0;
    return (!m || negatives==0) ? 0.0 : (double)m->false_positives/(double)negatives;
}
double can_metrics_average_detection_latency_us(const can_metrics_t *m) {
    return (!m || m->alerts==0) ? 0.0 : (double)m->detection_latency_sum_us/(double)m->alerts;
}
