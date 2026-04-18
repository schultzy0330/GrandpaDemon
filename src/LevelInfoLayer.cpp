#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <vector>
#include <string>
#include "ListManager.h"
#include "EffectsManager.h"
#include "ParticleManager.h"

using namespace geode::prelude;

class $modify(GrDInfoLayer, LevelInfoLayer) {

    struct Fields {
        bool m_hasBeenOpened = false;
    };
    
    void updateDifficultyFace() {
        int aredlPos = ListManager::getPositionOfID(m_level->m_levelID);
        if (aredlPos == -1 || aredlPos > 499) {
            return;
        }

        CCSprite* originalIcon = nullptr;
        bool iconFound = false;

        for (auto* obj : this->getChildrenExt()) {
            if (CCSprite* newObj = dynamic_cast<CCSprite*>(obj)) {
                if (newObj->getPosition() == m_difficultySprite->getPosition() &&
                    newObj->getZOrder() == 3) {
                    originalIcon = newObj;
                    iconFound = true;
                    break;
                }
            }
        }

        if (!originalIcon || !iconFound) {
            auto alert = FLAlertLayer::create(
                "Error",
                "There was a problem loading the demon difficulty face.\nYour sceen resolution may not be supported.\n\n<cb>-Grandpa Demon</c>",
                "OK"
            );
            alert->m_scene = this;
            alert->show();
            return;
        }

        CCSprite* newIcon = ListManager::getSpriteFromPosition(aredlPos, true);
        if (!newIcon) {
            return;
        }

        newIcon->setID("grd-difficulty");
        
        auto newPos = originalIcon->getPosition();
        newIcon->setPosition(originalIcon->getPosition());
        newIcon->setZOrder(originalIcon->getZOrder() + 10);

        for (auto* clearObj : originalIcon->getChildrenExt()) {
            if (CCSprite* newObj = dynamic_cast<CCSprite*>(clearObj)) {
                if (newObj->getTag() == 69420) {
                    newObj->removeFromParentAndCleanup(true);
                }
            }
        }

        for (auto* iconObj : originalIcon->getChildrenExt()) {
            if (CCSprite* newObj = dynamic_cast<CCSprite*>(iconObj)) {
                newObj->setTag(69420);
                this->addChild(newObj);
                newObj->setPosition(newPos);
            }
        }

        originalIcon->setVisible(false);
        this->addChild(newIcon);
        
        if (m_fields->m_hasBeenOpened) {
            return;
        }

        if (aredlPos <= 24) {
            EffectsManager::infinityBackground(this, aredlPos);
            EffectsManager::addInfinitySymbol(newIcon->getPosition(), this, aredlPos);

            if (!Mod::get()->getSettingValue<bool>("particles-disable")) {
                bool isGrandpa = false;

                if (aredlPos == 0 && !Mod::get()->getSettingValue<bool>("grandpa-demon-disable")) {
                    isGrandpa = true;
                }

                auto particle1 = ParticleManager::infiniteParticles1(50, isGrandpa);
                if (particle1) {
                    particle1->setPosition({ newIcon->getPositionX(), newIcon->getPositionY() + 5.f });
                    this->addChild(particle1);
                }

                auto particle2 = ParticleManager::infiniteParticles2(50);
                if (particle2) {
                    particle2->setPosition({ newIcon->getPositionX(), newIcon->getPositionY() + 5.f });
                    this->addChild(particle2);
                }
            }
        }

        if (aredlPos <= 74 && aredlPos > 24) {
            EffectsManager::mythicalBackground(this, aredlPos);

            if (!Mod::get()->getSettingValue<bool>("particles-disable")) {
                auto particle = ParticleManager::mythicalParticles(50);
                if (particle) {
                    particle->setPosition({ newIcon->getPositionX(), newIcon->getPositionY() + 5.f });
                    this->addChild(particle);
                }
            }
        }

        if (aredlPos <= 149 && aredlPos > 74) {
            EffectsManager::legendaryBackground(this, aredlPos);

            if (!Mod::get()->getSettingValue<bool>("particles-disable")) {
                auto particle = ParticleManager::legendaryParticles(50);
                if (particle) {
                    particle->setPosition({ newIcon->getPositionX(), newIcon->getPositionY() + 5.f });
                    this->addChild(particle);
                }
            }
        }
        
        m_fields->m_hasBeenOpened = true;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();

        if (ListManager::demonIDList.empty()) {
            return;
        }

        if (this->m_level->m_stars != 10) {
            return;
        }
        
        updateDifficultyFace();
    }
};
