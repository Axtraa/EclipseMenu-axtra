# AGENTS.md

Fork of **EclipseMenu** (Geometry Dash mod menu). C++23 + Geode SDK, targets GD `2.2081` on Windows/macOS/Android/iOS. This fork exists to port features from **Axtra's Utils** (`/home/lucas/Documents/Dev/axtra-utils`) into the Eclipse menu UI.

## Build & verify

- Local build is not configured in this checkout (`cmake` missing, `GEODE_SDK` unset). Reliable verification is CI.
- Every push to any branch triggers `.github/workflows/multi-platform.yml`: Windows, macOS, Android32, Android64, iOS.
- Local build (once prerequisites exist):
  ```bash
  cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
  cmake --build build --config RelWithDebInfo
  ```
- CI enables **unity build + LTO** (`GITHUB_ACTIONS` in `CMakeLists.txt`). Local builds may not, so duplicate non-static helper names across `.cpp` files can pass locally and fail in CI. Keep helpers `static` or in anonymous namespaces.
- Dependencies are pinned in `cmake/IncludeLibs.cmake` (CPM, git hashes). Don't bump casually.
- Translations are fetched from `EclipseMenu/translations` at configure time (`cmake/Translations.cmake`); `resources/translations/*` is gitignored. Set `ECLIPSE_TRANSLATIONS_REPO_PATH` to a local clone for offline/dev builds.
- `mod.json` requires Geode `5.3.0`; axtra-utils pins `5.7.1`. Port logic, not its build config or manifest.

## Adding a hack (primary workflow)

One file per hack: `src/hacks/<Category>/<Name>.cpp`. Directory maps directly to the menu tab:

| Directory | Tab |
|---|---|
| `Global` | `tab.global` |
| `Level` | `tab.level` |
| `Bypass` | `tab.bypass` |
| `Player` | `tab.player` |
| `Bot` | `tab.bot` |
| `Creator` | `tab.creator` |
| `Labels` | `tab.labels` |
| `Shortcuts` | `tab.shortcuts` |
| `Recorder` | `tab.recorder` |

Minimal shape (see `src/hacks/Level/SmartStartPos.cpp`, `src/hacks/Global/Speedhack.cpp`):

```cpp
#include <modules/config/config.hpp>
#include <modules/gui/gui.hpp>
#include <modules/gui/components/toggle.hpp>
#include <modules/hack/hack.hpp>

#include <Geode/modify/PlayLayer.hpp>

namespace eclipse::hacks::Level {
    class $hack(MyHack) {
        void init() override {
            auto tab = gui::MenuTab::find("tab.level");
            config::setIfEmpty("level.myhack", false);
            tab->addToggle("level.myhack")->handleKeybinds()->setDescription();
        }
        [[nodiscard]] const char* getId() const override { return "My Hack"; }
    };
    REGISTER_HACK(MyHack)

    class $modify(MyHackPLHook, PlayLayer) {
        ADD_HOOKS_DELEGATE("level.myhack")
        // hook overrides here
    };
}
```

Key mechanics:

- The component ID **is** the config key. `addToggle` stores its bool at `<id>`; `addFloatToggle`/`addIntToggle` store the value at `<id>` and the enabled state at `<id>.toggle` (which is what `ADD_HOOKS_DELEGATE` must receive for those).
- The component title is normally an **i18n key**; unknown keys render as the raw key. Fork-only features may use literal English titles/descriptions until translations land upstream.
- `ADD_HOOKS_DELEGATE(id)` makes hooks no-ops while the toggle is off (no manual `if` needed). For priority use `ENABLE_SAFE_HOOKS_ALL()` / `ENABLE_FIRST_HOOKS_ALL()`.
- Read config with `config::get<"key", T>()` (compile-time key, cached) or `config::get<T>("key", fallback)`.
- Options/sub-settings: `->addOptions([](auto o){ o->addInputInt("level.myhack.interval", 1, 1000); })`.
- Use `utils::get<T>()` to fetch game singletons (`PlayLayer`, `GameManager`, …).
- `MenuTab::find` creates unknown tabs automatically, but built-in tab order is hardcoded in `src/modules/gui/gui.cpp` (`builtInTabs`); a new custom tab sorts alphabetically after built-ins.

## Porting from axtra-utils

- axtra-utils is a single `src/main.cpp` driven by Geode `mod.json` settings. Do **not** copy that pattern: Eclipse uses `config` + `MenuTab` components, and keybinds via `handleKeybinds()` (not a custom `KeyRecordLayer`).
- OS-level key simulation (`SendInput`, `CGEventCreateKeyboardEvent`) is desktop-only. Wrap includes **and** calls in `#ifdef GEODE_IS_WINDOWS` / `#ifdef GEODE_IS_MACOS`; unguarded `<windows.h>` / CoreGraphics breaks Android/iOS CI.
- Check for existing equivalents before porting. Current overlaps: `hacks/Player/Autoclicker.cpp` (in-game `handleButton`, not OS input), `hacks/Level/HideLevelCompleteVFX.cpp` (`CCCircleWave`), `hacks/Level/LegacyPulse.cpp` (`FMODAudioEngine::m_pulse1`).
- Ported axtra features: `hacks/Level/NoOrbPulse.cpp`, `hacks/Level/NoCircles.cpp`, `hacks/Global/ForceAspectRatio.cpp`, `hacks/Player/AutoKeyPress.cpp` (desktop-only OS key simulation).
- Don't depend on the `axtra.axtra-utils` mod at runtime. `_spr` resolves to this mod's ID (`eclipse.eclipse-menu`).
- New sprites/files must be registered in `mod.json` under `resources` (glob patterns) or they won't ship.

## Conventions

- 4-space indent; `namespace eclipse::hacks::<Category>`; Eclipse module includes before Geode includes.
- Hook state goes in `struct Fields { ... }` accessed via `m_fields.self()`; members use `m_camelCase`.
- Config keys are namespaced by tab (`level.*`, `player.*`, `global.*`).
- `getId()` is a unique human-readable name used by other systems; don't change it once shipped.
- Release notes go in `changelog.md` using its BBcode-style tags (`<cg>`, `<co>`, `<cl>`, `<cy>`, `<cr>`); bump `mod.json` `version` at release.
- `LABELS.md` documents the RIFT label expression system; it is unrelated to i18n translation keys.
- Public API for other mods lives in `include/` (`eclipse.hpp` umbrella) — don't break its headers without reason.
