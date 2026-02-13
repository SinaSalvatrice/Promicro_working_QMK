#include QMK_KEYBOARD_H
#include "rgblight.h"

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_MS_WH_DOWN); // oder UP/DOWN tauschen, wenn Richtung falsch
        } else {
            tap_code(KC_MS_WH_UP);
        }
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_NO, KC_F14, KC_F15, KC_BACKSLASH,
        KC_NUM_LOCK, KC_KP_ASTERISK, KC_KP_SLASH, KC_KP_MINUS,
        KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_PLUS,
        KC_KP_4, KC_KP_5, KC_KP_6, KC_NO,
        KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_ENTER,
        KC_KP_0, KC_NO, , KC_KP_DOT, KC_NO
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


