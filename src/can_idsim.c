#include "can_idsim.h"
#include <string.h>
static void base_payload(can_frame_t *f, uint32_t n) {
    f->dlc=8; for (uint8_t i=0;i<8;++i) f->data[i]=(uint8_t)((n+i*17u)&0xFFu);
}
uint32_t can_generate_frame(can_frame_t *frame, uint32_t sequence, uint64_t time_us,
                            can_attack_scenario_t scenario) {
    if (!frame) return 0;
    memset(frame,0,sizeof(*frame));
    frame->timestamp_us=time_us;
    frame->id=(sequence%2u==0u)?0x100u:0x200u;
    base_payload(frame,sequence);
    switch (scenario) {
        case SCENARIO_ID_INJECTION: frame->id=0x666u; break;
        case SCENARIO_FLOOD: frame->id=0x100u; break;
        case SCENARIO_DLC_TAMPER: frame->dlc=4; break;
        case SCENARIO_PAYLOAD_TAMPER: frame->data[0]^=0xA5u; frame->data[1]^=0x5Au; break;
        case SCENARIO_NORMAL: default: break;
    }
    return frame->id;
}
