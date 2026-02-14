#include QMK_KEYBOARD_H
#include "rgblight.h"
#include "timer.h"
#include "lib/lib8tion/lib8tion.h"

/* Color palette (HSV, 0-255) - Turquoise + Yellow */
#define H_TURQ 128
#define H_YELL  43
#define SAT     255

static uint8_t dot_pos = 0;

/* RGB blend helper */
static inline RGB rgb_blend(RGB a, RGB b, uint8_t t) {
    RGB o;
    o.r = lerp8by8(a.r, b.r, t);
    o.g = lerp8by8(a.g, b.g, t);
    o.b = lerp8by8(a.b, b.b, t);
    return o;
}

/* Render RGB lights with breathing animation */
static void render_lights(void) {
    if (!rgblight_is_enabled()) {
        return;
    }

    const uint8_t n = RGBLIGHT_LED_COUNT;
    if (n == 0) return;

    uint8_t phase = (uint8_t)(timer_read() >> 3);
    uint8_t t     = sin8(phase);
    uint8_t br    = 40 + scale8(t, 215);

    RGB turq = hsv_to_rgb((HSV){ .h = H_TURQ, .s = SAT, .v = 255 });
    RGB yell = hsv_to_rgb((HSV){ .h = H_YELL, .s = SAT, .v = 255 });
    RGB base = rgb_blend(turq, yell, t);

    base.r = scale8(base.r, br);
    base.g = scale8(base.g, br);
    base.b = scale8(base.b, br);

    for (uint8_t i = 0; i < n; i++) {
        rgblight_setrgb_at(base.r, base.g, base.b, i);
    }

    uint8_t head = dot_pos % n;
    uint8_t tail = (head == 0) ? (n - 1) : (head - 1);

    rgblight_sethsv_at(H_YELL, SAT, 140, tail);
    rgblight_sethsv_at(H_TURQ, SAT, 255, head);

    rgblight_set();
}

/* Encoder update */
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            dot_pos = (dot_pos + 3) % RGBLIGHT_LED_COUNT;
            tap_code(KC_MS_WH_DOWN);
        } else {
            dot_pos = (dot_pos == 0) ? (RGBLIGHT_LED_COUNT - 1) : (dot_pos - 3);
            tap_code(KC_MS_WH_UP);
        }
        render_lights();
    }
    return true;
}

/* Encoder button debouncing */
static bool     enc_btn_last   = true;
static bool     enc_btn_stable = true;
static uint16_t enc_btn_timer  = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_NO, KC_F14, KC_F15, KC_BACKSPACE,
        KC_NUM, KC_KP_ASTERISK, KC_KP_SLASH, KC_KP_PLUS,
        KC_KP_7,   KC_KP_8,   KC_KP_9,   KC_KP_PLUS,
        KC_KP_4,   KC_KP_5,   KC_KP_6,   KC_KP_ENTER,
        KC_KP_1,   KC_KP_2,   KC_KP_3,   KC_KP_ENTER,
        KC_KP_0,   KC_KP_0,   KC_KP_DOT, KC_KP_DOT
    )
};

void keyboard_post_init_user(void) {
    setPinInputHigh(ENCODER_BTN_PIN);

    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHE);
    /* Startup: colorful pulse */
    for (uint8_t i = 0; i < RGBLIGHT_LED_COUNT; i++) {
        uint8_t hue = (uint8_t)((uint16_t)i * 150 / (RGBLIGHT_LED_COUNT ? RGBLIGHT_LED_COUNT : 1));
        rgblight_sethsv_at(hue, 255, 180, i);
        rgblight_set();
        wait_ms(40);
    }
    wait_ms(80);

    render_lights();
}

void matrix_scan_user(void) {
    /* Button debouncing */
    bool raw = readPin(ENCODER_BTN_PIN);

    if (raw != enc_btn_stable) {
        enc_btn_stable = raw;
        enc_btn_timer  = timer_read();
    }

    if (timer_elapsed(enc_btn_timer) > 25) {
        if (enc_btn_stable != enc_btn_last) {
            enc_btn_last = enc_btn_stable;

            if (!enc_btn_last) {
                rgblight_enable_noeeprom();
            }
        }
    }

    /* Update animation ~50 FPS */
    static uint16_t last = 0;
    if (timer_elapsed(last) > 20) {
        last = timer_read();
        render_lights();
    }
}
