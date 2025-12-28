use macroquad::prelude::*;

use crate::{options::editor_pallete::{BACKGROUND_COLOR, IDENTIFIER_COLOR}, text::{editor_cursor::CURSOR_LINE_COLOR, editor_token::EditorToken}};

pub struct EditorTokenAutocomplete {
    pub matching_to_fragment: Vec<String>,
}

impl EditorTokenAutocomplete {
    pub fn new() -> EditorTokenAutocomplete {
        EditorTokenAutocomplete {
            matching_to_fragment: vec![],
        }
    }

    /// TODO: May need to optimize this
    /// Find a matching pattern of the current cursor fragment, to match with an already
    /// existing token!
    pub fn find_autocompletes (
        &mut self,
        cursor_fragment: &str,
        tokens: &[Vec<EditorToken>], // Specific lifetime?
    ) -> Vec<String> {
        tokens
            .iter()
            .flat_map(|row| row.iter())
            .filter(|t| t.text.starts_with(cursor_fragment) && t.text != " ")
            .map(|t| t.text.clone()) // Copy to a new vector
            .collect()
    }

    /// Draw autocomplete contents if any.
    pub fn draw(
        &self,
        cursor_xy: (f32, f32)
    ) {
        if self.matching_to_fragment.is_empty() {
            return; // Nothing to draw, no autocomplete from cursor fragment
        }

        let entries = self.matching_to_fragment.len();

        if entries > 20 {
            return; // We have no idea still..
        }

        // Shadow the previous type
        let entries = entries as f32;

        const WORD_HEIGHT: f32 = 25.0;
        const BOX_WIDTH: f32 = 240.0;

        draw_rectangle(cursor_xy.0, cursor_xy.1, BOX_WIDTH, entries * WORD_HEIGHT, CURSOR_LINE_COLOR);
        draw_rectangle(cursor_xy.0 + 1.0, cursor_xy.1 + 1.0, BOX_WIDTH - 2.0, entries * WORD_HEIGHT - 2.0, BACKGROUND_COLOR);

        for i in 0..entries as usize {
            draw_text(self.matching_to_fragment[i].clone(), cursor_xy.0 as f32,
                cursor_xy.1 as f32 + i as f32 * WORD_HEIGHT + WORD_HEIGHT, WORD_HEIGHT, IDENTIFIER_COLOR);
        }
    }
}
