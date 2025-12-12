// Editor options structure and constructor

use macroquad::prelude::*;

pub struct EditorOptions {
      pub pallete: String,
      pub font: String,
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
                  font: "".to_string(),
                  audio: true,
                  smart: true,
                  fullscreen: true,
                  highlight: true,
            }
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
