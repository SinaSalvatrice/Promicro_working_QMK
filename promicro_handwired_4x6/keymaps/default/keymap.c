#include QMK_KEYBOARD_H
#include "rgblight.h"

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
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

    // einmal rot -> grün (sichtbarer Boot)
    rgblight_sethsv_noeeprom(0, 255, 80);
    wait_ms(150);
    rgblight_sethsv_noeeprom(85, 255, 80);
}
