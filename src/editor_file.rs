use std::path::{Path, PathBuf};

pub struct EditorFileSystem {
    pub current_dir: Option<PathBuf>,
    pub current_file: Option<PathBuf>
}

impl EditorFileSystem {
    pub fn new() -> Self {
        EditorFileSystem {
            current_dir: None,
            current_file: None
        }
    }

    /// Change to another cwd, cd use ,
    /// returns true if the change was valid, else false
    pub fn change_current_directory(&mut self, p: String) -> bool {
        let path = Path::new(&p);

        if path.is_dir() {
            self.current_dir = Some(path.to_path_buf());

            true
        } else {
            false
        }
    }

    /// Change to another file inside the current directory
    /// by typing its name in the console
    /// returns true if the change was valid, else false
    pub fn change_current_file(&mut self, f: String) -> bool {
        if let Some(dir) = &self.current_dir {
            let file_path = dir.join(&f);
            if file_path.is_file() {
                self.current_file = Some(file_path);
                return true;
            }
        }

        false
    }
}
