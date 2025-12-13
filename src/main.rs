// Hide the scary console.
// Do not hide it, because appearantly it moves faster??
// #![windows_subsystem = "windows"]

mod console;
mod text;
mod camera;
mod audio;
mod options;
mod win;

use std::vec;
use macroquad::prelude::*;

use crate::audio::editor_audio::EditorAudio;
use crate::camera::editor_camera::EditorCamera;
use crate::console::editor_console::{EditorConsole, console_message};
use crate::console::editor_file_system::{EditorFileSystem, draw_dir_contents, path_buffer_file_to_string, path_buffer_to_string};
use crate::options::editor_options::EditorOptions;
use crate::options::editor_pallete::{BACKGROUND_COLOR, COMPOSITE_TYPE_COLOR, CONSOLE_TEXT_COLOR, FILE_COLOR, FOLDER_COLOR, PUNCTUATION_COLOR};
use crate::text::editor_cursor::{CURSOR_WORD_OFFSET, EditorCursor};
use crate::text::editor_input::record_keyboard_to_file_text;
use crate::text::editor_language_manager::{EditorLanguageKeywords ,load_keywords_for_extension};
use crate::text::editor_text::{CURRENT_FILE_TOP_BAR_OFFSET, MODE_FONT_SIZE, MODE_Y_MARGIN, MODE_Y_OFFSET, draw};
use crate::text::editor_text_stylizer::EditorGeneralTextStylizer;
use crate::text::editor_token::tokenize_text;
use crate::win::editor_win_config::window_conf;

// TODO: Add Ctrl + z to undo last change.
// TODO: Add the palletes.
// TODO: Add more fonts.
// TODO: Aquire a windows publisher certificate to hide the scary publisher pop-up
// TODO: Add a few more directives for easier use
// TODO: Fix indentation and TAB navigation <1.5.5>
// TODO: Complete the editor options to include sound settings, cursor animation toggles and values, background images via shader and maybe keybind settings <1.5.6>
// TODO: Update docs
// TODO: Fix some issues regarding powershell and the shell bridge
// TODO: Improve directory and file navigation <1.5.7>
// TODO: Multiple open file support with "window splitting" per file <1.6.0>
// TODO: Find a suitable keybind for faster infile navigation
// TODO: Add some more keybinds
// TODO: Improve docs and codebase <1.6.1..1.6.X>
// TODO: More language support
// TODO: Tokenizer for autocompleting keywords
// TODO: Autocomplete for user defined identifiers and functions? <1.7.0>

// IDEA: Add a list of user defined functions to make it easier to traverse files. V++
// IDEA: Add a list of user defined identifiers that will pop up as an autocomplete thing.
// IDEA: Add a cmd/terminal wrapper maybe, for compiling/executing code and git commands.
// IDEA: Add file markings in specific file indeces for faster traversal <:mark>.
// IDEA: Add file markings finder <:spot>, moves by one in each directive return, won't clear inside the console so the user can keep moving.
// IDEA: Add file markings finder <:spot N>, moves to the N-th marked spot inside the file. 
// IDEA: Add multiple open files support

pub const VERSION: &str = "CoBaLT-v01.05.04";

#[macroquad::main(window_conf())]
async fn main() {
    // Editor options
    let mut ops = EditorOptions::new();    
    // Editor camera
    let mut ec = EditorCamera::new();
    // File system
    let mut efs = EditorFileSystem::new();
    // Editor audio
    let mut audio = EditorAudio::new();
    // Editor general text stylizer for file
    let mut file_gts = EditorGeneralTextStylizer::new().await;
    // Editor general text stylizer for console, font already preloaded.
    let mut console_gts = EditorGeneralTextStylizer::new().await;
    console_gts.color = CONSOLE_TEXT_COLOR;
    console_gts.font_size = 30;
    // Editor Cursor
    let mut file_cursor = EditorCursor::new();
    // Console
    let mut console = EditorConsole::new();
    // Actual file text, to change stuff around
    let mut file_text = vec![];

    // File tokens, used for rendering and colouring
    let mut file_tokens = vec![];
    // Language support based on file, default no higlighting
    let mut elk: EditorLanguageKeywords = load_keywords_for_extension("txt"); 

    let insert_word_w = measure_text("INSERT MODE", Some(&console_gts.font), MODE_FONT_SIZE as u16, 1.0).width;
    let select_word_w = measure_text("SELECTION MODE", Some(&console_gts.font), MODE_FONT_SIZE as u16, 1.0).width;
    let console_word_w = measure_text("CONSOLE MODE", Some(&console_gts.font), MODE_FONT_SIZE as u16, 1.0).width;

    loop {
        clear_background(BACKGROUND_COLOR);

        draw(&mut file_tokens, &mut file_cursor, &mut file_gts, &console, &mut ec);

        if !console.mode {
            record_keyboard_to_file_text(&mut file_cursor, &mut file_text, &mut file_tokens, &mut audio, &mut console,  &mut file_gts, &mut efs, &mut ops, &mut elk);

            let mut fname = path_buffer_file_to_string(&efs.current_file);
            if efs.unsaved_changes {
                fname = format!("*{}", path_buffer_file_to_string(&efs.current_file));
            }

            console_gts.color = COMPOSITE_TYPE_COLOR;
            
            if file_cursor.select_mode {
                console_gts.draw("SELECTION MODE", MODE_Y_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN - 15.0);
                console_gts.color = FOLDER_COLOR;
                console_gts.draw(&path_buffer_to_string(&efs.current_dir), select_word_w + 25.0, MODE_FONT_SIZE + MODE_Y_MARGIN - 15.0);
                console_gts.color = FILE_COLOR;
                console_gts.draw(&fname, select_word_w + CURRENT_FILE_TOP_BAR_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN + 15.0);
                console_gts.color = BLUE;
                console_gts.draw(&file_cursor.word, select_word_w + CURRENT_FILE_TOP_BAR_OFFSET + CURSOR_WORD_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN + 15.0);
            } else {
                console_gts.draw("INSERT MODE", MODE_Y_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN - 15.0);
                console_gts.color = FOLDER_COLOR;
                console_gts.draw(&path_buffer_to_string(&efs.current_dir), insert_word_w + 25.0, MODE_FONT_SIZE + MODE_Y_MARGIN - 15.0);
                console_gts.color = FILE_COLOR;
                console_gts.draw(&fname, insert_word_w + CURRENT_FILE_TOP_BAR_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN + 15.0);
                console_gts.color = BLUE;
                console_gts.draw(&file_cursor.word, insert_word_w + CURRENT_FILE_TOP_BAR_OFFSET + CURSOR_WORD_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN + 15.0);
            }
            
                
        } else {
            console.record_keyboard_to_console_text(&mut audio, &mut efs, &mut file_text, &mut file_cursor, &mut ops, &mut elk, &mut file_tokens, &mut file_gts);
            
            let mut fname = path_buffer_file_to_string(&efs.current_file);
            if efs.unsaved_changes {
                fname = format!("*{}", path_buffer_file_to_string(&efs.current_file));
            }
            
            console_gts.color = FILE_COLOR;
            console_gts.draw(&fname, console_word_w + CURRENT_FILE_TOP_BAR_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN - 15.0);
            console_gts.color = COMPOSITE_TYPE_COLOR;
            console_gts.draw("CONSOLE MODE", MODE_Y_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN - 15.0);
            console_gts.color = FOLDER_COLOR;
            console_gts.draw(&path_buffer_to_string(&efs.current_dir), MODE_Y_OFFSET, MODE_FONT_SIZE + MODE_Y_MARGIN + 15.0);
        }

        if console.mode {
            console_gts.color = PUNCTUATION_COLOR;
            console.draw(&console_gts);
        
            let is_cd = console.directive.starts_with(":cd ");
            let auto = draw_dir_contents(
                &efs.current_file,
                &efs.current_dir,
                &console.directive,
                &console,
                is_cd
            );
        
            if auto != "" {
                if is_cd {
                    console.directive = format!(":cd {}", auto);
                } else {
                    console.directive = auto;
                }
                console.cursor.x = console.directive.len();
            }
        }

        // Show message
        if console.showing_message && !console.message.is_empty() {
            console_message(&console.message, console.showing_manual);
        }

        // Nullify message
        if is_key_pressed(KeyCode::Escape) {
            console.showing_message = false;
            console.showing_manual = false;
            console.message.clear();
        }

        // Update resize animation
        console.animate_width();

        if efs.unsaved_changes { // Re-tokenize whenever needed
            file_tokens = tokenize_text(&file_text, &elk, &file_gts);
        }

        muse_next_frame().await;
    }
}
