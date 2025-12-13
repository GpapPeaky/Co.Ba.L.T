use macroquad::color::Color;
use crate::{
    options::editor_pallete::*,
    text::{editor_input::TAB_PATTERN, editor_language_manager::EditorLanguageKeywords, editor_text_stylizer::EditorGeneralTextStylizer},
};

pub struct EditorToken {
    pub text: String,
    pub color: Color,
}

/// Tokenize text using language keywords, function detection, and stylizer calibration
pub fn tokenize_text(
    text: &Vec<String>,
    elk: &EditorLanguageKeywords,
    gts: &EditorGeneralTextStylizer,
) -> Vec<Vec<EditorToken>> {
    let mut result = Vec::with_capacity(text.len());
    let mut in_string = false;
    let mut in_block_comment = false;

    for line in text {
        let mut line_tokens = Vec::new();
        let visual_line = line.replace("\t", TAB_PATTERN);
        let mut chars = visual_line.chars().peekable();

        while let Some(&c) = chars.peek() {
            let mut token = String::new();
            let color: Color;

            if in_block_comment {
                while let Some(&ch) = chars.peek() {
                    if ch == '@' {
                        if !token.is_empty() {
                            line_tokens.push(EditorToken { text: token.clone(), color: COMMENT_COLOR });
                            token.clear();
                        }

                        chars.next();
                        token.push('@');
                        while let Some(&c) = chars.peek() {
                            if c.is_alphanumeric() || c == '_' {
                                token.push(chars.next().unwrap());
                            } else { break; }
                        }

                        line_tokens.push(EditorToken { text: token.clone(), color: MACRO_COLOR });
                        token.clear();
                        continue;
                    }

                    let ch = chars.next().unwrap();
                    token.push(ch);
                    if ch == '*' && chars.peek() == Some(&'/') {
                        token.push(chars.next().unwrap());
                        in_block_comment = false;
                        break;
                    }
                }

                color = COMMENT_COLOR;
            } else if in_string {
                while let Some(ch) = chars.next() {
                    token.push(ch);
                    if ch == '"' && !token.ends_with("\\\"") {
                        in_string = false;
                        break;
                    }
                }
                color = STRING_LITERAL_COLOR;
            } else {
                match c {
                    '/' => {
                        chars.next();
                        if chars.peek() == Some(&'/') {
                            chars.next();
                            token.push_str("//");
                            token.extend(chars.by_ref());
                            color = COMMENT_COLOR;
                        } else if chars.peek() == Some(&'*') {
                            chars.next();
                            token.push_str("/*");
                            in_block_comment = true;
                            color = COMMENT_COLOR;
                        } else { token.push('/'); color = PUNCTUATION_COLOR; }
                    }
                    '"' => {
                        chars.next();
                        token.push('"');
                        in_string = true;
                        color = STRING_LITERAL_COLOR;
                    }
                    '#' => {
                        while let Some(&ch) = chars.peek() {
                            if ch.is_whitespace() { break; }
                            token.push(chars.next().unwrap());
                        }
                        color = MACRO_COLOR;
                    }
                    c if c.is_whitespace() => {
                        while let Some(&ch) = chars.peek() {
                            if !ch.is_whitespace() { break; }
                            token.push(chars.next().unwrap());
                        }
                        color = IDENTIFIER_COLOR;
                    }
                    c if c.is_ascii_digit() => {
                        while let Some(&ch) = chars.peek() {
                            if !(ch.is_ascii_digit() || ch == '.' || ch == 'f' || ch == 'F' || ch == '-') { break; }
                            token.push(chars.next().unwrap());
                        }
                        color = NUMBER_LITERAL_COLOR;
                    }
                    c if !c.is_alphanumeric() && c != '_' => {
                        token.push(chars.next().unwrap());
                        color = PUNCTUATION_COLOR;
                    }
                    _ => {
                        while let Some(&ch) = chars.peek() {
                            if !ch.is_alphanumeric() && ch != '_' { break; }
                            token.push(chars.next().unwrap());
                        }

                        let clean = token.trim_matches(|c: char| !c.is_alphanumeric() && c != '_');

                        let mut lookahead = chars.clone().peekable();
                        while let Some(&ch) = lookahead.peek() {
                            if ch.is_whitespace() { lookahead.next(); } else { break; }
                        }

                        if lookahead.peek() == Some(&'(')
                            && !elk.control_flow.contains(&clean)
                            && !elk.type_qualifiers.contains(&clean)
                            && !elk.composite_types.contains(&clean)
                            && !elk.storage_class.contains(&clean)
                            && !elk.misc.contains(&clean)
                            && !elk.data_types.contains(&clean)
                        {
                            color = FUNCTION_COLOR;
                        } else {
                            color = gts.calibrate_string_color(clean, &elk);
                        }
                    }
                }
            }

            if !token.is_empty() {
                line_tokens.push(EditorToken { text: token, color });
            }
        }

        result.push(line_tokens);
    }

    result
}
