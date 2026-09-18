# lris2-camerad-instrument

LRIS2 instrument module for
[camerad](https://github.com/CaltechOpticalObservatories/camera-interface).

Intended to live at `camerad/Instruments/lris2` as a submodule, selected at
build time:

```bash
cmake -DCONTROLLER=archon -DINSTRUMENT=lris2 ..
make
```

## Contents

| File | Purpose |
|---|---|
| `lris2_instrument.{h,cpp}` | `Camera::LRIS2`, derived from `ArchonInterface` |
| `lris2_interface_factory.cpp` | `Camera::Interface::create()` returning an `LRIS2` |
| `lris2.cmake` | source list consumed by camerad's CMake |
| `config/lris2.cfg` | camerad server config |
| `config/LRIS2.acf` | Archon config |

## Status

Minimal on purpose. Exposures use the standard `ArchonInterface` modes, so
there are no LRIS2-specific exposure mode classes yet. `Camera::LRIS2` exists as
the hook point for detector-specific behavior as it is needed.

`config/LRIS2.acf` is a skeleton: no `TAPLINE` keys, a zeroed `[SYSTEM]` block,
and none of the six RAW keys (`RAWENABLE`, `RAWSEL`, `RAWSTARTLINE`,
`RAWENDLINE`, `RAWSTARTPIXEL`, `RAWSAMPLES`). It needs real content before the
detector arrives. Note that camerad can only write config keys that already
exist in the loaded ACF, so the RAW keys must be present there before `raw set`
can drive them.

## Controller

The LRIS2 Archon reports backplane rev 7, firmware 1.0.1262, with an AD module
(type 2) in slot 6 and an undocumented type 17 module in slot 5. Slots 7 and 8
are empty. `RAWSEL` maps four channels per slot across slots 5 to 8, so
`RAWSEL` 4 to 7 addresses the known AD module.
