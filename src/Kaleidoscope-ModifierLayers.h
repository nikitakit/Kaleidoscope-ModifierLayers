#pragma once

#include <Kaleidoscope.h>

namespace kaleidoscope {
class ModifierLayers : public KaleidoscopePlugin {
public:
    typedef struct {
        uint8_t modifier_mask;
        uint8_t original_layer;
        uint8_t overlay_layer;
    } overlay_t;
    ModifierLayers(void);

    void begin(void) final;

    static Key eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state);
    static void loopHook(bool is_post_clear);

    static const overlay_t *overlays;
private:
    // There are exactly 8 standard modifier keys that fit in a uint8_t. See:
    // https://github.com/keyboardio/KeyboardioHID/blob/master/src/HIDTables.h
    static uint8_t mod_locked_held;
    static uint8_t mod_locked_unheld;
    static uint8_t mod_locked_held_next;
    static uint8_t mod_locked_unheld_next;

    // Layer toggles only respond to the modifier being pressed directly as its
    // own key, not via flags.
    static uint8_t mod_is_pressed_directly;
    static uint8_t mod_was_pressed_directly;

    static uint8_t live_unheld_required[ROWS][COLS];
};
}

extern kaleidoscope::ModifierLayers ModifierLayers;

#define LAYER_MODIFIER_KEY(key) ((uint8_t)1 << ((key).keyCode - HID_KEYBOARD_FIRST_MODIFIER))
