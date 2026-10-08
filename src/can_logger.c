#include "can_logger.h"
#include <string.h>
void can_alert_log_init(can_alert_log_t *log) { if (log) memset(log,0,sizeof(*log)); }
bool can_alert_log_push(can_alert_log_t *log, const can_alert_t *alert) {
    if (!log || !alert) return false;
    if (log->count >= CAN_ALERT_LOG_SIZE) { log->dropped++; return false; }
    log->alerts[log->count++]=*alert;
    return true;
}
