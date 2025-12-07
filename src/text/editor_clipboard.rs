use std::{process::Command, time::Duration};

pub fn copy_to_clipboard(
    text: &str
) -> Result<(), Box<dyn std::error::Error>> {
    let escaped = text.replace("'", "''"); // Escape single quotes
    Command::new("powershell")
        .args(["-NoProfile", "-Command", &format!("Set-Clipboard -Value '{}'", escaped)])
        .output()?;
    Ok(())
}

pub fn paste_from_clipboard(
) -> Result<String, String> {
    // Retry logic for Windows
    for attempt in 0..3 {
        let result = Command::new("powershell")
            .args(["-NoProfile", "-Command", "Get-Clipboard"])
            .output();
        
        match result {
            Ok(output) if output.status.success() => {
                let text = String::from_utf8_lossy(&output.stdout).to_string();
                return Ok(text.trim_end().to_string());
            }
            _ if attempt < 2 => {
                std::thread::sleep(Duration::from_millis(50));
                continue;
            }
            Err(e) => return Err(format!("Clipboard error: {}", e)),
            _ => return Err("Failed to read clipboard".to_string()),
        }
    }
    
    Err("Failed to access clipboard after retries".to_string())
}