// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _QWERTY,
    _COLEMAK,
    _LOWER,
    _RAISE,
    _ADJUST,
};

void via_init_kb(void) {
    if (via_eeprom_is_valid()) {
        sofle_load_via_mousekey_config();

        // If the stored firmware version doesn't match, reset the dynamic keymap
        // so the compiled default takes effect. Bump VIA_FIRMWARE_VERSION in
        // config.h whenever you want to push a new default keymap.
        uint32_t stored_version = 0;
        via_read_custom_config(&stored_version, sizeof(sofle_via_config_t), sizeof(uint32_t));
        if (stored_version != VIA_FIRMWARE_VERSION) {
            dynamic_keymap_reset();
            uint32_t new_version = VIA_FIRMWARE_VERSION;
            via_update_custom_config(&new_version, sizeof(sofle_via_config_t), sizeof(uint32_t));
        }
    } else {
        sofle_apply_mousekey_speed(MOUSEKEY_MAX_SPEED);
        sofle_save_via_mousekey_config();
        uint32_t new_version = VIA_FIRMWARE_VERSION;
        via_update_custom_config(&new_version, sizeof(sofle_via_config_t), sizeof(uint32_t));
    }
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &data[0];
    uint8_t *channel_id = &data[1];
    uint8_t *value_id   = &data[2];
    uint8_t *value_data = &data[3];

    (void)length;

    if (*channel_id != id_custom_channel) {
        *command_id = id_unhandled;
        return;
    }

    switch (*command_id) {
        case id_custom_set_value:
            switch (*value_id) {
                case SOFLE_VIA_MOUSEKEY_MAX_SPEED:
                    sofle_apply_mousekey_speed(value_data[0]);
                    break;
                default:
                    *command_id = id_unhandled;
                    break;
            }
            break;
        case id_custom_get_value:
            switch (*value_id) {
                case SOFLE_VIA_MOUSEKEY_MAX_SPEED:
                    value_data[0] = sofle_via_config.mousekey_max_speed;
                    break;
                default:
                    *command_id = id_unhandled;
                    break;
            }
            break;
        case id_custom_save:
            sofle_save_via_mousekey_config();
            break;
        default:
            *command_id = id_unhandled;
            break;
    }
}
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * BASE - QWERTY with home-row mods
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Ent  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  |   A  |LCTL/S|LGUI/D|LALT/F|   G  |-------.    ,-------|   H  |RALT/J|RGUI/K|RCTL/L|   ;  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTL | LGUI | LAlt | Bspc |/ SYM  /       \ NAV   \| Spc | MOU  | RAlt | RGUI |
 *            `----------------------------------'           '------''---------------------------'
 */
[_BASE] = LAYOUT(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_ENT,
  KC_ESC,  KC_A,    HRM_S,   HRM_D,   HRM_F,   KC_G,                      KC_H,    HRM_J,   HRM_K,   HRM_L,   KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE,  XXXXXXX,KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
                 KC_LCTL, KC_LGUI, KC_LALT, KC_BSPC, MO(_SYMBOLS),   MO(_NAV), KC_SPC, MO(_MOUSE), KC_RALT, KC_RGUI
),
/*
 * MOUSE - cursor, wheel, acceleration, media
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |WhlLft| MsUp |WhlRgt|WhlUp |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |MsLeft|MsDwn |MsRgt |WhlDwn|-------.    ,-------|      | Acl0 | Acl1 | Acl2 |      |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | Prev | Play | Next |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      | Btn1 |/ Btn2 /       \      \| Spc  |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_MOUSE] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, MS_WHLL, MS_UP,   MS_WHLR, MS_WHLU,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLD,                   XXXXXXX, MS_ACL0, MS_ACL1, MS_ACL2, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MUTE,  XXXXXXX,XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX,
                 XXXXXXX, XXXXXXX, XXXXXXX, MS_BTN1, MS_BTN2,    XXXXXXX, KC_SPC,  _______, _______, XXXXXXX
),
/*
 * SYMBOLS - numbers, function keys, brackets, operators
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   <  |   {  |   [  |   (  |   -  |-------.    ,-------|   =  |   )  |   ]  |   }  |   >  |   |  |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |   =  |   -  |   +  |   _  |   }  |-------|    |-------|   [  |   +  |   ;  |   :  |   \  |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      |/       /       \      \|      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_SYMBOLS] = LAYOUT(
  _______, KC_F1,      KC_F2,      KC_F3,   KC_F4,      KC_F5,                     KC_F6,   KC_F7,      KC_F8,   KC_F9,      KC_F10,     KC_F11,
  KC_GRV,  KC_1,       KC_2,       KC_3,    KC_4,       KC_5,                      KC_6,    KC_7,       KC_8,    KC_9,       KC_0,       KC_F12,
  _______, S(KC_COMM), S(KC_LBRC), KC_LBRC, S(KC_9),    KC_MINS,                   KC_EQL,  S(KC_0),    KC_RBRC, S(KC_RBRC), S(KC_DOT),  S(KC_BSLS),
  _______, KC_EQL,     KC_MINS,    S(KC_EQL), S(KC_MINS), S(KC_RBRC), _______,  _______,KC_LBRC, S(KC_EQL), KC_SCLN, S(KC_SCLN), KC_BSLS,    _______,
                 _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______
),
/*
 * NAV - navigation, editing, clipboard
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | Ins  | PScr | Menu |      |      |                    | PgUp |      |  Up  |      |      | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | LAlt | LCtl |LShift|      | Caps |-------.    ,-------| PgDn | Left | Down | Rght | Del  | Bspc |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      | Undo | Cut  | Copy | Paste|      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      |/       /       \      \|      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_NAV] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  _______, KC_INS,  KC_PSCR, KC_APP,  XXXXXXX, XXXXXXX,                   KC_PGUP, _______, KC_UP,   _______, _______, KC_BSPC,
  _______, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS,                   KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  KC_BSPC,
  _______, KC_UNDO, KC_CUT,  KC_COPY, KC_PSTE, XXXXXXX, _______,  _______,XXXXXXX, _______, XXXXXXX, _______, XXXXXXX, _______,
                 _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______
),
/*
 * ADJUST - boot, OS toggle, media (hold SYMBOLS + NAV simultaneously)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Boot |      |      |      |CG_TG |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |CG_TG |      |      |      |-------.    ,-------|      | VolDn| Mute | VolUp|      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | Prev | Play | Next |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      |/       /       \      \|      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, CG_TOGG, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, CG_TOGG, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                 _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______
)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[DYNAMIC_KEYMAP_LAYER_COUNT][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE]    = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [_MOUSE]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(RM_HUED, RM_HUEU) },
    [_SYMBOLS] = { ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(RM_HUED, RM_HUEU) },
    [_NAV]     = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT),  ENCODER_CCW_CW(KC_DOWN, KC_UP)  },
    [_ADJUST]  = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _SYMBOLS, _NAV, _ADJUST);
}

// Custom heatmap color formula - overrides the built-in render output.
// Runs independently on each half, so both sides display correctly.
// To change the color mapping, edit this function and typing_heatmap_anim.h.
#if defined(RGB_MATRIX_ENABLE) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_config.mode == RGB_MATRIX_TYPING_HEATMAP) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uint8_t idx = g_led_config.matrix_co[row][col];
                if (idx != NO_LED && idx >= led_min && idx < led_max) {
                    uint8_t val = g_rgb_frame_buffer[row][col];
                    uint8_t hue = 1 + (val > 20 ? val - 20 : 0);
                    hsv_t hsv = {hue, rgb_matrix_config.hsv.s, 155};
                    rgb_t rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(idx, rgb.r, rgb.g, rgb.b);
                }
            }
        }
    }
    return false;
}
#endif

/* Copyright 2021 Carlos Martins
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"


  // Physical Layout
  // Columns
  // 0  1  2  3  4  5  6  7  8  9  10 11 12 13
  //                                           ROWS
  // 12 13 22 23 32 33       33 32 23 22 13 12  0
  //    02    03    04       04    03    02
  // 11 14 21 24 31 34       34 31 24 21 14 11  1
  //                01       01
  // 10 15 20 25 30 35       35 30 25 20 15 10  2
  //
  // 09 16 19 26 29 36       36 29 26 19 16 09  3
  //
  //     08 17 18 27 28     28 27 18 17 08      4
  //    07    06    05       05    06    07

led_config_t g_led_config = {
    {
        { 10, 11, 20, 21, 30, 31 },
        {  9, 12, 19, 22, 29, 32 },
        {  8, 13, 18, 23, 28, 33 },
        {  7, 14, 17, 24, 27, 34 },
        {  6, 15, 16, 25, 26, NO_LED },
        {  46,  47,  56,  57,  66,  67 },
        {  45,  48,  55,  58,  65,  68 },
        {  44,  49,  54,  59,  64,  69 },
        {  43,  50,  53,  60,  63,  70 },
        {  42,  51,  52,  61,  62,  NO_LED }
    },
    {
       // Left side underglow
        {96, 40}, {16, 20}, {48, 10}, {80, 18}, {88, 60}, {56, 57}, {24,60},
        // Left side Matrix
        {32, 57}, { 0, 48}, { 0, 36}, { 0, 24}, { 0, 12},
        {16, 12}, {16, 24}, {16, 36}, {16, 48}, {48, 55},
        {64, 57}, {32, 45}, {32, 33}, {32, 21}, {32,  9},
        {48,  7}, {48, 19}, {48, 31}, {48, 43}, {80, 59},
        {96, 64}, {64, 45}, {64, 33}, {64, 21}, {64,  9},
        {80, 10}, {80, 22}, {80, 34}, {80, 47},


        // Right side underglow
        {128, 40}, {208, 20}, {176, 10}, {144, 18}, {136, 60}, {168, 57}, {200,60},
        // Right side Matrix
        {192, 57}, {224, 48}, {224, 36}, {224, 24}, {224, 12},
        {208, 12}, {208, 24}, {208, 36}, {208, 48}, {176, 55},
        {160, 57}, {192, 45}, {192, 33}, {192, 21}, {192,  9},
        {176,  7}, {176, 19}, {176, 31}, {176, 43}, {144, 59},
        {128, 64}, {160, 45}, {160, 33}, {160, 21}, {160,  9},
        {144, 10}, {144, 22}, {144, 34}, {144, 47},
    },
    {
        LED_FLAG_INDICATOR, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_INDICATOR, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT
    }
};
