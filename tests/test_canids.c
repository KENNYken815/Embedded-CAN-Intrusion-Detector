#include <assert.h>
#include <stdio.h>
#include "can_baseline.h"
#include "can_detector.h"
#include "can_idsim.h"
#include "can_metrics.h"
#include "can_logger.h"

static void train(can_baseline_t *b) {
    for (uint32_t n=0;n<32;++n) {
        can_frame_t f; can_generate_frame(&f,n,(uint64_t)n*10000ULL,SCENARIO_NORMAL);
        assert(can_baseline_update(b,&f)==0);
    }
}
int main(void) {
    can_baseline_t b; train(&b);
    assert(can_baseline_learning_complete(&b));
    can_detector_t d; can_detector_init(&d,(can_detector_config_t){.burst_score=12});
    can_alert_t a; can_frame_t f;

    can_generate_frame(&f,100,1000000ULL,SCENARIO_ID_INJECTION);
    assert(can_detector_process(&d,&b,&f,&a)==CAN_ALERT_UNKNOWN_ID);

    can_generate_frame(&f,101,1010000ULL,SCENARIO_DLC_TAMPER);
    assert(can_detector_process(&d,&b,&f,&a)==CAN_ALERT_DLC_MISMATCH);

    can_metrics_t m; can_metrics_init(&m);
    for(int i=0;i<10;++i) can_metrics_record_frame(&m);
    can_metrics_record_alert(&m,20,1);
    can_metrics_record_alert(&m,40,0);
    assert(can_metrics_average_detection_latency_us(&m)==30.0);
    assert(can_metrics_false_positive_rate(&m)>0.0);

    can_alert_log_t log; can_alert_log_init(&log);
    assert(can_alert_log_push(&log,&a));
    assert(log.count==1);
    puts("All CAN IDS tests passed.");
    return 0;
}
