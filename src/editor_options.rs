pub struct EditorOptions {
      pub pallete: String,
      pub font: String,
      pub audio: bool,
      pub volume: u8,
      pub smart: bool,
      pub fullscreen: bool,
      pub highlight: bool,
      pub cursor_has_w: bool
};

impl EditorOptions {
      /// Constructor
      pub fn new() -> EditorOptions {
            EditorOptions {
                  pallete: "",
                  font: "",
                  audio: true,
                  volume: 50,
                  smart: true,
                  fullscreen: true,
                  highlight: true,
                  cursor_has_w: true
            }
      }
}

