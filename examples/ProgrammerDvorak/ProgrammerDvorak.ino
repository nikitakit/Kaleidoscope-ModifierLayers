#include <Kaleidoscope.h>
#include <Kaleidoscope-ModifierLayers.h>

enum { PDVORAK, PDVORAK_NUMBERS };

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [PDVORAK] = KEYMAP_STACKED
 (___,             Key_LeftBracket, LSHIFT(Key_LeftBracket), LSHIFT(Key_RightBracket), LSHIFT(Key_9), Key_Equals, XXX,
  LSHIFT(Key_7),   Key_Semicolon,   Key_Comma,               Key_Period,               Key_P,         Key_Y,      Key_Tab,
  LSHIFT(Key_4),   Key_A,           Key_O,                   Key_E,                    Key_U,         Key_I,
  Key_Backslash,   Key_Quote,       Key_Q,                   Key_J,                    Key_K,         Key_X,      Key_Escape,
  Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
  XXX,

  XXX,            LSHIFT(Key_8), LSHIFT(Key_0), LSHIFT(Key_Equals), Key_RightBracket, LSHIFT(Key_1), LSHIFT(Key_3),
  Key_Enter,      Key_F, Key_G, Key_C, Key_R, Key_L, Key_Slash,
                  Key_D, Key_H, Key_T, Key_N, Key_S, Key_Minus,
  Key_RightAlt,   Key_B, Key_M, Key_W, Key_V, Key_Z, LSHIFT(Key_2),
  Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
  XXX),

  // Programmer Dvorak overrides the behavior of a number of keys when shift is
  // held. The ModifierLayers plugin allows us to specify this as a keymap.
  [PDVORAK_NUMBERS] = KEYMAP_STACKED
  (___, Key_7, Key_5, Key_3, Key_1, Key_9, ___,
   LSHIFT(Key_5), ___, ___, ___, ___, ___, ___,
   LSHIFT(Key_Backtick), ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   ___, Key_0, Key_2, Key_4, Key_6, Key_8, Key_Backtick,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, LSHIFT(Key_6),
   ___, ___, ___, ___,
   ___)
};

static const kaleidoscope::ModifierLayers::overlay_t overlays[] = {
  // When either shift key is held, overlay the PDVORAK_NUMBERS layer if the
  // PDVORAK layer is active. Note that keys defined directly in the
  // PDVORAK_NUMBERS will not have the shift modifier applied to them.
  {LAYER_MODIFIER_KEY(Key_LeftShift) | LAYER_MODIFIER_KEY(Key_RightShift), PDVORAK, PDVORAK_NUMBERS},
  // The last element must be an all-zero terminator
  {0, 0, 0}
};

void setup() {
  Kaleidoscope.setup();
  Kaleidoscope.use(&ModifierLayers);
  ModifierLayers.overlays = overlays;
}

void loop() {
  Kaleidoscope.loop();
}
