#include "CBLT_Interpolator.hpp"

namespace CBLT {
    Interpolator::Interpolator() : progress(0.0f), speed(0.1f), active(false) {}

    void Interpolator::Start(UT::f32 fromX, UT::f32 fromY, UT::f32 toX, UT::f32 toY, UT::f32 speedPerFrame) {
        al.sourceX = fromX;
        al.sourceY = fromY;
        al.targetX = toX;
        al.targetY = toY;
        speed = speedPerFrame;
        progress = 0.0f;
        active = true;
    }

    std::pair<UT::f32, UT::f32> Interpolator::Update() {
        if (!active) return {al.targetX, al.targetY};

        progress += speed;
        if (progress >= 1.0f) {
            progress = 1.0f;
            active = false;
        }

        UT::f32 x = al.sourceX + (al.targetX - al.sourceX) * progress;
        UT::f32 y = al.sourceY + (al.targetY - al.sourceY) * progress;

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