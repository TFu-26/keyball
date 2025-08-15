/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// enum my_keyball_keycodes {
//     PRC_TOG,                      // Precision モードトグル
//     PRC_SW,                       // Precision モードスイッチ 
// };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LCTL  , KC_LGUI  , KC_LALT  ,LSFT_T(KC_LNG2),LT(1,KC_SPC),LT(3,KC_LNG1),KC_BSPC,LT(2,KC_ENT),LSFT_T(KC_LNG2),KC_RALT,KC_RGUI, KC_RSFT
  ),

  [1] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_RBRC  ,                            KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    KC_F5    , KC_EXLM  , S(KC_6)  ,S(KC_INT3), S(KC_8)  ,                           S(KC_INT1), KC_BTN1  , KC_PGUP  , KC_BTN2  , KC_SCLN  ,
    S(KC_EQL),S(KC_LBRC),S(KC_7)   , S(KC_2)  ,S(KC_RBRC),                            KC_LBRC  , KC_DLR   , KC_PGDN  , KC_BTN3  , KC_F11   ,
    KC_INT1  , KC_EQL   , S(KC_3)  , _______  , _______  , _______  ,      TO(2)    , TO(0)    , _______  , KC_RALT  , KC_RGUI  , KC_F12
  ),

  [2] = LAYOUT_universal(
    KC_TAB   , KC_7     , KC_8     , KC_9     , KC_MINS  ,                            KC_NUHS  , _______  , KC_BTN3  , _______  , KC_BSPC  ,
   S(KC_QUOT), KC_4     , KC_5     , KC_6     ,S(KC_SCLN),                            S(KC_9)  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_QUOT  ,
    KC_SLSH  , KC_1     , KC_2     , KC_3     ,S(KC_MINS),                           S(KC_NUHS), KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  ,
    KC_ESC   , KC_0     , KC_DOT   , KC_DEL   , KC_ENT   , KC_BSPC  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , SCRL_DVI ,                            _______  , _______  , _______  , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),

};
// clang-format on

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"
#include "custom_oled.c"

// 画面はマスター側のみ回転させるように修正
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    //return OLED_ROTATION_270;
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    } else {
        return OLED_ROTATION_180;
    }
}   

// メイン側OLEDの表示処理
void oledkit_render_info_user(void) {
    //keyball_oled_render_keyinfo();
    //keyball_oled_render_ballinfo();
    //keyball_oled_render_layerinfo();
    keyball_oled_render_mymain();
}

// // サブ側OLEDの表示処理
// void oledkit_render_logo_user(void) {
//     keyball_oled_render_mysub();
// }

// レイヤーに合わせたLED制御 ※未完成
//  14,  10,  6,  3,   0,        43,  40,  37,  34,  30
//  15,  11,  7,  4,   1,        44,  41,  38,  35,  31 
//  16,  12,  8,  5,   2,        45,  42,  39,  36,  32
//  17,  13,  9,                                     33
// U19          U18,                  U29,          U28
// U20,     U21,                         U26         U27
//                   U22,  U23, U24,  U25,
layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);

    // UnderGrowはすべてOFF（後続で上書きする前提で全OFF）
    //rgblight_sethsv(HSV_OFF);

    // レイヤーごとにLEDの色を変更
    uint8_t layer = biton32(state);
    switch (layer) {
        case 0:
            rgblight_sethsv(HSV_WHITE);
            //rgblight_sethsv_range(HSV_WHITE, 30, 46);
            //rgblight_sethsv_range(HSV_WHITE,  0, 18);
            break;
        case 1:
            rgblight_sethsv(HSV_CYAN);
            //rgblight_sethsv_range(HSV_GREEN,  0, 18);
            //rgblight_sethsv_range(HSV_BLUE, 30, 46);
            break;
        case 2:
            rgblight_sethsv(HSV_PURPLE);
            //rgblight_sethsv_range(HSV_AZURE,  0, 18);
            //rgblight_sethsv_range(HSV_CORAL, 30, 46);
            break;
        case 3:
            rgblight_sethsv(HSV_AZURE);
            //rgblight_sethsv_range(HSV_PURPLE,  0, 18);
            //rgblight_sethsv_range(HSV_RED, 30, 46);
            break;
    }

    return state;
}

#endif