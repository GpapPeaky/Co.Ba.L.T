// Editor text manipulation and rendering 
// using a regex pattern to match highlight 
// colouring.
//
// The editor's identation can be switched on/off
// through the console.

use macroquad::prelude::*;

use crate::console::editor_console::*;
use crate::options::editor_pallete::*;
use crate::text::editor_autocomplete::EditorTokenAutocomplete;
use crate::text::editor_cursor::*;
use crate::text::editor_input::*;
use crate::text::editor_text_stylizer::*;
use crate::camera::editor_camera::*;
use crate::text::editor_token::EditorToken;

pub const CURRENT_FILE_TOP_BAR_OFFSET: f32 = 100.0;

pub const MODE_FONT_SIZE: f32 = 30.0;
pub const MODE_Y_MARGIN: f32 = 10.0;
pub const MODE_Y_OFFSET: f32 = 15.0;

pub const FILE_LINE_NUMBER_X_MARGIN: f32 = 5.0;
pub const FILE_LINE_NUMBER_Y_MARGIN: f32 = 26.0;

pub const FILE_TEXT_X_MARGIN: f32 = 50.0;
pub const FILE_TEXT_Y_MARGIN: f32 = 80.0;

/// Find a word in the text 
/// and move the cursor there
/// return true if found, false if not
pub fn find_word_in_text(
    word: &str,
    text: &Vec<String>,
    cursor: &mut EditorCursor
) -> bool {
    if let Some(line_index) = text.iter().position(|line| line.contains(word)) {
        cursor.xy.1 = line_index;
        true
    } else {
        false
    }
}

/// General file text drawing via tokenization
/// as well as everything else
pub fn draw(
    tokens: &Vec<Vec<EditorToken>>,
    cursor: &mut EditorCursor,
    gts: &mut EditorGeneralTextStylizer,
    console: &EditorConsole,
    camera: &mut EditorCamera,
    file_text: &mut Vec<String>,
    eta: &mut EditorTokenAutocomplete
) {
    let text_y_offset = 25.0;
    let start_x = FILE_TEXT_X_MARGIN;
    let start_y = FILE_TEXT_Y_MARGIN;
    let line_spacing = gts.font_size as f32;
    let line_start_fix = gts.font_size as f32 * 1.5;

    let cam_top = camera.offset_y;
    let cam_bottom = camera.offset_y + screen_height();

    // Draw visible tokens
    let first_line = ((cam_top - start_y) / line_spacing).max(0.0) as usize;
    let last_line = ((cam_bottom - start_y) / line_spacing)
        .min(tokens.len() as f32 - 1.0) as usize;

    // Draw selection before cursor
    if cursor.select_mode {
        cursor.draw_selection(
            file_text,
            start_x,
            start_y,
            line_spacing,
            &gts.font,
            gts.font_size,
            camera,
            line_start_fix,
            text_y_offset,
        );
    }

    for (i, line) in tokens.iter().enumerate().skip(first_line).take(last_line - first_line + 1) {
        let y = start_y + i as f32 * line_spacing + text_y_offset;
        let mut x = start_x + line_start_fix;

        for token in line {
            let width = measure_text(&token.text, Some(&gts.font), gts.font_size, 1.0).width;
            let (sx, sy) = camera.world_to_screen(x, y);
            gts.color = token.color;
            gts.draw(&token.text, sx, sy);
            x += width;
        }
    }

    // Draw cursor (by character, not token)
    if !console.mode && !tokens.is_empty() && cursor.xy.1 < tokens.len() {
        let line_tokens = &tokens[cursor.xy.1];
        let mut prefix_text = String::new();
        let mut char_count = 0;

        'outer: for token in line_tokens {
            for ch in token.text.chars() {
                if char_count == cursor.xy.0 {
                    break 'outer;
                }
                prefix_text.push(ch);
                char_count += 1;
            }
        }

        let visual_prefix = prefix_text.replace("\t", TAB_PATTERN);
        let text_before_cursor = measure_text(&visual_prefix, Some(&gts.font), gts.font_size, 1.0);

        let logical_x = start_x + line_start_fix + text_before_cursor.width;
        let logical_y = start_y + cursor.xy.1 as f32 * line_spacing + text_y_offset;

        cursor.animate_to(logical_x, logical_y);
        camera.follow_cursor(cursor.anim_x, cursor.anim_y);

        let (sx, sy) = camera.world_to_screen(cursor.anim_x, cursor.anim_y);
        let cursor_width = gts.font_size as f32 / 7.5;

        cursor.draw_cursor_line(start_x, sy - gts.font_size as f32, gts.font_size as f32);
        draw_rectangle(
            sx.round(),
            sy - gts.font_size as f32 + CURSOR_HEIGHT,
            cursor_width,
            gts.font_size as f32,
            CURSOR_COLOR,
        );

        if cursor.word != "" && cursor.word != " " {
            // Draw autocompletes
            eta.draw((
                sx.round(),
                sy - gts.font_size as f32
            ));
        }
    }

    // Sidebar
    let sidebar_width = start_x + line_start_fix - 5.0;
    draw_rectangle(0.0, 0.0, sidebar_width, screen_height(), COMPOSITE_TYPE_COLOR);
    draw_rectangle(0.0, 0.0, sidebar_width - 1.0, screen_height(), BACKGROUND_COLOR);

    // Line numbers
    gts.color = CURSOR_COLOR;
    for i in first_line..=last_line {
        let line_y_world =
            1.1 * FILE_TEXT_X_MARGIN + FILE_LINE_NUMBER_Y_MARGIN + gts.font_size as f32 * i as f32
                + text_y_offset;
        let screen_y = line_y_world - camera.offset_y;
        gts.draw(&i.to_string(), FILE_LINE_NUMBER_X_MARGIN, screen_y);
    }

    // Top bar
    let top_bar_height = MODE_Y_MARGIN + MODE_FONT_SIZE + text_y_offset;
    draw_rectangle(0.0, 0.0, screen_width(), top_bar_height + 1.0, COMPOSITE_TYPE_COLOR);
    draw_rectangle(0.0, 0.0, screen_width(), top_bar_height, BACKGROUND_COLOR);

    // Cursor position display
    if !console.mode {
        let cursor_idx = format!("Ln {}, Col {}", cursor.xy.1, cursor.xy.0);
        gts.color = CONSOLE_TEXT_COLOR;
        let previous_size = gts.font_size;
        gts.font_size = 30;
        gts.draw(
            &cursor_idx,
            MODE_Y_OFFSET,
            MODE_FONT_SIZE + MODE_Y_MARGIN + MODE_Y_OFFSET,
        );
        gts.font_size = previous_size;
    }
}
