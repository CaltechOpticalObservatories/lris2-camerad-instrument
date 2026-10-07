# lris2-camerad-instrument

LRIS2 instrument module for
[camerad](https://github.com/CaltechOpticalObservatories/camera-interface).

This repository owns the instrument and pulls camerad in as a dependency, so
there is nothing to wire up by hand:

```bash
cmake -S . -B build && cmake --build build
```

The first configure fetches camerad over the network. Its own dependencies,
CCfits, cfitsio, Boost and zmqpp, have to be installed on the build machine.

## Python module

LRIS2 drives the Archon through the Python module rather than the daemon, so
this is the path that matters:

```bash
pip install .
```

That produces `camera_interface`, built for LRIS2. The module constructs the
camera in-process and needs no running camerad:

```python
import camera_interface as ci

camera = ci.Camera("config/lris2.cfg")
camera.open()
print(camera.bias("list 9"))
```

The module is named `camera_interface` for every instrument and is not
`module_local`, so give each instrument its own virtualenv.

To build it from CMake instead of pip, add `-DBUILD_PYTHON_MODULE=ON`.
`ctest` then runs an import check, which catches the module linking with its
interface factory missing.

## Contents

| File | Purpose |
|---|---|
| `lris2_instrument.{h,cpp}` | `Camera::LRIS2`, derived from `ArchonInterface` |
| `lris2_interface_factory.cpp` | `Camera::Interface::create()` returning an `LRIS2` |
| `CMakeLists.txt` | builds the daemon and the Python module |
| `pyproject.toml` | builds the Python module as a wheel |
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

The LRIS2 Archon reports backplane rev 7, firmware 1.0.1262. Its `SYSTEM` reply
gives the populated slots as:

| Slot | Type | Module |
|---|---|---|
| 1, 2, 3 | 16 | DriverX |
| 4 | 9 | LVXBias |
| 5 | 17 | ADM |
| 6 | 2 | AD |
| 9 | 8 | HVXBias |
| 10 | 12 | XVBias |
| 11, 12 | 11 | HeaterX |

Slots 7 and 8 are empty.

`RAWSEL` is a flat channel index over the whole system, not a slot and channel
pair: it runs 0 to 71 in the config, which is four module slots of eighteen ADM
channels. There is no arithmetic that recovers the slot from it, so read the
module types from `SYSTEM` to know which channel belongs to which board.
