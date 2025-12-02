// Editor's 'virtual' terminal/shell bridge

/// EXPERIMENTAL: Execute terminal commands, with the <:t> $ <command> directive
pub fn execute_terminal_command(directive_command: &str, current_dir: &Option<std::path::PathBuf>) -> (String, bool) {
    // Extract command after $
    let t_command = directive_command
        .split('$')
        .nth(1)
        .map(|s| s.trim());

    match t_command {
        Some(cmd) if !cmd.is_empty() => {
            if let Some(dir) = current_dir {
                #[cfg(target_os = "windows")]
                let (shell, flag) = {
                    // Prefer pwsh if installed (PowerShell 7+), fallback to Windows PowerShell
                    if which::which("pwsh").is_ok() {
                        ("pwsh", "-Command")
                    } else {
                        ("powershell", "-Command")
                    }
                };

                #[cfg(not(target_os = "windows"))]
                let (shell, flag) = ("/bin/sh", "-c");

                let output = std::process::Command::new(shell)
                    .arg(flag)
                    .arg(cmd)
                    .current_dir(dir)
                    .stdout(std::process::Stdio::piped())
                    .stderr(std::process::Stdio::piped())
                    .output();

                match output {
                    Ok(output) => {
                        let mut result = String::new();
                        if !output.stdout.is_empty() {
                            result.push_str(&String::from_utf8_lossy(&output.stdout));
                        }
                        if !output.stderr.is_empty() {
                            if !result.is_empty() { result.push('\n'); }
                            result.push_str(&String::from_utf8_lossy(&output.stderr));
                        }
                        (result, true)
                    }
                    Err(e) => (format!("Failed to execute command: {} <:t>", e), false),
                }
            } else {
                ("InvalidPathForCommand <:t>".to_string(), false)
            }
        }
        _ => ("NoCommandGiven <:t>".to_string(), false),
    }
}
