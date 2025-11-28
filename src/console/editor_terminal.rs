use std::process::{Command, Stdio};
use std::path::PathBuf;

pub fn execute_terminal_command(directive_command: &str, current_dir: &Option<PathBuf>) -> (String, bool) {
    // Example directive: ":t $ echo Hello, world"
    let t_command = directive_command
        .split('$')
        .nth(1) // get the part after '$'
        .map(|s| s.trim()); // trim whitespace

    match t_command {
        Some(cmd) if !cmd.is_empty() => {
            if let Some(dir) = current_dir {
                // Use shell to execute the command so we can run anything like "ls -l", "cargo r", etc.
                #[cfg(target_os = "windows")]
                let mut shell = {
                    let mut c = Command::new("cmd");
                    c.arg("/C").arg(cmd);
                    c.current_dir(&dir);
                    c.stdout(Stdio::piped());
                    c.stderr(Stdio::piped());
                    c
                };

                #[cfg(not(target_os = "windows"))]
                let mut shell = {
                    let mut c = Command::new("sh");
                    c.arg("-c").arg(cmd);
                    c.current_dir(&dir);
                    c.stdout(Stdio::piped());
                    c.stderr(Stdio::piped());
                    c
                };

                match shell.output() {
                    Ok(output) => {
                        // Combine stdout and stderr
                        let mut result = String::new();
                        if !output.stdout.is_empty() {
                            result.push_str(&String::from_utf8_lossy(&output.stdout));
                        }
                        if !output.stderr.is_empty() {
                            if !result.is_empty() { result.push('\n'); }
                            result.push_str(&String::from_utf8_lossy(&output.stderr));
                        }
                        (result.to_string(), true)
                    }
                    Err(e) => (format!("Failed to execute command: {}", e), false),
                }
            } else {
                ("InvalidPathForCommand <:t>".to_string(), false)
            }
        }
        _ => ("NoCommandGiven <:t>".to_string(), false),
    }
}