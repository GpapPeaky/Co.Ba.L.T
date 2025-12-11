// Cursor navigation and
// kickback module

use std::collections::HashMap;

use macroquad::prelude::*;
use miniquad::date;

use crate::{audio::editor_audio::*, text::editor_cursor::{CURSOR_CONTINUOUS_PRESS_DELAY, CURSOR_CONTINUOUS_PRESS_INITIAL_DELAY}};

pub struct EditorConsoleCursor {
    pub x: usize,
    pub _key_timers: HashMap<(KeyCode, Option<KeyCode>), f64>,
    pub anim_x: f32,
    pub vel_x: f32,
}

impl EditorConsoleCursor {
    pub fn new() -> EditorConsoleCursor {
        EditorConsoleCursor {
            x: 0,
            _key_timers: HashMap::new(),
            anim_x: 0.0,
            vel_x: 0.0
        }
    }

    /// Interpolate console cursor position
    pub fn animate_to(
        &mut self,
        target_x: f32
    ) {
        let stiffness = 0.41;
        let damping   = 0.57;
    
        let dx = target_x - self.anim_x;
    
        self.vel_x += dx * stiffness;
    
        self.vel_x *= damping;
    
        self.anim_x += self.vel_x;
    }

    /// Returns true if key is pressed with continuous repeat
    pub fn _is_combo_active(&mut self, key: KeyCode, modifier: Option<KeyCode>) -> bool {
        if is_key_down(key) && modifier.map_or(true, |m| is_key_down(m)) {
            let now = date::now();

            let timer = self._key_timers.entry((key, modifier)).or_insert(now + CURSOR_CONTINUOUS_PRESS_INITIAL_DELAY);

            if now >= *timer {
                // Set next repeat
                *timer = now + CURSOR_CONTINUOUS_PRESS_DELAY;
                return true;
            } else {
                return false;
            }
        } else {
            self._key_timers.remove(&(key, modifier));
            return false;
        }
    }
}

/// Standard console cursor navigation
pub fn console_text_navigation(
    cursor_x: &mut usize, 
    directive: &mut String, 
    audio: &mut EditorAudio
) {
    let cursor_x_pos = *cursor_x as i32;

    if is_key_pressed(KeyCode::Left) {
        if cursor_x_pos > 0 {
            audio.play_nav();
            *cursor_x -= 1;
        }
    }

    if is_key_pressed(KeyCode::Right) {
        if cursor_x_pos < directive.chars().count() as i32 {
            audio.play_nav();
            *cursor_x += 1;
        }
    }
}

