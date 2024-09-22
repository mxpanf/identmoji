# 🫥 Identmoji: Unique Server Identifier Generator

**Identmoji** is a tool that generates a unique identifier for your server using emojis and a code derived from system-specific data. It can output the identifier in text or JSON format and save it to a configuration file for future reference.

> [!NOTE]  
> This project was created as a learning tool to explore the architecture of command-line applications written in C and to deepen understanding of the language itself. It is not intended for serious or critical use. The generated identifiers, while fun and unique, should be considered as a playful addition to your workflow or an aesthetic enhancement rather than a reliable method for important identification purposes, especially in production environments.
>
> We encourage you to enjoy using this project for entertainment, but it is not recommended for any serious or sensitive tasks.

## Features

- Generates a unique server identifier consisting of:
  - A single emoji or a set of four emojis.
  - A five-character uppercase code.
- Options to output in text or JSON format.
- Ability to save the identifier to a file.
- Checks for existing configuration files.

## Command-Line Options

The following flags can be used to modify the behavior of the program:

| **Flag**          | **Description**                                                                            |
|-------------------|--------------------------------------------------------------------------------------------|
| `-4`              | Use 4 emojis instead of 1 in the output.                                                   |
| `--json`          | Output in JSON format.                                                                     |
| `--full`          | Output full details: single emoji (`emojis`), emoji set (`emojis_set`), and code (`code`).  |
| `-o [file]`       | Specify the output file to save the identifier.                                             |
| `--check`         | Check if the configuration file exists and display its content.                             |
| `--help`          | Display help information about the available options.                                       |

## Build Instructions

### Install Dependencies

#### On Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev
```

#### On Fedora/RHEL/CentOS

```bash
sudo yum groupinstall "Development Tools"
sudo yum install openssl-devel
```

#### On Arch Linux

```bash
sudo pacman -S base-devel openssl
```

### Build the Application

To build the application, simply run:

```bash
gcc identmoji.c -o identmoji -lssl -lcrypto
```

This will compile the source code and generate the `identmoji` executable.

## Usage

<details>
  <summary>Default Usage</summary>

By default, **identmoji** will generate an identifier with a single emoji and a five-character code.

```bash
./identmoji
```

**Example Output:**

```
Server Identifier: 💻A1B2C
```

</details>

<details>
  <summary>Using 4 Emojis</summary>

To generate an identifier with a set of four emojis, use the `-4` flag:

```bash
./identmoji -4
```

**Example Output:**

```
Server Identifier: 💻🐧💡🚀A1B2C
```

</details>

<details>
  <summary>Full Output (Text)</summary>

The `--full` flag provides a more detailed output with a single emoji, a set of four emojis, and the code:

```bash
./identmoji --full
```

**Example Output:**

```
Server Identifier: 💻
Server Identifier Set: 💻🐧💡🚀
Server Code: A1B2C
```

</details>

<details>
  <summary>Full Output (JSON)</summary>

To get the full output in JSON format, use both the `--json` and `--full` flags:

```bash
./identmoji --json --full
```

**Example Output:**

```json
{
  "emojis": "💻",
  "emojis_set": "💻🐧💡🚀",
  "code": "A1B2C"
}
```

</details>

<details>
  <summary>Saving the Identifier to a File</summary>

You can save the generated identifier to a file using the `-o` or `--output` flag:

```bash
./identmoji --json --full -o /path/to/identifier.json
```

This will save the JSON-formatted identifier to the specified file.

**Example Output:**

```
Configuration saved to /path/to/identifier.json
```

</details>

<details>
  <summary>Checking Existing Configuration</summary>

You can check if a configuration file exists and display its contents using the `--check` flag:

```bash
./identmoji --check
```

If the file exists, its contents will be displayed. If the file does not exist, you'll receive a message like:

```
No configuration found at /path/to/identifier.json
```

Also available with `-o` flag (but as an input file)

```bash
./identmoji --check -o /path/to/identifier.json
```

</details>