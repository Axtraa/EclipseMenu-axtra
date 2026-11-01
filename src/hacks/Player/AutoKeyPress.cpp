#include <modules/config/config.hpp>
#include <modules/gui/gui.hpp>
#include <modules/gui/components/keybind.hpp>
#include <modules/gui/components/toggle.hpp>
#include <modules/hack/hack.hpp>
#include <modules/keybinds/manager.hpp>

#include <Geode/modify/PlayLayer.hpp>

#ifdef GEODE_IS_DESKTOP

#ifdef GEODE_IS_WINDOWS
#include <Windows.h>
#elif defined(GEODE_IS_MACOS)
// Avoid CommentType conflict with Geode bindings.
#define CommentType CommentTypeDummy
#include <CoreGraphics/CoreGraphics.h>
#undef CommentType
#endif

namespace eclipse::hacks::Player {
    static int toNativeKey(keybinds::Keys key) {
        using keybinds::Keys;

        #ifdef GEODE_IS_WINDOWS
        switch (key) {
            case Keys::A: case Keys::B: case Keys::C: case Keys::D: case Keys::E:
            case Keys::F: case Keys::G: case Keys::H: case Keys::I: case Keys::J:
            case Keys::K: case Keys::L: case Keys::M: case Keys::N: case Keys::O:
            case Keys::P: case Keys::Q: case Keys::R: case Keys::S: case Keys::T:
            case Keys::U: case Keys::V: case Keys::W: case Keys::X: case Keys::Y:
            case Keys::Z:
                return 0x41 + (static_cast<int>(key) - static_cast<int>(Keys::A));

            case Keys::Num0: case Keys::Num1: case Keys::Num2: case Keys::Num3: case Keys::Num4:
            case Keys::Num5: case Keys::Num6: case Keys::Num7: case Keys::Num8: case Keys::Num9:
                return 0x30 + (static_cast<int>(key) - static_cast<int>(Keys::Num0));

            case Keys::F1: case Keys::F2: case Keys::F3: case Keys::F4: case Keys::F5:
            case Keys::F6: case Keys::F7: case Keys::F8: case Keys::F9: case Keys::F10:
            case Keys::F11: case Keys::F12: case Keys::F13: case Keys::F14: case Keys::F15:
            case Keys::F16: case Keys::F17: case Keys::F18: case Keys::F19: case Keys::F20:
            case Keys::F21: case Keys::F22: case Keys::F23: case Keys::F24:
                return 0x70 + (static_cast<int>(key) - static_cast<int>(Keys::F1));

            case Keys::Space: return 0x20;
            case Keys::Apostrophe: return 0xDE;
            case Keys::Comma: return 0xBC;
            case Keys::Minus: return 0xBD;
            case Keys::Period: return 0xBE;
            case Keys::Slash: return 0xBF;
            case Keys::Semicolon: return 0xBA;
            case Keys::Equal: return 0xBB;
            case Keys::LeftBracket: return 0xDB;
            case Keys::Backslash: return 0xDC;
            case Keys::RightBracket: return 0xDD;
            case Keys::GraveAccent: return 0xC0;

            case Keys::NumPad0: case Keys::NumPad1: case Keys::NumPad2: case Keys::NumPad3: case Keys::NumPad4:
            case Keys::NumPad5: case Keys::NumPad6: case Keys::NumPad7: case Keys::NumPad8: case Keys::NumPad9:
                return 0x60 + (static_cast<int>(key) - static_cast<int>(Keys::NumPad0));
            case Keys::NumPadDecimal: return 0x6E;
            case Keys::NumPadDivide: return 0x6F;
            case Keys::NumPadMultiply: return 0x6A;
            case Keys::NumPadSubtract: return 0x6D;
            case Keys::NumPadAdd: return 0x6B;
            case Keys::NumPadEnter: return 0x0D;

            case Keys::Menu: return 0x5D;
            case Keys::Escape: return 0x1B;
            case Keys::Enter: return 0x0D;
            case Keys::Tab: return 0x09;
            case Keys::Backspace: return 0x08;
            case Keys::Insert: return 0x2D;
            case Keys::Delete: return 0x2E;
            case Keys::Home: return 0x24;
            case Keys::End: return 0x23;
            case Keys::PageUp: return 0x21;
            case Keys::PageDown: return 0x22;
            case Keys::CapsLock: return 0x14;
            case Keys::ScrollLock: return 0x91;
            case Keys::NumLock: return 0x90;
            case Keys::PrintScreen: return 0x2C;
            case Keys::Pause: return 0x13;

            case Keys::Up: return 0x26;
            case Keys::Down: return 0x28;
            case Keys::Left: return 0x25;
            case Keys::Right: return 0x27;

            case Keys::LeftShift: return 0xA0;
            case Keys::LeftControl: return 0xA2;
            case Keys::LeftAlt: return 0xA4;
            case Keys::LeftSuper: return 0x5B;
            case Keys::RightShift: return 0xA1;
            case Keys::RightControl: return 0xA3;
            case Keys::RightAlt: return 0xA5;
            case Keys::RightSuper: return 0x5C;

            default: return -1;
        }
        #else
        switch (key) {
            case Keys::A: return 0x00;
            case Keys::S: return 0x01;
            case Keys::D: return 0x02;
            case Keys::F: return 0x03;
            case Keys::H: return 0x04;
            case Keys::G: return 0x05;
            case Keys::Z: return 0x06;
            case Keys::X: return 0x07;
            case Keys::C: return 0x08;
            case Keys::V: return 0x09;
            case Keys::B: return 0x0B;
            case Keys::Q: return 0x0C;
            case Keys::W: return 0x0D;
            case Keys::E: return 0x0E;
            case Keys::R: return 0x0F;
            case Keys::Y: return 0x10;
            case Keys::T: return 0x11;
            case Keys::Num1: return 0x12;
            case Keys::Num2: return 0x13;
            case Keys::Num3: return 0x14;
            case Keys::Num4: return 0x15;
            case Keys::Num6: return 0x16;
            case Keys::Num5: return 0x17;
            case Keys::Equal: return 0x18;
            case Keys::Num9: return 0x19;
            case Keys::Num7: return 0x1A;
            case Keys::Minus: return 0x1B;
            case Keys::Num8: return 0x1C;
            case Keys::Num0: return 0x1D;
            case Keys::RightBracket: return 0x1E;
            case Keys::O: return 0x1F;
            case Keys::U: return 0x20;
            case Keys::LeftBracket: return 0x21;
            case Keys::I: return 0x22;
            case Keys::P: return 0x23;
            case Keys::Enter: return 0x24;
            case Keys::L: return 0x25;
            case Keys::J: return 0x26;
            case Keys::Apostrophe: return 0x27;
            case Keys::K: return 0x28;
            case Keys::Semicolon: return 0x29;
            case Keys::Backslash: return 0x2A;
            case Keys::Comma: return 0x2B;
            case Keys::Slash: return 0x2C;
            case Keys::N: return 0x2D;
            case Keys::M: return 0x2E;
            case Keys::Period: return 0x2F;
            case Keys::Tab: return 0x30;
            case Keys::Space: return 0x31;
            case Keys::GraveAccent: return 0x32;
            case Keys::Backspace: return 0x33;
            case Keys::Escape: return 0x35;
            case Keys::RightSuper: return 0x36;
            case Keys::LeftSuper: return 0x37;
            case Keys::LeftShift: return 0x38;
            case Keys::CapsLock: return 0x39;
            case Keys::LeftAlt: return 0x3A;
            case Keys::LeftControl: return 0x3B;
            case Keys::RightShift: return 0x3C;
            case Keys::RightAlt: return 0x3D;
            case Keys::RightControl: return 0x3E;
            case Keys::F17: return 0x40;
            case Keys::NumPadDecimal: return 0x41;
            case Keys::NumPadMultiply: return 0x43;
            case Keys::NumPadAdd: return 0x45;
            case Keys::NumLock: return 0x47;
            case Keys::NumPadDivide: return 0x4B;
            case Keys::NumPadEnter: return 0x4C;
            case Keys::NumPadSubtract: return 0x4E;
            case Keys::F18: return 0x4F;
            case Keys::F19: return 0x50;
            case Keys::NumPad0: return 0x52;
            case Keys::NumPad1: return 0x53;
            case Keys::NumPad2: return 0x54;
            case Keys::NumPad3: return 0x55;
            case Keys::NumPad4: return 0x56;
            case Keys::NumPad5: return 0x57;
            case Keys::NumPad6: return 0x58;
            case Keys::NumPad7: return 0x59;
            case Keys::F20: return 0x5A;
            case Keys::NumPad8: return 0x5B;
            case Keys::NumPad9: return 0x5C;
            case Keys::F5: return 0x60;
            case Keys::F6: return 0x61;
            case Keys::F7: return 0x62;
            case Keys::F3: return 0x63;
            case Keys::F8: return 0x64;
            case Keys::F9: return 0x65;
            case Keys::F11: return 0x67;
            case Keys::F13: return 0x69;
            case Keys::F16: return 0x6A;
            case Keys::F14: return 0x6B;
            case Keys::F10: return 0x6D;
            case Keys::F12: return 0x6F;
            case Keys::F15: return 0x71;
            case Keys::Insert: return 0x72;
            case Keys::Home: return 0x73;
            case Keys::PageUp: return 0x74;
            case Keys::Delete: return 0x75;
            case Keys::F4: return 0x76;
            case Keys::End: return 0x77;
            case Keys::F2: return 0x78;
            case Keys::PageDown: return 0x79;
            case Keys::F1: return 0x7A;
            case Keys::Left: return 0x7B;
            case Keys::Right: return 0x7C;
            case Keys::Down: return 0x7D;
            case Keys::Up: return 0x7E;

            default: return -1;
        }
        #endif
    }

    #ifdef GEODE_IS_WINDOWS
    static bool isExtendedKey(int vk) {
        switch (vk) {
            case 0x2D: case 0x2E: // Insert, Delete
            case 0x24: case 0x23: // Home, End
            case 0x21: case 0x22: // PageUp, PageDown
            case 0x25: case 0x26: case 0x27: case 0x28: // Arrows
            case 0xA1: case 0xA3: case 0xA5: // Right Shift, Control, Alt
            case 0x5B: case 0x5C: // Left/Right Super
            case 0x5D: // Menu
            case 0x90: // NumLock
            case 0x2C: // PrintScreen
            case 0x6F: // NumPadDivide
                return true;
            default:
                return vk >= 0x60 && vk <= 0x6F; // Numpad
        }
    }
    #endif

    static void pressKey(int native) {
        #ifdef GEODE_IS_WINDOWS
        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = static_cast<WORD>(native);
        if (isExtendedKey(native)) input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
        SendInput(1, &input, sizeof(INPUT));
        #elif defined(GEODE_IS_MACOS)
        if (auto event = CGEventCreateKeyboardEvent(nullptr, static_cast<CGKeyCode>(native), true)) {
            CGEventPost(kCGHIDEventTap, event);
            CFRelease(event);
        }
        #endif
    }

    static void releaseKey(int native) {
        #ifdef GEODE_IS_WINDOWS
        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = static_cast<WORD>(native);
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        if (isExtendedKey(native)) input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
        SendInput(1, &input, sizeof(INPUT));
        #elif defined(GEODE_IS_MACOS)
        if (auto event = CGEventCreateKeyboardEvent(nullptr, static_cast<CGKeyCode>(native), false)) {
            CGEventPost(kCGHIDEventTap, event);
            CFRelease(event);
        }
        #endif
    }

    static void tapKey(int native) {
        pressKey(native);
        releaseKey(native);
    }

    class $hack(AutoKeyPress) {
        void init() override {
            auto tab = gui::MenuTab::find("tab.player");

            config::setIfEmpty("player.autokeypress", false);
            config::setIfEmpty("player.autokeypress.percent", 50.f);
            config::setIfEmpty<keybinds::KeybindProps>("player.autokeypress.key", keybinds::Keys::F2);

            tab->addToggle("Auto Key Press", "player.autokeypress")
               ->handleKeybinds()
               ->setDescription("Presses a keyboard key at a set level percentage.")
               ->addOptions([](auto options) {
                   options->addInputFloat("Trigger Percent", "player.autokeypress.percent", 0.f, 100.f, "%.1f%%");
                   options->addKeybind("Key", "player.autokeypress.key")
                          ->setDefaultKey(keybinds::Keys::F2);
               });
        }

        [[nodiscard]] const char* getId() const override { return "Auto Key Press"; }
    };

    REGISTER_HACK(AutoKeyPress)

    static int currentNativeKey() {
        auto key = config::get<"player.autokeypress.key", keybinds::KeybindProps>(keybinds::Keys::F2);
        return toNativeKey(key.key);
    }

    class $modify(AutoKeyPressPLHook, PlayLayer) {
        struct Fields {
            bool m_fired = false;
            bool m_held = false;
            float m_releaseTimer = 0.f;
            float m_startPosX = 0.f;
            int m_nativeKey = -1;
        };

        void resetState() {
            auto fields = m_fields.self();

            if (fields->m_held && fields->m_nativeKey >= 0)
                releaseKey(fields->m_nativeKey);

            fields->m_fired = false;
            fields->m_held = false;
            fields->m_releaseTimer = 0.f;
            fields->m_nativeKey = -1;
            fields->m_startPosX = m_player1 ? m_player1->getPositionX() : 0.f;
        }

        void setupHasCompleted() {
            PlayLayer::setupHasCompleted();
            this->resetState();
        }

        void resetLevel() override {
            PlayLayer::resetLevel();
            this->resetState();
        }

        void destroyPlayer(PlayerObject* player, GameObject* object) override {
            PlayLayer::destroyPlayer(player, object);

            auto fields = m_fields.self();
            if (!fields->m_fired || !config::get<"player.autokeypress", bool>()) return;

            int native = currentNativeKey();
            if (native >= 0) tapKey(native);
        }

        void levelComplete() {
            PlayLayer::levelComplete();

            auto fields = m_fields.self();
            if (!fields->m_fired || !config::get<"player.autokeypress", bool>()) return;

            int native = currentNativeKey();
            if (native >= 0) tapKey(native);
        }

        void updateVisibility(float dt) override {
            auto fields = m_fields.self();

            if (fields->m_held) {
                fields->m_releaseTimer -= dt;
                if (fields->m_releaseTimer <= 0.f) {
                    releaseKey(fields->m_nativeKey);
                    fields->m_held = false;
                    fields->m_nativeKey = -1;
                }
            }

            if (!fields->m_fired && fields->m_startPosX <= 0.f
                && config::get<"player.autokeypress", bool>() && m_level && m_player1) {
                float playerX = m_player1->getPositionX();
                float levelLength = m_levelLength;

                if (playerX > 0.f && levelLength > 1000.f) {
                    float trigger = config::get<"player.autokeypress.percent", float>(50.f);
                    float current = (playerX / levelLength) * 100.f;

                    if (current >= trigger) {
                        int native = currentNativeKey();
                        if (native >= 0) {
                            pressKey(native);
                            fields->m_nativeKey = native;
                            fields->m_held = true;
                            fields->m_releaseTimer = 0.1f;
                        }
                        fields->m_fired = true;
                    }
                }
            }

            PlayLayer::updateVisibility(dt);
        }

        void onExit() override {
            auto fields = m_fields.self();
            if (fields->m_held && fields->m_nativeKey >= 0)
                releaseKey(fields->m_nativeKey);

            PlayLayer::onExit();
        }
    };
}

#endif
