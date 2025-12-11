// Cursor navigation and
// kickback module

use std::collections::{HashMap};

use macroquad::prelude::*;
use miniquad::date;

use crate::{audio::editor_audio::*, camera::editor_camera::EditorCamera, text::editor_input::TAB_PATTERN};

pub const CURSOR_WORD_OFFSET: f32 = 600.0;

pub const CURSOR_CONTINUOUS_PRESS_INITIAL_DELAY: f64 = 0.02;

pub const CURSOR_CONTINUOUS_PRESS_DELAY: f64 = 0.08;

pub const CURSOR_LINE_COLOR: Color = Color::new(1.0, 1.0, 1.0, 0.05);

pub const CURSOR_HEIGHT: f32 = 3.5;

pub struct EditorCursor {
    pub xy: (usize, usize),
    pub word: String,
    pub key_timers: HashMap<(KeyCode, Option<KeyCode>), f64>,
    pub anim_x: f32,
    pub anim_y: f32,
    pub vel_x: f32,
    pub vel_y: f32,
    pub select_mode: bool,
    pub select_xy: (usize, usize)
}

impl EditorCursor {
    pub fn new() -> EditorCursor {
        EditorCursor {
            xy: (0, 0),
            word: String::from(""),
            key_timers: HashMap::new(),
            anim_x: 0.0,
            anim_y: 0.0,
            vel_x: 0.0,
            vel_y: 0.0,
            select_mode: false,
            select_xy: (0, 0)
        }
    }

    /// Draw selection highlight
    pub fn draw_selection(
        &self,
        text: &[String],
        start_x: f32,
        start_y: f32,
        line_spacing: f32,
        font: &Font,
        font_size: u16,
        camera: &EditorCamera,
        line_start_fix: f32,
        text_y_offset: f32,
    ) {
        if !self.select_mode {
            return;
        }

        // Determine start and end points
        let (start_line, start_col, end_line, end_col) = 
            if self.xy.1 < self.select_xy.1 || 
               (self.xy.1 == self.select_xy.1 && self.xy.0 < self.select_xy.0) {
                (self.xy.1, self.xy.0, self.select_xy.1, self.select_xy.0)
            } else {
                (self.select_xy.1, self.select_xy.0, self.xy.1, self.xy.0)
            };

        // Draw selection for each line
        for line_idx in start_line..=end_line {
            if line_idx >= text.len() {
                break;
            }

            let line = &text[line_idx];
            
            // Determine start and end columns for this line
            let col_start = if line_idx == start_line { start_col } else { 0 };
            let col_end = if line_idx == end_line { end_col } else { line.chars().count() };

            // Get byte indices
            let byte_start = char_to_byte(line, col_start);
            let byte_end = char_to_byte(line, col_end);

            // Measure text positions
            let prefix = &line[..byte_start];
            let selected = &line[byte_start..byte_end];
            
            let visual_prefix = prefix.replace("\t", TAB_PATTERN);
            let visual_selected = selected.replace("\t", TAB_PATTERN);
            
            let prefix_width = measure_text(&visual_prefix, Some(font), font_size, 1.0).width;
            let selected_width = measure_text(&visual_selected, Some(font), font_size, 1.0).width;

            // Calculate world position
            let world_x = start_x + line_start_fix + prefix_width;
            let world_y = start_y + line_idx as f32 * line_spacing + text_y_offset;

            // Convert to screen coordinates
            let (screen_x, screen_y) = camera.world_to_screen(world_x, world_y);

            // Draw selection rectangle
            draw_rectangle(
                screen_x.round(),
                screen_y.round() - font_size as f32 + CURSOR_HEIGHT,
                selected_width,
                font_size as f32,
                Color::from_rgba(100, 150, 255, 80) // Light blue with transparency
            );
        }
    }

    /// Highlight the line the cursor is at currently
    pub fn draw_cursor_line(
        &self,
        x: f32,
        y: f32,
        font_size: f32
    ) {
        draw_rectangle(
            x,
            y + CURSOR_HEIGHT,
            screen_width(),
             font_size,
            CURSOR_LINE_COLOR
        );
    }

    /// Returns true if key is pressed with continuous repeat
    pub fn is_combo_active(&mut self, key: KeyCode, modifier: Option<KeyCode>) -> bool {
        if is_key_down(key) && modifier.map_or(true, |m| is_key_down(m)) {
            let now = date::now();

            let timer = self.key_timers.entry((key, modifier)).or_insert(now + CURSOR_CONTINUOUS_PRESS_INITIAL_DELAY);

            if now >= *timer {
                // Set next repeat
                *timer = now + CURSOR_CONTINUOUS_PRESS_DELAY;
                return true;
            } else {
                return false;
            }
        } else {
            self.key_timers.remove(&(key, modifier));
            return false;
        }
    }

    /// Interpolate cursor movement
    pub fn animate_to(&mut self, target_x: f32, target_y: f32) {
        let stiffness = 0.76;
        let damping   = 0.34;
    
        let dx = target_x - self.anim_x;
        let dy = target_y - self.anim_y;
    
        self.vel_x += dx * stiffness;
        self.vel_y += dy * stiffness;
    
        self.vel_x *= damping;
        self.vel_y *= damping;
    
        self.anim_x += self.vel_x;
        self.anim_y += self.vel_y;
    }
}

/// Convert character index to byte index for UTF-8 strings
pub fn char_to_byte(line: &str, char_idx: usize) -> usize {
    line.char_indices().nth(char_idx).map(|(b, _)| b).unwrap_or(line.len())
}

/// Find the cursor's word fragment
pub fn recognize_cursor_word(
    cursor: &mut EditorCursor,
    line: &String
) {
    // Find the character collection of the word, left and right
    // from the word_idx

    let cursor_idx = cursor.xy.0;
    let left_distance = calibrate_distance_to_whitespace_or_character(false, cursor_idx, line);
    let right_distance = calibrate_distance_to_whitespace_or_character(true, cursor_idx, line);
     
    // Index of where the word starts
    let left_cursor_idx = cursor_idx - left_distance;
    let right_cursor_idx = right_distance + cursor_idx;

    // Convert char indices → byte indices
    let left_byte_idx = char_to_byte(line, left_cursor_idx);
    let right_byte_idx = char_to_byte(line, right_cursor_idx);
    
    cursor.word = line[left_byte_idx..right_byte_idx].to_string();
}

/// Standard cursor navigation (with repeat timer)
pub fn file_text_navigation(
    cursor: &mut EditorCursor,
    text: &mut Vec<String>,
    audio: &mut EditorAudio,
) {
    if text.is_empty() {
        cursor.xy = (0, 0);
        return;
    }

    cursor.xy.1 = cursor.xy.1.min(text.len() - 1);
    cursor.xy.0 = cursor.xy.0.min(text[cursor.xy.1].len());

    // Up
    if cursor.is_combo_active(KeyCode::Up, None) && cursor.xy.1 > 0 {
        cursor.xy.1 -= 1;
        cursor.xy.0 = cursor.xy.0.min(text[cursor.xy.1].len());
        audio.play_nav();
    }

    // Down
    if cursor.is_combo_active(KeyCode::Down, None) && cursor.xy.1 + 1 < text.len() {
        cursor.xy.1 += 1;
        cursor.xy.0 = cursor.xy.0.min(text[cursor.xy.1].len());
        audio.play_nav();
    }

    // Left
    if cursor.is_combo_active(KeyCode::Left, None) {
        if cursor.xy.0 > 0 {
            cursor.xy.0 -= 1;
        } else if cursor.xy.1 > 0 {
            cursor.xy.1 -= 1;
            cursor.xy.0 = text[cursor.xy.1].len();
        }
        audio.play_nav();
    }

    // Right
    if cursor.is_combo_active(KeyCode::Right, None) {
        if cursor.xy.0 < text[cursor.xy.1].len() {
            cursor.xy.0 += 1;
        } else if cursor.xy.1 + 1 < text.len() {
            cursor.xy.1 += 1;
            cursor.xy.0 = 0;
        }
        audio.play_nav();
    }

    recognize_cursor_word(cursor, &text[cursor.xy.1]);
}

/// Special navigation with LCTRL movement
pub fn file_text_special_navigation(
    cursor: &mut EditorCursor, 
    text: &mut Vec<String>, 
    audio: &mut EditorAudio,
) {
    if text.is_empty() {
        cursor.xy.0 = 0;
        cursor.xy.1 = 0;
        return;
    }

    // Clamp cursor to valid line
    cursor.xy.1 = cursor.xy.1.min(text.len() - 1);
    cursor.xy.0 = cursor.xy.0.min(text[cursor.xy.1].len());
    
    let line_len = text[cursor.xy.1].len();
    let left_steps_to_whitespace = calibrate_distance_to_whitespace_or_character(false, cursor.xy.0, &text[cursor.xy.1]);
    let right_steps_to_whitespace = calibrate_distance_to_whitespace_or_character(true, cursor.xy.0, &text[cursor.xy.1]);

    if cursor.is_combo_active(KeyCode::Left, None) {
        if cursor.xy.0 > 0 {
            cursor.xy.0 = cursor.xy.0.saturating_sub(left_steps_to_whitespace);
        } else if cursor.xy.1 > 0 {
            cursor.xy.1 -= 1;
            cursor.xy.0 = text[cursor.xy.1].len();
        }

        audio.play_nav();
    }

    if cursor.is_combo_active(KeyCode::Right, None) {
        if cursor.xy.0 < line_len {
            cursor.xy.0 += right_steps_to_whitespace.min(line_len - cursor.xy.0);
        } else if cursor.xy.1 + 1 < text.len() {
            cursor.xy.1 += 1;
            cursor.xy.0 = 0;
        }

        audio.play_nav();
    }
    
    // Vertical step
    let cursor_vertical_step = 5; 

    if cursor.is_combo_active(KeyCode::Up, None) {
        if cursor.xy.1 > cursor_vertical_step {
            cursor.xy.1 -= cursor_vertical_step;
            cursor.xy.0 = cursor.xy.0.min(text[cursor.xy.1].len());
        } else {
            cursor.xy.1 = 0;
        }
        
        audio.play_nav();
    }

    if cursor.is_combo_active(KeyCode::Down, None) {
        if cursor.xy.1 + cursor_vertical_step < text.len() {
            cursor.xy.1 += cursor_vertical_step;
            cursor.xy.0 = cursor.xy.0.min(text[cursor.xy.1].len());
        } else {
            cursor.xy.1 = text.len() - 1;
        }
            
        audio.play_nav();
    }

    recognize_cursor_word(cursor, &text[cursor.xy.1]);
}

/// Calculate the distance from the left or right 
/// to a whitepsace based on the cursor's position
/// return the distance
#[allow(dead_code)]
pub fn calibrate_distance_to_whitespace(
    leftorright: bool,
    cursor_idx: usize,
    line: &str,    
) -> usize {
    let chars: Vec<char> = line.chars().collect();
    let len = chars.len();
    if len == 0 {
        return 0;
    }
    
    let mut cursor = cursor_idx.min(len);
    let mut steps = 0;
    
    // True right, false left
    if leftorright {
        if cursor >= len {
            return 0;
        }
        
        for i in cursor..len {
            if chars[i] == ' ' {
                break;
            }
            
            steps += 1;
        }
        
        return steps;
    } else {
        if cursor == 0 {
            return 0;
        }
        
        cursor -= 1;
        
        while cursor > 0 {
            if chars[cursor - 1] == ' ' {
                break;
            }
            
            cursor -= 1;
            steps += 1;
        }
         
        steps + 1
    }
    
}

/// Calculate the distance from the cursor to the next "word boundary"
/// A word boundary is: whitespace, punctuation, or parentheses/brackets
pub fn calibrate_distance_to_whitespace_or_character(
    left_or_right: bool,
    cursor_idx: usize,
    line: &str
) -> usize {
    let chars: Vec<char> = line.chars().collect();
    let len = chars.len();
    if len == 0 {
        return 0;
    }

    let mut steps = 0;

    if left_or_right {
        // Move right
        let mut i = cursor_idx.min(len);
        while i < len {
            let c = chars[i];
            if !c.is_alphanumeric() {
                break;
            }
            i += 1;
            steps += 1;
        }
    } else {
        // Move left
        let mut i = cursor_idx.min(len);
        while i > 0 {
            let c = chars[i - 1];
            if !c.is_alphanumeric() {
                break;
            }
            i -= 1;
            steps += 1;
        }
    }

    steps
}
