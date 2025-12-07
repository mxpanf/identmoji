# Project Blueprint: Identmoji v2 (Linux Edition)

## 1. Executive Summary

**Identmoji v2** is a system-wide Linux utility designed to generate a deterministic, visual cryptographic fingerprint (emoji-based) for server identification. Unlike v1, this version focuses on production environments, utilizing stable Linux Hardware IDs (HWID) and offering seamless shell/MOTD integration.

**Target Stack:** Go (Golang).
*Reasoning: Static binaries, excellent Linux syscall support, high performance.*

---

## 2. Core Logic & Algorithm

### 2.1. Entropy Source (The Input)

To ensure the identifier remains constant across reboots and network changes, strictly prioritize the inputs in this order:

1.  **Manual Override (Highest Priority):**
    * Flag: `--seed "production-db-01"`
    * Env Var: `IDENTMOJI_SEED="production-db-01"`
    * *Use case: Docker containers or ephemeral VMs where HWID changes but identity shouldn't.*

2.  **System ID (Primary Automatic Source):**
    * **Primary:** Read `/etc/machine-id` (Systemd standard).
    * **Fallback:** Read `/var/lib/dbus/machine-id`.
    * *Use case: Bare metal servers, VPS, long-lived VMs.*

3.  **Hostname (Discouraged Fallback):**
    * Only used if forced via config or if machine-id is totally absent/empty.

### 2.2. Hashing & Mapping

We need a cryptographically secure, deterministic output.

1.  **Normalization:** Input string is trimmed and converted to lowercase.
2.  **Hashing:** Apply `SHA-256` to the input.
3.  **Chunking:** Split the hash into 4-byte integers.
4.  **Modulo Mapping:** `Integer % Size_Of_Emoji_Palette` = Index of Emoji.
5.  **Formatting:** Assemble the selected emojis into the final string.

---

## 3. File System Hierarchy (FHS Compliance)

Since this is a system utility available to all users, we adhere to Linux FHS:

* **Binary:**
    * `/usr/local/bin/identmoji` (The executable).
* **Configuration:**
    * `/etc/identmoji/config.toml` (Global configuration).
    * *Rationale: Standard location for system-wide configs.*
* **State / Cache (Optional):**
    * `/var/lib/identmoji/fingerprint`
    * *Rationale: If the user wants to "freeze" the generated ID to a file to prevent calculation overhead.*

---

## 4. Configuration Schema (`/etc/identmoji/config.toml`)

```toml
[general]
# Logic: "hwid" (default) or "seed"
strategy = "hwid"

# If strategy is "seed", this value is used.
# Can be overridden by environment variable IDENTMOJI_SEED
manual_seed = ""

# Output format: "text", "json", "export" (shell vars)
format = "text"

[visual]
# Length of the identifier
chain_length = 3

# Palettes to include (animals, food, tech, geometric)
# We exclude flags by default to avoid rendering issues.
categories = ["animals", "geometric"]

# Spacer between emojis (space, hyphen, or empty)
separator = " "

[integration]
# Add a label before the emojis? e.g. "Server ID: 🦁"
prefix = "ID: "
```

-----

## 5\. Integration Strategy

### 5.1. MOTD (Message of the Day)

To display the Identmoji immediately upon SSH login.

**For Debian/Ubuntu (`update-motd`):**
Create an executable script at `/etc/update-motd.d/05-identmoji`:

```bash
#!/bin/sh
echo
/usr/local/bin/identmoji --config /etc/identmoji/config.toml
echo
```

**For RHEL/CentOS/Generic:**
Append the execution line to `/etc/profile.d/identmoji.sh`:

```bash
# /etc/profile.d/identmoji.sh
if [ -n "$PS1" ]; then
    /usr/local/bin/identmoji
fi
```

### 5.2. Shell Prompt Integration

Users can add the identifier to their `PS1`.

```bash
# In .bashrc or .zshrc
export SERVER_EMOJI=$(identmoji --short)
PS1="$SERVER_EMOJI $PS1"
```
