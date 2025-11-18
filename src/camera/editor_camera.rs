use macroquad::window::{screen_height, screen_width};
use crate::text::editor_text::*;

pub struct EditorCamera {
    pub offset_x: f32,
    pub offset_y: f32,
    pub camera_w: f32,
    pub camera_h: f32,
}

impl EditorCamera {
    pub fn new() -> EditorCamera {
        EditorCamera {
            offset_x: 0.0,
            offset_y: 0.0,
            camera_w: screen_width(),
            camera_h: screen_height(),
        }
    }

    /// Cursor-following camera movement
    pub fn follow_cursor(&mut self, cursor_x_px: f32, cursor_y_px: f32) {
        let follow_margin = 50.0;
        let top_ui_height = MODE_Y_MARGIN + MODE_FONT_SIZE + 25.0;
        let left_ui_width = FILE_TEXT_X_MARGIN + 80.0;

        let right_boundary = self.offset_x + self.camera_w - follow_margin;
        let left_boundary = self.offset_x + left_ui_width + follow_margin;

        if cursor_x_px > right_boundary {
            self.offset_x += cursor_x_px - right_boundary;
        } else if cursor_x_px < left_boundary {
            self.offset_x = (self.offset_x - (left_boundary - cursor_x_px)).max(0.0);
        }

        let bottom_boundary = self.offset_y + self.camera_h - follow_margin;
        let top_boundary = self.offset_y + top_ui_height + follow_margin;

        if cursor_y_px > bottom_boundary {
            self.offset_y += cursor_y_px - bottom_boundary;
        } else if cursor_y_px < top_boundary {
            self.offset_y = (self.offset_y - (top_boundary - cursor_y_px)).max(0.0);
        }
    }

    /// Convert world coordinates to screen coordinates
    pub fn world_to_screen(&self, x: f32, y: f32) -> (f32, f32) {
        (x - self.offset_x, y - self.offset_y)
    }
}
