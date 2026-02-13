#include QMK_KEYBOARD_H
#include "rgblight.h"
#include "print.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_NO, KC_F14, KC_F15, KC_BACKSPACE,P
        KC_NUM_LOCK, KC_KP_ASTERIST, KC_KP_SLASH, KC_KP_MINUS,
        KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS,
        KC_KP_4, KC_KP_5, KC_KP_6, KC_NO,
        KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER,
        KC_KP_0, KC_NO, KC_DOT, KC_NO
    )
};

void keyboard_post_init_user(void) {
    // optional debug
    debug_enable = true;
    debug_matrix = true;
    debug_keyboard = true;

    // startup RGB
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(170, 255, 80);

    for (uint8_t i = 0; i < 3; i++) {
        rgblight_toggle_noeeprom();
        wait_ms(120);
        rgblight_toggle_noeeprom();
        wait_ms(120);
    }

    rgblight_sethsv_noeeprom(85, 255, 80);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uprintf("key %s kc=%u row=%u col=%u\n",
            record->event.pressed ? "DOWN" : "UP",
            keycode,
            record->event.key.row,
            record->event.key.col);
    return true;
}
