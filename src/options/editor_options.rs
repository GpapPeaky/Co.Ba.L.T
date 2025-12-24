// Editor options structure and constructor

use macroquad::prelude::*;

pub struct EditorOptions {
      pub pallete: String,
      pub t_font: String, // text font
      pub c_font: String, // console font
      pub audio: bool,
      pub smart: bool,
      pub fullscreen: bool,
      pub highlight: bool,
}

impl EditorOptions {
      /// Constructor
      pub fn new() -> EditorOptions {
            EditorOptions {
                  pallete: "".to_string(),
                  t_font: "".to_string(),
                  c_font: "".to_string(),
                  audio: true,
                  smart: true,
                  fullscreen: true,
                  highlight: true,
            }
      }

      /// Switch pallete
      pub fn switch_pallete(
        &mut self,
        pallete_name: &str
      ) {
        self.pallete = pallete_name.to_string();
      }
      
      /// Switch text font
      pub fn switch_text_font(
        &mut self,
        font_name: &str
      ) {
        self.t_font = font_name.to_string();
      }

      /// Switch console font
      pub fn switch_console_font(
        &mut self,
        font_name: &str
      ) {
        self.c_font = font_name.to_string();
      }
      
      /// Toggle typing sounds on and off
      pub fn toggle_audio(
          &mut self
      ) {
          self.audio = !self.audio;
      }
      
      /// Toggle on and off smart identation
      pub fn toggle_smart(
          &mut self
      ) {
          self.smart = !self.smart;
      }
     
      /// Toggle on and off fullscreen
      pub fn toggle_fullscreen(
        &mut self
      ) {
         self.fullscreen = !self.fullscreen;
         
         // Actual toggle.
         set_fullscreen(self.fullscreen);
      }
      
      /// Toggle on and off highlighting
      pub fn toggle_highlight(
          &mut self
      ) {
          self.highlight = !self.highlight;
      }
}
