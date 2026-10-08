#ifndef CAN_LOGGER_H
#define CAN_LOGGER_H
#include <stdint.h>
#include <stdbool.h>
#include "can_detector.h"
#define CAN_ALERT_LOG_SIZE 128u
typedef struct {
    can_alert_t alerts[CAN_ALERT_LOG_SIZE];
    uint32_t count;
    uint32_t dropped;
} can_alert_log_t;
void can_alert_log_init(can_alert_log_t *log);
bool can_alert_log_push(can_alert_log_t *log, const can_alert_t *alert);
