#include <cmath>

#include <modules/config/config.hpp>
#include <modules/gui/gui.hpp>
#include <modules/gui/components/toggle.hpp>
#include <modules/hack/hack.hpp>

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

namespace eclipse::hacks::Global {
    class $hack(ForceAspectRatio) {
        void init() override {
            auto tab = gui::MenuTab::find("tab.global");

            config::setIfEmpty("global.forceaspectratio", false);

            tab->addToggle("Force 16:9", "global.forceaspectratio")
               ->handleKeybinds()
               ->setDescription("Adds black letterbox bars to display the game in 16:9.");
        }

        [[nodiscard]] const char* getId() const override { return "Force 16:9"; }
    };

    REGISTER_HACK(ForceAspectRatio)

    class $modify(ForceAspectRatioPLHook, PlayLayer) {
        struct Fields {
            cocos2d::CCNode* m_overlay = nullptr;
            cocos2d::CCSize m_size = {0.f, 0.f};
        };

        void removeOverlay() {
            if (auto overlay = m_fields->m_overlay) {
                overlay->removeFromParentAndCleanup(true);
                m_fields->m_overlay = nullptr;
            }
        }

        void updateOverlay() {
            this->removeOverlay();
            if (!config::get<"global.forceaspectratio", bool>()) return;

            auto ui = m_uiLayer;
            if (!ui) return;

            auto director = utils::get<cocos2d::CCDirector>();
            if (!director) return;

            auto winSize = director->getWinSize();
            constexpr float target = 16.f / 9.f;
            float current = winSize.width / winSize.height;
            if (std::abs(current - target) < 0.005f) return;

            auto overlay = cocos2d::CCNode::create();
            overlay->setID("force-169-overlay"_spr);

            if (current > target) {
                float barW = (winSize.width - winSize.height * target) / 2.f;

                auto left = cocos2d::CCLayerColor::create({0, 0, 0, 255}, barW, winSize.height);
                left->setPosition({0, 0});
                overlay->addChild(left);

                auto right = cocos2d::CCLayerColor::create({0, 0, 0, 255}, barW, winSize.height);
                right->setPosition({winSize.width - barW, 0});
                overlay->addChild(right);
            } else {
                float barH = (winSize.height - winSize.width / target) / 2.f;

                auto top = cocos2d::CCLayerColor::create({0, 0, 0, 255}, winSize.width, barH);
                top->setPosition({0, winSize.height - barH});
                overlay->addChild(top);

                auto bottom = cocos2d::CCLayerColor::create({0, 0, 0, 255}, winSize.width, barH);
                bottom->setPosition({0, 0});
                overlay->addChild(bottom);
            }

            ui->addChild(overlay, -999999);
            m_fields->m_overlay = overlay;
            m_fields->m_size = winSize;
        }

        void updateVisibility(float dt) override {
            bool want = config::get<"global.forceaspectratio", bool>();
            bool has = m_fields->m_overlay != nullptr;

            if (!want) {
                if (has) this->removeOverlay();
            } else {
                auto director = utils::get<cocos2d::CCDirector>();
                auto winSize = director ? director->getWinSize() : cocos2d::CCSize{0.f, 0.f};

                if (!has || winSize.width != m_fields->m_size.width || winSize.height != m_fields->m_size.height)
                    this->updateOverlay();
            }

            PlayLayer::updateVisibility(dt);
        }

        void onExit() override {
            this->removeOverlay();
            PlayLayer::onExit();
        }
    };
}
