#pragma once

#define ENCODER_A_PINS {B3, F7}
#define ENCODER_B_PINS {F4, F6}
#define ENCODER_BUTTON_A B2
#define ENCODER_BUTTON_B F5
#define DEBOUNCE 10
#define MAGIC_KEY_BOOTLOADER BRIGHTNESS_UP
#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 1

// // Top Encoder
// #define ENCODER_A_PINS { B3 }
// #define ENCODER_B_PINS { F4 }

// // Bottom Encoder
// #define ENCODER_A_PINS { F7 }
// #define ENCODER_B_PINS { F6 }

#define ENCODER_RESOLUTION 4

#define RGB_LED_COUNT 6
#define RGBLIGHT_LED_COUNT 6

#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 50
#define RGBLIGHT_VAL_STEP 50
#define RGBLIGHT_DEFAULT_VAL 20
#define RGBLIGHT_LIMIT_VAL 200
#define RGBLIGHT_DEFAULT_SPD 20

#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_STATIC_GRADIENT