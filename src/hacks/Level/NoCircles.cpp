#include <modules/config/config.hpp>
#include <modules/gui/gui.hpp>
#include <modules/gui/components/toggle.hpp>
#include <modules/hack/hack.hpp>

#include <Geode/modify/CCCircleWave.hpp>

namespace eclipse::hacks::Level {
    class $hack(NoCircles) {
        void init() override {
            auto tab = gui::MenuTab::find("tab.level");

            config::setIfEmpty("level.nocircles", false);

            tab->addToggle("No Circles", "level.nocircles")
               ->handleKeybinds()
               ->setDescription("Hides circle wave effects.");
        }

        [[nodiscard]] const char* getId() const override { return "No Circles"; }
    };

    REGISTER_HACK(NoCircles)

    class $modify(NoCirclesCCCWHook, CCCircleWave) {
        ADD_HOOKS_DELEGATE("level.nocircles")

        void draw() override {}
    };
}
