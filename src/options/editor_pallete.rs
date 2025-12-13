use macroquad::prelude::*;

// Helper: convert 16-bit RGB565 to Color
const fn rgb565_to_color(rgb: u16) -> Color {
    let r = ((rgb >> 11) & 0x1F) as f32 / 31.0;
    let g = ((rgb >> 5) & 0x3F) as f32 / 63.0;
    let b = (rgb & 0x1F) as f32 / 31.0;
    Color::new(r, g, b, 1.0)
}

// ------------------- Console -------------------
pub const CONSOLE_CONTAINER_COLOR: Color    = rgb565_to_color(0x0000); // black
pub const CONSOLE_CURSOR_COLOR: Color       = rgb565_to_color(0xFFFF); // white
pub const CONSOLE_TEXT_COLOR: Color         = rgb565_to_color(0xF81F); // magenta
pub const CONSOLE_FRAME_COLOR: Color        = rgb565_to_color(0x7BEF); // gray
pub const SELECTED_FILE_COLOR: Color        = rgb565_to_color(0x07E0); // green
pub const FOLDER_COLOR: Color               = rgb565_to_color(0x001F); // blue
pub const FILE_COLOR: Color                 = rgb565_to_color(0xFFE0); // yellow

// ------------------- Text Editor -------------------
pub const BACKGROUND_COLOR: Color           = rgb565_to_color(0x0000); // black
pub const COMPOSITE_TYPE_COLOR: Color       = rgb565_to_color(0xFFE0); // yellow
pub const STORAGE_CLASS_COLOR: Color        = rgb565_to_color(0xF800); // red
pub const MISC_COLOR: Color                 = rgb565_to_color(0xFD20); // orange
pub const TYPE_QUALIFIER_COLOR: Color       = rgb565_to_color(0x07E0); // green
pub const CONTROL_FLOW_COLOR: Color         = rgb565_to_color(0xFC10); // pink/red
pub const PUNCTUATION_COLOR: Color          = rgb565_to_color(0xFFFF); // white
pub const DATA_TYPE_COLOR: Color            = rgb565_to_color(0x07FF); // cyan
pub const NUMBER_LITERAL_COLOR: Color       = rgb565_to_color(0xF81F); // magenta
pub const STRING_LITERAL_COLOR: Color       = rgb565_to_color(0x07E0); // green
pub const CURSOR_COLOR: Color               = rgb565_to_color(0xFFE0); // yellow
pub const MACRO_COLOR: Color                = rgb565_to_color(0xF81F); // magenta
pub const COMMENT_COLOR: Color              = rgb565_to_color(0x001F); // blue
pub const IDENTIFIER_COLOR: Color           = rgb565_to_color(0xFFFF); // white
pub const FUNCTION_COLOR: Color             = rgb565_to_color(0xFFE0); // yellow
