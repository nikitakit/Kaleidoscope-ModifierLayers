#include "Kaleidoscope-ModifierLayers.h"

namespace kaleidoscope {
const ModifierLayers::overlay_t *ModifierLayers::overlays = NULL;

uint8_t ModifierLayers::mod_locked_held = 0;
uint8_t ModifierLayers::mod_locked_unheld = 0;
uint8_t ModifierLayers::mod_locked_held_next = 0;
uint8_t ModifierLayers::mod_locked_unheld_next = 0;

uint8_t ModifierLayers::mod_is_pressed_directly = 0;
uint8_t ModifierLayers::mod_was_pressed_directly = 0;

uint8_t ModifierLayers::live_unheld_required[ROWS][COLS];


ModifierLayers::ModifierLayers(void) {
}

void ModifierLayers::begin(void) {
    Kaleidoscope.useEventHandlerHook(eventHandlerHook);
    Kaleidoscope.useLoopHook(loopHook);
}

inline uint8_t flagsToModifierMask(uint8_t flags) {
    // TODO(nikita): it may be better to also incorporate
    // (Key_RightControl, Key_RightShift, Key_RightGui)
    uint8_t result = 0;
    result |= (flags & CTRL_HELD) ? LAYER_MODIFIER_KEY(Key_LeftControl) : 0;
    result |= (flags & LALT_HELD) ? LAYER_MODIFIER_KEY(Key_LeftAlt) : 0;
    result |= (flags & RALT_HELD) ? LAYER_MODIFIER_KEY(Key_RightAlt) : 0;
    result |= (flags & SHIFT_HELD)? LAYER_MODIFIER_KEY(Key_LeftShift) : 0;
    result |= (flags & GUI_HELD)  ? LAYER_MODIFIER_KEY(Key_LeftGui) : 0;
    return result;
}

Key ModifierLayers::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
    // If we are idle or have an injected key, fall through
    if ((!keyWasPressed(key_state) && !keyIsPressed(key_state)) || (key_state & INJECTED)) {
        return mapped_key;
    }

    if (keyIsPressed(key_state)
    && (!mapped_key.flags)
    && (mapped_key.keyCode >= HID_KEYBOARD_FIRST_MODIFIER)
    && (mapped_key.keyCode <= HID_KEYBOARD_LAST_MODIFIER)) {
        mod_is_pressed_directly |= LAYER_MODIFIER_KEY(mapped_key);
        return mapped_key;
    }

    // Don't handle synthetic keys
    if ((mapped_key.flags & SYNTHETIC)) {
        return mapped_key;
    }

    // We can't support rollover between keys that require a modifier to be held
    // and keys that require the same modifier to be unheld. Whichever one of
    // the requirements gets detected first is remembered, and all keypress
    // events that contradict it will get masked out.
    // When a key is first toggled on, the unheld requirements for that key are
    // computed and stored.
    if (keyToggledOn(key_state)) {
        uint8_t layer = Layer.lookupActiveLayer(row, col);
        uint8_t modifier_mask = 0;

        // This can be precomputed (which may be faster but will use more memory)
        for (byte index = 0; overlays[index].modifier_mask != 0; index++) {
            if (layer == overlays[index].overlay_layer && Layer.isOn(overlays[index].original_layer)) {
                modifier_mask = overlays[index].modifier_mask;
            }
        }
        live_unheld_required[row][col] = modifier_mask & ~flagsToModifierMask(mapped_key.flags);
    } else if (keyToggledOff(key_state)) {
        // In theory this should not be necessary, but do it just in case the
        // next keyToggledOn event for this key doesn't reach this handler
        live_unheld_required[row][col] = 0;
        return mapped_key;
    }

    uint8_t unheld_required = live_unheld_required[row][col];
    uint8_t held_required = ~unheld_required & mod_was_pressed_directly;

    if ((unheld_required & mod_locked_held) || (held_required & mod_locked_unheld)) {
        KeyboardHardware.maskKey(row, col);
        return Key_NoKey;
    }

    mod_locked_held |= held_required;
    mod_locked_held_next |= held_required;

    mod_locked_unheld |= unheld_required;
    mod_locked_unheld_next |= unheld_required;
    return mapped_key;
}


void ModifierLayers::loopHook(bool is_post_clear) {
    if (!is_post_clear) {
        // Release all modifier keys that are required to be not held
        for (byte index = 0; index < 8; index++) {
            if (mod_locked_unheld_next & ((uint8_t)1 << index)) {
                uint8_t key_code = index + HID_KEYBOARD_FIRST_MODIFIER;
                kaleidoscope::hid::releaseRawKey({ key_code, KEY_FLAGS });
            }
        }

        // Toggle any layers we need to
        uint32_t old_layer_state = Layer.getLayerState();
        for (byte index = 0; overlays[index].modifier_mask != 0; index++) {
            if (Layer.isOn(overlays[index].original_layer)) {
                if (mod_is_pressed_directly & overlays[index].modifier_mask) {
                    Layer.on(overlays[index].overlay_layer);
                } else {
                    Layer.off(overlays[index].overlay_layer);
                }
            } else {
                Layer.off(overlays[index].overlay_layer);
            }
        }
    } else {
        mod_locked_held = mod_locked_held_next;
        mod_locked_unheld = mod_locked_unheld_next;
        mod_was_pressed_directly = mod_is_pressed_directly;
        mod_locked_held_next = 0;
        mod_locked_unheld_next = 0;
        mod_is_pressed_directly = 0;
    }
}
}

kaleidoscope::ModifierLayers ModifierLayers;
