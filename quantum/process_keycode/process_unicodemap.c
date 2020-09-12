/* Copyright 2017 Jack Humbert
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

#include "process_unicodemap.h"

#include <print.h>

__attribute__((weak)) uint16_t unicodemap_index(uint16_t keycode) {
    if (keycode >= QK_UNICODEMAP_PAIR) {
        // Keycode is a pair: extract index based on Shift / Caps Lock state
        uint16_t index = keycode - QK_UNICODEMAP_PAIR;

        bool shift = (get_mods() | get_oneshot_mods() | get_weak_mods() ) & MOD_MASK_SHIFT;
        bool caps  = IS_HOST_LED_ON(USB_LED_CAPS_LOCK);

        // uprintf("shift %d caps %d\n", shift, caps);

        if (shift ^ caps) {
            index >>= 7;
        }

        return index & 0x7F;
    } else {
        print("no pair ");
        // Keycode is a regular index
        return keycode - QK_UNICODEMAP;
    }
}

bool process_unicodemap(uint16_t keycode, keyrecord_t *record) {
    // uprintf("process_unicodemap %d ", keycode);

    // bool shift1 = unicode_saved_mods & MOD_MASK_SHIFT;
    // uprintf("shift1 %d ", shift1);
    // uint8_t shift2 = get_mods() & MOD_MASK_SHIFT;
    // uprintf("shift2 %d\n", shift2);

    // if (keycode >= QK_UNICODEMAP) {
    //     print("less 1 ");
    // }
    //
    // if (keycode <= QK_UNICODEMAP_PAIR_MAX) {
    //     print("less 2 ");
    // }
    //
    // if (record->event.pressed) {
    //     print("cond 3 ");
    // }

    if (keycode >= QK_UNICODEMAP && keycode <= QK_UNICODEMAP_PAIR_MAX && record->event.pressed) {
        uint32_t code_point = pgm_read_dword(unicode_map + unicodemap_index(keycode));
        register_unicode(code_point);
    }
    return true;
}
