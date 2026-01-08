#pragma once

#include "CBLT_Util.hpp"

namespace CBLT {

    // Animation line for interpolation
    typedef struct AnimationLine {
        UT::i32 targetX;            // Where to go at x
        UT::i32 targetY;            // Where to go at y
        UT::i32 sourceX;            // Where from at x
        UT::i32 sourceY;            // Where from at y            
    } AnimationLine;

    class Interpolator {
        private:
            AnimationLine al;   // Animation line to follow
            UT::f32 progress;   // 0.0 at source -> 1.0 at target
            UT::f32 speed;      // How fast the interpolation is moving
            UT::b active;       // Interpolation ongoing

        public:
            // Constructor
            Interpolator(void);

            // Start a new interpolation
            void Start(UT::i32 fromX, UT::i32 fromY, UT::i32 toX, UT::i32 toY, UT::f32 speedPerFrame = 0.1f);
    
            // Update interpolation and return current position
            std::pair<UT::i32, UT::i32> Update(void);
    
            // Check if interpolation is active
            UT::b IsActive(void) const;
    
            // Stop or reset interpolation
            void Stop(void);
    }; // Interpolator class
} // CBLT