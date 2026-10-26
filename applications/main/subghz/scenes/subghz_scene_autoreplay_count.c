#include "../subghz_i.h"
#include "../helpers/subghz_custom_event.h"

#define AUTOREPLAY_COUNT_MIN 1
#define AUTOREPLAY_COUNT_MAX 999

static void subghz_scene_autoreplay_count_input_callback(void* context, int32_t number) {
    furi_assert(context);
    SubGhz* subghz = context;
    if(number >= AUTOREPLAY_COUNT_MIN && number <= AUTOREPLAY_COUNT_MAX) {
        subghz->last_settings->autoreplay_count = (uint32_t)number;
    }
    view_dispatcher_send_custom_event(subghz->view_dispatcher, SubGhzCustomEventByteInputDone);
}

void subghz_scene_autoreplay_count_on_enter(void* context) {
    SubGhz* subghz = context;

    uint32_t current = subghz->last_settings->autoreplay_count;
    if(current < (uint32_t)AUTOREPLAY_COUNT_MIN) current = 1;
    if(current > (uint32_t)AUTOREPLAY_COUNT_MAX) current = AUTOREPLAY_COUNT_MAX;

    number_input_set_header_text(subghz->number_input, "Autoreplay Count");
    number_input_set_result_callback(
        subghz->number_input,
        subghz_scene_autoreplay_count_input_callback,
        subghz,
        (int32_t)current,
        AUTOREPLAY_COUNT_MIN,
        AUTOREPLAY_COUNT_MAX);

    view_dispatcher_switch_to_view(subghz->view_dispatcher, SubGhzViewIdNumberInput);
}

bool subghz_scene_autoreplay_count_on_event(void* context, SceneManagerEvent event) {
    SubGhz* subghz = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubGhzCustomEventByteInputDone) {
            scene_manager_previous_scene(subghz->scene_manager);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(subghz->scene_manager);
        consumed = true;
    }

    return consumed;
}

void subghz_scene_autoreplay_count_on_exit(void* context) {
    UNUSED(context);
}
