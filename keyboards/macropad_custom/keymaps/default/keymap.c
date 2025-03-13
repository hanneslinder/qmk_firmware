// keymap.c
#include QMK_KEYBOARD_H

// Just a single layer for now
enum layers { _BASE };

#define RESET_KEY KC_F24

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[_BASE] = LAYOUT(RESET_KEY, KC_F23, KC_F22, KC_F21, KC_F20, KC_F19, KC_F18, KC_F17, KC_F16, KC_MUTE, QK_UNDERGLOW_TOGGLE)};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t bootloader_key_timer          = 0;
    static bool     bootloader_other_key_recorded = false;

    if (keycode != RESET_KEY) {
        uprintf("Other key pressed %s", keycode);
        bootloader_other_key_recorded = true;
    }

    switch (keycode) {
        case RESET_KEY:
            if (record->event.pressed) {
                uprintf("start timer");
                bootloader_key_timer          = timer_read();
                bootloader_other_key_recorded = false;
            } else {
                if (!bootloader_other_key_recorded) {
                    if (timer_elapsed(bootloader_key_timer) > 500 && timer_elapsed(bootloader_key_timer) < 2000) {
                        uprintf("case 1");

                        rgblight_sethsv(HSV_RED);
                    } else if (timer_elapsed(bootloader_key_timer) >= 2000) {
                        uprintf("case 2");
                        rgblight_sethsv(HSV_BLUE);
                        bootloader_jump();
                    }
                }
                bootloader_key_timer = 0;
            }
            break;
        default:
            break;
    }

    return true;
}

// // Encoder functionality
bool encoder_update_user(uint8_t index, bool clockwise) {
    uprintf("Encoder index: %d, Direction: %s\n", index, clockwise ? "CW" : "CCW");

    // First encoder
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        if (clockwise) {
            rgblight_increase_hue();
        } else {
            rgblight_decrease_hue();
        }
    }
    return false;
}

void keyboard_pre_init_user(void) {
    setPinInputHigh(ENCODER_BUTTON_A);
    setPinInputHigh(ENCODER_BUTTON_B);
}
