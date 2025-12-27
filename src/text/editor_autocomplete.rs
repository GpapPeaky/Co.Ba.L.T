use macroquad::prelude::*;

use crate::text::editor_token::EditorToken;

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
            .filter(|t| t.text.starts_with(cursor_fragment))
            .map(|t| t.text.clone()) // Copy to a new vector
            .collect()
    }

    /// Draw autocomplete contents if any.
    pub fn draw(
        &self,
        cursor_xy: (usize, usize)
    ) {
        if self.matching_to_fragment.is_empty() {
            return; // Nothing to draw, no autocomplete from cursor fragment
        }


        // TODO
    }
}
