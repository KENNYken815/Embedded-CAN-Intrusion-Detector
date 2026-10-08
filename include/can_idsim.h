#ifndef CAN_IDSIM_H
#define CAN_IDSIM_H
#include <stdint.h>
#include "can_frame.h"
typedef enum {
    SCENARIO_NORMAL = 0,
    SCENARIO_ID_INJECTION,
    SCENARIO_FLOOD,
    SCENARIO_DLC_TAMPER,
    SCENARIO_PAYLOAD_TAMPER
} can_attack_scenario_t;
uint32_t can_generate_frame(can_frame_t *frame, uint32_t sequence, uint64_t time_us,
                            can_attack_scenario_t scenario);
