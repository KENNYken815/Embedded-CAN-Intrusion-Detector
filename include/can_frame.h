#ifndef CAN_FRAME_H
#define CAN_FRAME_H
#include <stdint.h>
#include <stdbool.h>
#define CAN_MAX_DATA 64u
typedef struct {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[CAN_MAX_DATA];
    uint64_t timestamp_us;
    bool fd;
} can_frame_t;
uint64_t can_hash_payload(const can_frame_t *frame);
