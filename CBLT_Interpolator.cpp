#include "CBLT_Interpolator.hpp"

namespace CBLT {
    Interpolator::Interpolator() : progress(0.0f), speed(0.1f), active(false) {}

    void Interpolator::Start(UT::i32 fromX, UT::i32 fromY, UT::i32 toX, UT::i32 toY, UT::f32 speedPerFrame) {
        al.sourceX = fromX;
        al.sourceY = fromY;
        al.targetX = toX;
        al.targetY = toY;
        speed = speedPerFrame;
        progress = 0.0f;
        active = true;
    }

    std::pair<UT::i32, UT::i32> Interpolator::Update() {
        if (!active) return {al.targetX, al.targetY};

        progress += speed;
        if (progress >= 1.0f) {
            progress = 1.0f;
            active = false;
        }

        UT::i32 x = static_cast<UT::i32>(al.sourceX + (al.targetX - al.sourceX) * progress);
        UT::i32 y = static_cast<UT::i32>(al.sourceY + (al.targetY - al.sourceY) * progress);

        return {x, y};
    }

    UT::b Interpolator::IsActive() const {
        return active;
    }

    void Interpolator::Stop() {
        active = false;
        progress = 1.0f;
    }
} // CBLT