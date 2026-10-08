#include <stdio.h>
#include "can_frame.h"
#include "can_baseline.h"
#include "can_detector.h"
#include "can_logger.h"
#include "can_metrics.h"
#include "can_idsim.h"

static void train(can_baseline_t *b) {
    for (uint32_t n=0;n<40;++n) {
        can_frame_t f;
        can_generate_frame(&f,n,(n%2u==0u)?(uint64_t)n*10000ULL:(uint64_t)n*10000ULL,SCENARIO_NORMAL);
        can_baseline_update(b,&f);
    }
}
static void inspect(can_detector_t *d,const can_baseline_t *b,can_alert_log_t *log,
                    can_metrics_t *m,can_frame_t *f,int expected_attack) {
    can_alert_t a;
    can_alert_type_t t=can_detector_process(d,b,f,&a);
    can_metrics_record_frame(m);
    if (t!=CAN_ALERT_NONE) {
        can_alert_log_push(log,&a);
        can_metrics_record_alert(m, 25, expected_attack);
        printf("ALERT id=0x%03X type=%d score=%u latency=25us\n",a.id,a.type,a.score);
    }
}
int main(void) {
    can_baseline_t b; can_detector_t d; can_alert_log_t log; can_metrics_t m;
    can_baseline_init(&b,40);
    train(&b);
    can_detector_init(&d,(can_detector_config_t){.period_score=20,.payload_score=20,.burst_score=12,.threshold=60});
    can_alert_log_init(&log); can_metrics_init(&m);

    for (uint32_t n=40;n<80;++n) {
        can_frame_t f; can_generate_frame(&f,n,(uint64_t)n*10000ULL,SCENARIO_NORMAL);
        inspect(&d,&b,&log,&m,&f,0);
    }
    can_frame_t attack;
    can_generate_frame(&attack,81,810000ULL,SCENARIO_ID_INJECTION);
    inspect(&d,&b,&log,&m,&attack,1);
    can_generate_frame(&attack,82,820000ULL,SCENARIO_DLC_TAMPER);
    inspect(&d,&b,&log,&m,&attack,1);
    can_generate_frame(&attack,83,830000ULL,SCENARIO_PAYLOAD_TAMPER);
    inspect(&d,&b,&log,&m,&attack,1);

    printf("CAN IDS demo: frames=%u alerts=%u avg_detection_latency=%.1fus FPR=%.3f\n",
           m.total_frames,m.alerts,can_metrics_average_detection_latency_us(&m),
           can_metrics_false_positive_rate(&m));
    return 0;
}
