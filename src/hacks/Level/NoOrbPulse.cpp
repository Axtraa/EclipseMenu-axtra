#include <modules/config/config.hpp>
#include <modules/gui/gui.hpp>
#include <modules/gui/components/toggle.hpp>
#include <modules/hack/hack.hpp>

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

namespace eclipse::hacks::Level {
    class $hack(NoOrbPulse) {
        void init() override {
            auto tab = gui::MenuTab::find("tab.level");

            config::setIfEmpty("level.noorbpulse", false);
            config::setIfEmpty("level.noorbpulse.size", 0.6f);

            tab->addToggle("No Orb Pulse", "level.noorbpulse")
               ->handleKeybinds()
               ->setDescription("Freezes the orb pulse effect at a fixed size.")
               ->addOptions([](auto options) {
                   options->addInputFloat("Pulse Size", "level.noorbpulse.size", 0.f, 1.f, "%.2f");
               });
        }

        [[nodiscard]] const char* getId() const override { return "No Orb Pulse"; }
    };

    REGISTER_HACK(NoOrbPulse)

    class $modify(NoOrbPulsePLHook, PlayLayer) {
        ADD_HOOKS_DELEGATE("level.noorbpulse")

        void updateVisibility(float dt) override {
            auto* fmod = FMODAudioEngine::get();
            if (!fmod) {
                PlayLayer::updateVisibility(dt);
                return;
            }

            auto* audio = m_audioEffectsLayer;
            float pulse = fmod->m_pulse1;
            float audioScale = audio ? audio->m_audioScale : 0.f;

            float size = config::get<"level.noorbpulse.size", float>(0.6f);
            fmod->m_pulse1 = size;
            if (audio) audio->m_audioScale = size;

            PlayLayer::updateVisibility(dt);

            fmod->m_pulse1 = pulse;
            if (audio) audio->m_audioScale = audioScale;
        }
    };
}
