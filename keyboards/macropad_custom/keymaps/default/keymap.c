#include QMK_KEYBOARD_H

enum layers { _BASE, _RGB };

enum custom_keycodes { RGB_LAYER = SAFE_RANGE };

#define RESET_KEY KC_F24

#define BLINK_DURATION 150 // Duration of each blink in ms
#define BLINK_COUNT 2      // Number of blinks
#define BLINK_DELAY 100    // Delay between blinks in ms

uint8_t  saved_hue;
uint8_t  saved_sat;
uint8_t  saved_val;
bool     is_blinking   = false;
uint32_t blink_timer   = 0;
uint8_t  blink_state   = 0;
uint8_t  blink_counter = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[_BASE] = LAYOUT(RESET_KEY, KC_F23, KC_F22, KC_F21, KC_F20, KC_F19, KC_F18, QK_UNDERGLOW_VALUE_UP, QK_UNDERGLOW_VALUE_DOWN, KC_MUTE, RGB_LAYER), [_RGB] = LAYOUT(RESET_KEY, QK_UNDERGLOW_VALUE_UP, QK_UNDERGLOW_VALUE_DOWN, QK_UNDERGLOW_MODE_NEXT, QK_UNDERGLOW_MODE_PREVIOUS, QK_UNDERGLOW_TOGGLE, QK_UNDERGLOW_SATURATION_UP, QK_UNDERGLOW_SATURATION_DOWN, KC_NO, KC_MUTE, KC_TRNS)};

// Function to save current RGB state
void save_rgb_state(void) {
    saved_hue = rgblight_get_hue();
    saved_sat = rgblight_get_sat();
    saved_val = rgblight_get_val();
}

// Function to restore RGB state
void restore_rgb_state(void) {
    rgblight_sethsv(saved_hue, saved_sat, saved_val);
}

void trigger_rgb_blink(void) {
    if (!is_blinking) {
        save_rgb_state();
        is_blinking   = true;
        blink_state   = 0;
        blink_counter = 0;
        blink_timer   = timer_read32();
    }
}

// Handle the blinking in the matrix scan function
void matrix_scan_user(void) {
    // Only process if we're in blinking mode
    if (is_blinking) {
        // Check if it's time to change state
        if (timer_elapsed32(blink_timer) > (blink_state % 2 == 0 ? BLINK_DURATION : BLINK_DELAY)) {
            blink_timer = timer_read32();
            blink_state++;

            // Toggle LED state
            if (blink_state % 2 == 0) {
                // LED OFF state (or dimmed)
                uint8_t dimmed_val = saved_val > 50 ? 0 : saved_val + 100; // Significant contrast
                rgblight_sethsv(saved_hue, saved_sat, dimmed_val);
                blink_counter++; // Count completed blinks
            } else {
                // LED ON state
                restore_rgb_state();
            }

            // Check if we're done blinking
            if (blink_counter >= BLINK_COUNT) {
                is_blinking = false;
                restore_rgb_state();
            }
        }
    }
}

// Modify your layer state function to trigger the blink
layer_state_t layer_state_set_user(layer_state_t state) {
    // Detect layer changes
    layer_state_t old_state = layer_state;
    if (state != old_state) {
        // Layer has changed, trigger the blink
        trigger_rgb_blink();

        // Optional: Change RGB mode based on layer
        if (layer_state_cmp(state, _RGB)) {
            // Save state first, so we restore properly after blinking
            save_rgb_state();
        } else {
            save_rgb_state();
        }
    }

    return state;
}

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
        case RGB_LAYER:
            if (record->event.pressed) {
                layer_invert(_RGB);
            }
            return false;
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

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
}
