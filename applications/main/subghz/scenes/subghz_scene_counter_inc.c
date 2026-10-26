#include "../subghz_i.h"
#include "../helpers/subghz_custom_event.h"

#define COUNTER_INC_MIN 1
#define COUNTER_INC_MAX 999

static void subghz_scene_counter_inc_callback(void* context, int32_t number) {
    SubGhz* subghz = context;
    subghz->last_settings->counter_inc = (uint32_t)number;
    furi_hal_subghz_set_rolling_counter_mult(number);
    subghz_last_settings_save(subghz->last_settings);
    view_dispatcher_send_custom_event(
        subghz->view_dispatcher, SubGhzCustomEventSceneSettingCounterInc);
}

void subghz_scene_counter_inc_on_enter(void* context) {
    SubGhz* subghz = context;
    uint32_t current =
        subghz->last_settings->counter_inc ? subghz->last_settings->counter_inc : 1;
    number_input_set_header_text(subghz->number_input, "Counter Inc (1-999)");
    number_input_set_result_callback(
        subghz->number_input,
        subghz_scene_counter_inc_callback,
        subghz,
        (int32_t)current,
        (int32_t)COUNTER_INC_MIN,
        (int32_t)COUNTER_INC_MAX);
    view_dispatcher_switch_to_view(subghz->view_dispatcher, SubGhzViewIdNumberInput);
}

bool subghz_scene_counter_inc_on_event(void* context, SceneManagerEvent event) {
    SubGhz* subghz = context;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubGhzCustomEventSceneSettingCounterInc) {
            scene_manager_previous_scene(subghz->scene_manager);
            return true;
        }
    }
    return false;
}

void subghz_scene_counter_inc_on_exit(void* context) {
    UNUSED(context);
}
