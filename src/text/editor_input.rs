// Text input module

use macroquad::prelude::*;

use crate::options::editor_options::EditorOptions;
use crate::text::editor_language_manager::EditorLanguageKeywords;
use crate::text::editor_text_stylizer::*;
use crate::text::editor_cursor::*;
use crate::text::editor_clipboard::*;

use crate::audio::editor_audio::*;
use crate::console::editor_console::*;
use crate::console::editor_file_system::*;
use crate::console::editor_directives::*;

pub const TAB_SIZE: usize = 4;
pub const TAB_PATTERN: &str = "    ";

fn lshift_shortcuts(
    cursor: &mut EditorCursor,
    text: &mut Vec<String>,
    _audio: &mut EditorAudio,
    _console: &mut EditorConsole,
    efs: &mut EditorFileSystem,
) -> bool {
    if cursor.is_combo_active(KeyCode::Up, Some(KeyCode::LeftShift)) && cursor.xy.1 > 0 {
        let current_line = cursor.xy.1;
        let swap_with = current_line - 1;
        text.swap(current_line, swap_with);
        efs.unsaved_changes = true;

        // Clamp cursor.x to new line length
        cursor.xy.0 = cursor.xy.0.min(text[swap_with].len());

        return true;
    }

    if cursor.is_combo_active(KeyCode::Down, Some(KeyCode::LeftShift)) && cursor.xy.1 + 1 < text.len() {
        let current_line = cursor.xy.1;
        let swap_with = current_line + 1;
        text.swap(current_line, swap_with);
        efs.unsaved_changes = true;

        // Clamp cursor.x to new line length
        cursor.xy.0 = cursor.xy.0.min(text[swap_with].len());

        return true;
    }

    false
}

/// Left control shortcuts (unchanged, using is_combo_active for repeats)
pub fn lctrl_shortcuts(
    cursor: &mut EditorCursor,
    text: &mut Vec<String>,
    audio: &mut EditorAudio,
    console: &mut EditorConsole,
    efs: &mut EditorFileSystem,
    gts: &mut EditorGeneralTextStylizer,
    ops: &mut EditorOptions,
    elk: &mut EditorLanguageKeywords,
) -> bool {
    if is_key_down(KeyCode::LeftControl) {
        if cursor.is_combo_active(KeyCode::X, None) && !text.is_empty() {
            audio.play_delete();
            efs.unsaved_changes = true;
            text.remove(cursor.xy.1);
            return true;
        }

        if cursor.is_combo_active(KeyCode::D, None) && !text.is_empty() {
            audio.play_insert();
            let line_clone = text[cursor.xy.1].clone();
            text.insert(cursor.xy.1 + 1, line_clone);
            return true;
        }

        // Save/write to file
        if is_key_pressed(KeyCode::S) {
            console.directive = ":w".to_string();
            execute_directive(&mut console.directive, efs, text, cursor, ops, elk);

            return true;
        }
        
        // Go to line
        if is_key_pressed(KeyCode::L) {
            console.directive = ":l ".to_string();
            console.mode = true;
            // Opens the console with the cursor right on where it needs to be
            console.cursor.x = console.directive.len();

            return true;
        }

        // Open native file explorer
        if is_key_pressed(KeyCode::O) {
            console.directive = ":O".to_string();
            execute_directive(&mut console.directive, efs, text, cursor, ops, elk);

            return true;
        }
        
        // Find infile
        if is_key_pressed(KeyCode::F) {
            console.directive = ":f ".to_string();
            console.cursor.x = console.directive.len();
            console.mode = true;            
        }

        // Open terminal
        if is_key_pressed(KeyCode::T) {
            console.directive = ":t $ ".to_string();
            console.cursor.x = console.directive.len();
            console.mode = true;

            return true;
        }
        
        // Create a new file
        if is_key_pressed(KeyCode::N) {
            console.directive = ":c f".to_string();
            execute_directive(&mut console.directive, efs, text, cursor, ops, elk);
            console.directive = ":b ".to_string();
            console.mode = true;
            console.cursor.x = console.directive.len();
        
            return true;
        }
        
        // 'Baptize' current file
        if is_key_pressed(KeyCode::B) {
            console.directive = ":b ".to_string();
            console.mode = true;
            console.cursor.x = console.directive.len();

            return true;
        }
        
        // Remove current file
        if is_key_pressed(KeyCode::R) {
            console.directive = ":r ".to_string();
            console.cursor.x = console.directive.len();
            console.mode = true;

            return true;
        }
        
        // Create directory
        if is_key_pressed(KeyCode::M) {
            console.directive = ":md ".to_string();
            console.mode = true;
            console.cursor.x = console.directive.len();

            return true;
        }

        // Delete the word that the cursor is currently at
        if is_key_pressed(KeyCode::W) {
            // Find the character collection of the word, left and right
            // from the word_idx

            let cursor_idx = cursor.xy.0;
            let left_distance = calibrate_distance_to_whitespace(false, cursor_idx, &text[cursor.xy.1]);
            let right_distance = calibrate_distance_to_whitespace(true, cursor_idx, &text[cursor.xy.1]);
            
            let left_cursor_idx = cursor_idx - left_distance;
            let word_len = cursor_idx + right_distance;
            
            // Actual deletion.            
            for _ in left_cursor_idx..word_len {
                let line = &mut text[cursor.xy.1];
                let byte_idx = char_to_byte(line, left_cursor_idx);
                if byte_idx < line.len() {
                    line.remove(byte_idx);
                }
            }
            
            audio.play_delete();
            efs.unsaved_changes = true;
            
            return true;
        }
        
        // Save and quit
        if is_key_pressed(KeyCode::Q) {
            console.directive = ":W".to_string();
            execute_directive(&mut console.directive, efs, text, cursor, ops, elk);
            console.directive = ":q".to_string();
            execute_directive(&mut console.directive, efs, text, cursor, ops, elk);
        }
        
        // Quit
        if is_key_pressed(KeyCode::E) {
            console.directive = ":e".to_string();
            execute_directive(&mut console.directive, efs, text, cursor, ops, elk);
        }

        // Console switch
        if is_key_pressed(KeyCode::GraveAccent) {
            console.mode = true; 

            return true;
        }

        if is_key_pressed(KeyCode::Minus) {
            if gts.font_size > 12 {
                gts.font_size -= 2;
            }

            return true;
        }
        
        if is_key_pressed(KeyCode::Equal) {
            if gts.font_size < 45 {
                gts.font_size += 2;
            }

            return true;
        }
        
        // Copy text
        if is_key_pressed(KeyCode::C) {
            let extract = if cursor.select_mode {
                // Determine start and end points
                let (start_line, start_col, end_line, end_col) = 
                    if cursor.xy.1 < cursor.select_xy.1 || 
                    (cursor.xy.1 == cursor.select_xy.1 && cursor.xy.0 < cursor.select_xy.0) {
                        (cursor.xy.1, cursor.xy.0, cursor.select_xy.1, cursor.select_xy.0)
                    } else {
                        (cursor.select_xy.1, cursor.select_xy.0, cursor.xy.1, cursor.xy.0)
                    };
                
                let mut selected = String::new();
                
                if start_line == end_line {
                    // Single line selection
                    let line = &text[start_line];
                    let byte_start = char_to_byte(line, start_col);
                    let byte_end = char_to_byte(line, end_col);
                    selected = line[byte_start..byte_end].to_string();
                } else {
                    // Multi-line selection
                    // First line: from start_col to end
                    let first_line = &text[start_line];
                    let byte_start = char_to_byte(first_line, start_col);
                    selected.push_str(&first_line[byte_start..]);
                    selected.push('\n'); // ADD NEWLINE
                    
                    // Middle lines: entire lines
                    for line_idx in (start_line + 1)..end_line {
                        selected.push_str(&text[line_idx]);
                        selected.push('\n'); // ADD NEWLINE
                    }
                    
                    // Last line: from start to end_col
                    let last_line = &text[end_line];
                    let byte_end = char_to_byte(last_line, end_col);
                    selected.push_str(&last_line[..byte_end]);
                    // NO newline after last line
                }
                
                selected
            } else {
                // No selection, copy current line
                text[cursor.xy.1].clone()
            };
        
            let _ = copy_to_clipboard(&extract);
        }
        
        // Paste text
        if is_key_pressed(KeyCode::V) {
            match paste_from_clipboard() {
                Ok(pasted_text) => {
                    let lines: Vec<&str> = pasted_text.split('\n').collect();
                    
                    if lines.len() == 1 {
                        // Single line paste - insert into current line
                        let line = &mut text[cursor.xy.1];
                        let byte_idx = char_to_byte(line, cursor.xy.0);
                        line.insert_str(byte_idx, &pasted_text);
                        
                        // Move cursor to end of pasted text
                        cursor.xy.0 += pasted_text.chars().count();
                    } else {
                        // Multi-line paste
                        let current_line = &text[cursor.xy.1];
                        let byte_idx = char_to_byte(current_line, cursor.xy.0);
                        
                        let before = current_line[..byte_idx].to_string();
                        let after = current_line[byte_idx..].to_string();
                        
                        // First line: existing beginning + first pasted line
                        text[cursor.xy.1] = format!("{}{}", before, lines[0]);
                        
                        // Insert middle lines (if any)
                        for i in 1..lines.len() - 1 {
                            text.insert(cursor.xy.1 + i, lines[i].to_string());
                        }
                        
                        // Last line: last pasted line + existing ending
                        let last_line = format!("{}{}", lines.last().unwrap(), after);
                        text.insert(cursor.xy.1 + lines.len() - 1, last_line);
                        
                        // Update cursor position
                        cursor.xy.1 += lines.len() - 1;
                        cursor.xy.0 = lines.last().unwrap().chars().count();
                    }
                    
                    // Exit selection mode after paste
                    cursor.select_mode = false;
                }
                Err(e) => eprintln!("Paste failed: {}", e),
            }
        }

        // Select mode switch
        if is_key_pressed(KeyCode::P) {
            // On exit, copy selection before toggling off
            if cursor.select_mode {
                // Determine start and end points
                let (start_line, start_col, end_line, end_col) = 
                    if cursor.xy.1 < cursor.select_xy.1 || 
                    (cursor.xy.1 == cursor.select_xy.1 && cursor.xy.0 < cursor.select_xy.0) {
                        (cursor.xy.1, cursor.xy.0, cursor.select_xy.1, cursor.select_xy.0)
                    } else {
                        (cursor.select_xy.1, cursor.select_xy.0, cursor.xy.1, cursor.xy.0)
                    };
                
                let mut selected = String::new();
                
                if start_line == end_line {
                    // Single line selection
                    let line = &text[start_line];
                    let byte_start = char_to_byte(line, start_col);
                    let byte_end = char_to_byte(line, end_col);
                    selected = line[byte_start..byte_end].to_string();
                } else {
                    // Multi-line selection
                    // First line: from start_col to end
                    let first_line = &text[start_line];
                    let byte_start = char_to_byte(first_line, start_col);
                    selected.push_str(&first_line[byte_start..]);
                    selected.push('\n');
                    
                    // Middle lines: entire lines
                    for line_idx in (start_line + 1)..end_line {
                        selected.push_str(&text[line_idx]);
                        selected.push('\n');
                    }
                    
                    // Last line: from start to end_col
                    let last_line = &text[end_line];
                    let byte_end = char_to_byte(last_line, end_col);
                    selected.push_str(&last_line[..byte_end]);
                }
                
                // Copy to clipboard
                let _ = copy_to_clipboard(&selected);
                
                // Reset selection
                cursor.select_xy = (0, 0);
            } else {
                // On entry, save the initial xy
                cursor.select_xy = cursor.xy;
            }
            
            // Toggle selection mode
            cursor.select_mode = !cursor.select_mode;
            
            return true;
        }

        file_text_special_navigation(cursor, text, audio);
        
        return true;
    }

    false
}

/// Record special key presses
pub fn record_special_keys(
    cursor: &mut EditorCursor,
    text: &mut Vec<String>,
    audio: &mut EditorAudio,
    console: &mut EditorConsole,
    gts: &mut EditorGeneralTextStylizer,
    efs: &mut EditorFileSystem,
    ops: &mut EditorOptions,
    elk: &mut EditorLanguageKeywords,
) -> bool {
    // Backspace
    if cursor.is_combo_active(KeyCode::Backspace, None) {
        audio.play_delete();
        efs.unsaved_changes = true;

        if text.is_empty() {
            return true;
        }

        // TODO: Add selected lines deleting

        let line = &mut text[cursor.xy.1];
        cursor.xy.0 = cursor.xy.0.min(line.chars().count());

        if cursor.xy.0 == 0 && cursor.xy.1 > 0 {
            let current_line = text.remove(cursor.xy.1);
            cursor.xy.1 -= 1;
            cursor.xy.0 = text[cursor.xy.1].chars().count();
            text[cursor.xy.1].push_str(&current_line);
            return true;
        }

        let cursor_pos = cursor.xy.0;

        if cursor_pos >= TAB_SIZE {
            let start = char_to_byte(line, cursor_pos - TAB_SIZE);
            let end = char_to_byte(line, cursor_pos);
            if &line[start..end] == TAB_PATTERN {
                line.replace_range(start..end, "");
                cursor.xy.0 -= TAB_SIZE;
                return true;
            }
        }

        if cursor_pos > 0 {
            let idx = char_to_byte(line, cursor_pos - 1);
            if idx < line.len() {
                line.remove(idx);
                cursor.xy.0 -= 1;
            }
        }

        return true;
    }

    // Tab insertion
    if cursor.is_combo_active(KeyCode::Tab, None) {
        audio.play_space();
        let line = &mut text[cursor.xy.1];
        let idx = char_to_byte(line, cursor.xy.0);
        line.insert_str(idx, TAB_PATTERN);
        cursor.xy.0 += TAB_SIZE;
        return true;
    }

    // Enter key (line splitting, indentation)
    if cursor.is_combo_active(KeyCode::Enter, None) {
        audio.play_return();
        efs.unsaved_changes = true;

        let cursor_pos = cursor.xy.0;
        let mut line = text.remove(cursor.xy.1);
        let split_index = char_to_byte(&line, cursor_pos);
        let mut rest_of_line = line.split_off(split_index);
        let base_indent: String = line.chars().take_while(|c| c.is_whitespace()).collect();
        let opener = line.trim_end().chars().last();
        let mut inner_indent = base_indent.clone();

        if let Some(opener) = opener {
            let expected_closer = match opener { '(' => ')', '{' => '}', '[' => ']', _ => '\0' };

            if expected_closer != '\0' && rest_of_line.starts_with(expected_closer) {
                rest_of_line = rest_of_line[expected_closer.len_utf8()..].to_string();
                inner_indent.push_str(TAB_PATTERN);
            }
        }

        text.insert(cursor.xy.1, line);
        cursor.xy.1 += 1;
        cursor.xy.0 = inner_indent.chars().count();
        text.insert(cursor.xy.1, format!("{}{}", inner_indent, rest_of_line));

        if let Some(opener) = opener {
            let closer = match opener { '(' => ')', '{' => '}', '[' => ']', _ => '\0' };

            if closer != '\0' {
                let mut next_line_idx = cursor.xy.1;

                while next_line_idx < text.len() && text[next_line_idx].trim().is_empty() {
                    next_line_idx += 1;
                }

                // Check if the already is a closer
                let should_insert_closer = if next_line_idx < text.len() {
                    !text[next_line_idx].trim_start().starts_with(closer)
                } else {
                    true
                };
                
                if should_insert_closer {
                    text.insert(cursor.xy.1 + 1, format!("{}{}", base_indent, closer));
                }
            }
        }
    }

    lshift_shortcuts(cursor, text, audio, console, efs);

    let is_lctrl = lctrl_shortcuts(cursor, text, audio, console, efs, gts, ops, elk);

    if !is_lctrl {
        file_text_navigation(cursor, text, audio);
    }

    false
}

/// Standard key recording function
pub fn record_keyboard_to_file_text(
    cursor: &mut EditorCursor,
    text: &mut Vec<String>,
    audio: &mut EditorAudio,
    console: &mut EditorConsole,
    gts: &mut EditorGeneralTextStylizer,
    efs: &mut EditorFileSystem,
    ops: &mut EditorOptions,
    elk: &mut EditorLanguageKeywords,
) {
    if text.is_empty() { text.push(String::new()); }

    if record_special_keys(cursor, text, audio, console, gts, efs, ops, elk) {
        return;
    }

    if let Some(c) = get_char_pressed() {
        if c.is_control() || c.is_ascii_control() { return; }

        let line = text.get_mut(cursor.xy.1).unwrap();
        efs.unsaved_changes = true;

        let idx = char_to_byte(line, cursor.xy.0);
            
        // Next opener/closer autocomplete.
        match c {
            '(' => {
                line.insert(idx, c);
                let idx_next = char_to_byte(line, cursor.xy.0 + 1);
                line.insert(idx_next, ')');
                cursor.xy.0 += 1;

                // recognize_cursor_word(cursor, &text[cursor.xy.1]);
            }
            
            '[' => {
                line.insert(idx, c);
                let idx_next = char_to_byte(line, cursor.xy.0 + 1);
                line.insert(idx_next, ']');
                cursor.xy.0 += 1;

                // recognize_cursor_word(cursor, &text[cursor.xy.1]);
            }
            
            '{' => {
                line.insert(idx, c);
                let idx_next = char_to_byte(line, cursor.xy.0 + 1);
                line.insert(idx_next, '}');
                cursor.xy.0 += 1;

                // recognize_cursor_word(cursor, &text[cursor.xy.1]);
            }
            
            
            '"' => {
                if next_char_is('"', cursor, line) {
                    // Just move cursor over existing quote
                    cursor.xy.0 += 1;
                } else {
                    line.insert(idx, '"');
                    line.insert(idx + 1, '"');
                    cursor.xy.0 += 1;
                }

                // recognize_cursor_word(cursor, &text[cursor.xy.1]);
            }
            
            ')' => {
                if next_char_is(')', cursor, line) {
                    cursor.xy.0 += 1;
                } else {
                    line.insert(idx, ')');
                    cursor.xy.0 += 1;
                }
            }
        
            ']' => {
                if next_char_is(']', cursor, line) {
                    cursor.xy.0 += 1;
                } else {
                    line.insert(idx, ']');
                    cursor.xy.0 += 1;
                }
            }
        
            '}' => {
                if next_char_is('}', cursor, line) {
                    cursor.xy.0 += 1;
                } else {
                    line.insert(idx, '}');
                    cursor.xy.0 += 1;
                }
            }

            _ => {
                line.insert(idx, c);
                cursor.xy.0 += 1;
            }

        }
        
        audio.play_insert();
    }
}

/// Check the character next to the cursor, for autocomplete
/// issues
fn next_char_is(c: char, cursor: &EditorCursor, line: &str) -> bool {
    line.chars().nth(cursor.xy.0)
        .map(|ch| ch == c)
        .unwrap_or(false)
}
