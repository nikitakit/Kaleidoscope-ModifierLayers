# Kaleidoscope-ModifierLayers

`ModifierLayers` is a plugin that lets you specify non-standard effects of
modifier keys using a keymap.

For example in the Programmer Dvorak keyboard layout, the top row of the
keyboard contains the symbols `$&[{}(=*)+]!#`, while numbers can be typed by
pressing `Shift` and one of the symbols. The keys of the shifted and unshifted
layers are rearranged independently when compared to a QWERTY layout.

This plugin lets you specify overlay keymaps to use when modifier keys are held.
For any transparent entry in the overlay keymap, the appropriate key from the
layers underneath will be selected and combined with the modifier. For any
non-transparent entry, that entry will be used without applying the modifier
(this allows mapping `Shift + )` = `2`, where the `2` is unshifted).

The `ModifierLayers` plugin also takes care of rollover effects. If you define
`Shift + )` = `2` and press `Shift + ) + a`, your computer will receive either
`2` or `A`, depending on the order you pressed the keys, and not `@` (the result
of doing `Shift + 2` on a QWERTY keyboard.)

## Using the plugin

To use the plugin, you should define your keymap layers, create an array to
specify which overlay layers to use, and configure the provided `ModifierLayers`
object to use the array. Here is an example for setting up Programmer Dvorak:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-ModifierLayers.h>

enum { PDVORAK, PDVORAK_NUMBERS };

const Key keymaps[][ROWS][COLS] PROGMEM = {
    [PDVORAK] = // ... (omitted)
    [PDVORAK_NUMBERS] = // ... (omitted)
  };
}

static const kaleidoscope::ModifierLayers::overlay_t overlays[] = {
  {
      LAYER_MODIFIER_KEY(Key_LeftShift) | LAYER_MODIFIER_KEY(Key_RightShift),
      PDVORAK,
      PDVORAK_NUMBERS
  },
  {0, 0, 0}
};

void setup() {
  Kaleidoscope.setup();
  Kaleidoscope.use(&ModifierLayers);
  ModifierLayers.overlays = overlays;
}
```

Each overlay entry contains three pieces of information:
- A set of modifier keys, any of which will activate the overlay
- The base layer: the overlay will only activate when the base layer is active
- The overlay layer

The final entry in the list must be `{0, 0, 0}`.

## Example

A more complete Programmer Dvorak example is located in the `examples/` folder.
