#include QMK_KEYBOARD_H
#include "rgblight.h"
#include "print.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_A, KC_B, KC_C, KC_ENT,
        KC_1, KC_2, KC_3, KC_4,
        KC_5, KC_6, KC_7, KC_8,
        KC_9, KC_0, KC_DOT, KC_SPC,
        KC_ESC, KC_TAB, KC_BSPC, KC_DEL,
        KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
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
