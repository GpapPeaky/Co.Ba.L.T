pub struct EditorCamera {
    pub line_x_start: u32,
    pub line_y_start: u32
}

impl EditorCamera {
    pub fn new() -> EditorCamera {
        EditorCamera {
            line_x_start: 0,
            line_y_start: 0
        }
    }
}