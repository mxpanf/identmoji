#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>  // For cryptographic functions using EVP API
#include <sys/stat.h>     // For file and directory status
#include <ctype.h>        // For character operations

// Define the default configuration file path
#define DEFAULT_CONFIG_FILE "~/.config/identmoji.json"

// Emoji list to select from
const char *EMOJI_LIST[] = {
    "🤖", "🦷", "🎃", "🐳", "💼", "🧊", "🍪", "🦀", "💡", "🌹",
    "👄", "👾", "🪼", "😺", "🍗", "🌌", "🌳", "✈️", "💅", "🎯",
    "🦉", "🫀", "👻", "🎓", "👀", "🌵", "🦜", "🌚", "💾", "☄️",
    "👑", "🐭", "🧶", "😈", "🦾", "🐼", "☁️", "💿", "⚙️", "🍁",
    "🎲", "🐣", "🐸", "🌋", "👽", "🧠", "🐨", "🔌", "🚗", "⛩",
    "🪨", "🍒", "💻", "🗿", "🍾", "🪵", "🐧", "🖥️", "🌊", "🔥",
    "🚀", "🍏", "📂", "⚡️", "⭐️", "💣", "🔒", "🎱", "📀", "🌈",
    "🛠️", "🏆", "🐍", "📜", "🍹", "📡", "🖨️", "📊", "🎮", "🎧"
};
int EMOJI_LIST_SIZE = sizeof(EMOJI_LIST) / sizeof(EMOJI_LIST[0]);

/**
 * Generates an MD5 hash of the input string using the EVP API.
 * 
 * @param input The input string to hash.
 * @param output The output buffer where the hash will be stored.
 */
void md5_hash(const char *input, unsigned char *output) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();  // Create a new digest context
    if (ctx == NULL) {
        fprintf(stderr, "Error: Unable to create MD context\n");
        exit(1);
    }

    const EVP_MD *md = EVP_md5();  // Specify MD5 as the hashing algorithm
    if (EVP_DigestInit_ex(ctx, md, NULL) != 1 ||
        EVP_DigestUpdate(ctx, input, strlen(input)) != 1 ||
        EVP_DigestFinal_ex(ctx, output, NULL) != 1) {
        fprintf(stderr, "Error: MD5 hashing failed\n");
        EVP_MD_CTX_free(ctx);
        exit(1);
    }

    EVP_MD_CTX_free(ctx);  // Free the digest context
}

/**
 * Converts MD5 hash bytes to a hexadecimal string.
 * 
 * @param hash The input hash bytes.
 * @param output The output buffer where the hex string will be stored.
 */
void md5_to_string(unsigned char *hash, char *output) {
    for (int i = 0; i < 16; ++i) {
        sprintf(output + (i * 2), "%02x", hash[i]);  // Convert each byte to hex
    }
    output[32] = '\0';  // Null-terminate the string
}

/**
 * Retrieves unique system data to be used for generating the identifier.
 * This includes UUID, operating system name, CPU model name, CPU clock speed, and disk serial number.
 * 
 * @param output The output buffer where the combined system data will be stored.
 */
void get_system_data(char *output) {
    FILE *fp;
    char uuid[256] = "no-uuid";
    char os_name[256] = "no-os";
    char cpu_model[256] = "no-cpu-model";
    char cpu_mhz[256] = "no-cpu-mhz";
    char disk_serial[256] = "no-disk-serial";

    // Get UUID from system
    fp = fopen("/sys/class/dmi/id/product_uuid", "r");
    if (fp != NULL) {
        if (fscanf(fp, "%255s", uuid) != 1) {
            fprintf(stderr, "Warning: Could not read UUID\n");
        }
        fclose(fp);
    } else {
        fprintf(stderr, "Error: Unable to open /sys/class/dmi/id/product_uuid\n");
    }

    // Get OS name (without version)
    fp = fopen("/etc/os-release", "r");
    if (fp != NULL) {
        while (fgets(os_name, sizeof(os_name), fp)) {
            if (strncmp(os_name, "NAME=", 5) == 0) {
                // Extract OS name (e.g., NAME="Ubuntu")
                sscanf(os_name, "NAME=\"%255[^\"]\"", os_name);
                break;
            }
        }
        fclose(fp);
    } else {
        fprintf(stderr, "Error: Unable to read /etc/os-release for OS name\n");
    }

    // Get CPU model name
    fp = popen("grep -m1 'model name' /proc/cpuinfo | awk -F: '{print $2}'", "r");
    if (fp != NULL) {
        if (fgets(cpu_model, sizeof(cpu_model), fp) == NULL) {
            fprintf(stderr, "Warning: Could not read CPU model\n");
        }
        pclose(fp);
    } else {
        fprintf(stderr, "Error: Unable to read CPU model from /proc/cpuinfo\n");
    }

    // Get CPU clock speed (cpu MHz)
    fp = popen("grep -m1 'cpu MHz' /proc/cpuinfo | awk -F: '{print $2}'", "r");
    if (fp != NULL) {
        if (fgets(cpu_mhz, sizeof(cpu_mhz), fp) == NULL) {
            fprintf(stderr, "Warning: Could not read CPU clock speed\n");
        }
        pclose(fp);
    } else {
        fprintf(stderr, "Error: Unable to read CPU clock speed from /proc/cpuinfo\n");
    }

    // Get disk serial number
    fp = popen("udevadm info --query=all --name=/dev/sda | grep 'ID_SERIAL=' | awk -F= '{print $2}'", "r");
    if (fp != NULL) {
        if (fscanf(fp, "%255s", disk_serial) != 1) {
            fprintf(stderr, "Warning: Could not read disk serial\n");
        }
        pclose(fp);
    } else {
        fprintf(stderr, "Error: Unable to read disk serial from /dev/sda\n");
    }

    // Combine all system data into one string
    snprintf(output, 1024, "%s%s%s%s%s", uuid, os_name, cpu_model, cpu_mhz, disk_serial);
}

/**
 * Selects emojis based on the hash value.
 * 
 * @param hash The hash string.
 * @param num_emojis The number of emojis to select.
 * @param output The output buffer where the selected emojis will be stored.
 */
void select_emojis(const char *hash, int num_emojis, char *output) {
    output[0] = '\0';  // Initialize output string
    for (int i = 0; i < num_emojis; ++i) {
        char byte_str[5];  // 4 characters + null terminator
        strncpy(byte_str, hash + (i * 4), 4);
        byte_str[4] = '\0';
        int byte = (int)strtol(byte_str, NULL, 16);  // Convert hex to int
        int index = byte % EMOJI_LIST_SIZE;          // Get index in emoji list
        strcat(output, EMOJI_LIST[index]);           // Append emoji to output
    }
}

/**
 * Generates a five-character uppercase code from the hash.
 * 
 * @param hash The hash string.
 * @param code The output buffer where the code will be stored.
 */
void generate_code(const char *hash, char *code) {
    strncpy(code, hash + 10, 5);  // Get 5 characters starting from position 10
    code[5] = '\0';
    for (int i = 0; i < 5; i++) {
        code[i] = toupper((unsigned char)code[i]);  // Convert to uppercase
    }
}

/**
 * Checks if a file exists.
 * 
 * @param filename The path to the file.
 * @return 1 if the file exists, 0 otherwise.
 */
int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

/**
 * Writes the generated data to a file.
 * 
 * @param filename The output file path.
 * @param emoji The single emoji.
 * @param emoji_set The set of four emojis.
 * @param code The code.
 * @param output_json Flag indicating if output is in JSON format.
 * @param full_output Flag indicating if full output is requested.
 * @return 1 if successful, 0 if failed.
 */
int write_to_file(const char *filename, const char *emoji, const char *emoji_set, const char *code, int output_json, int full_output) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not write to %s\n", filename);
        return 0;  // Return 0 on failure
    }

    // Write in JSON or plain text format based on flags
    if (output_json) {
        if (full_output) {
            fprintf(fp, "{\n  \"emojis\": \"%s\",\n  \"emojis_set\": \"%s\",\n  \"code\": \"%s\"\n}\n", emoji, emoji_set, code);
        } else {
            fprintf(fp, "{\n  \"emojis\": \"%s\",\n  \"code\": \"%s\"\n}\n", emoji, code);
        }
    } else {
        if (full_output) {
            fprintf(fp, "Server Identifier: %s\n", emoji);
            fprintf(fp, "Server Identifier Set: %s\n", emoji_set);
            fprintf(fp, "Server Code: %s\n", code);
        } else {
            fprintf(fp, "Server Identifier: %s%s\n", emoji, code);
        }
    }

    fclose(fp);
    return 1;  // Return 1 on success
}

/**
 * Prints the help message with usage instructions.
 */
void print_help() {
    printf("Usage: identmoji [options]\n");
    printf("Options:\n");
    printf("  -4              Use 4 emojis instead of 1\n");
    printf("  --json          Output in JSON format\n");
    printf("  --full          Output full details (emoji, emojis_set, code)\n");
    printf("  -o, --output    Specify output file path\n");
    printf("  --check         Check if configuration already exists and display its content\n");
    printf("  --help          Display this help message\n");
}

/**
 * Main function to generate the server identifier.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status.
 */
int main(int argc, char *argv[]) {
    int num_emojis = 1;     // Default is 1 emoji
    int output_json = 0;    // Default is text output
    int full_output = 0;    // Default is not full output
    int check_file = 0;     // Check for existing configuration
    char *output_file = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-4") == 0) {
            num_emojis = 4;
        } else if (strcmp(argv[i], "--json") == 0) {
            output_json = 1;
        } else if (strcmp(argv[i], "--full") == 0) {
            full_output = 1;
        } else if ((strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--check") == 0) {
            check_file = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help();
            return 1;
        }
    }

    // Check if configuration file exists
    if (check_file) {
        const char *config_file = output_file ? output_file : DEFAULT_CONFIG_FILE;
        if (file_exists(config_file)) {
            // Print the content of the file
            FILE *fp = fopen(config_file, "r");
            if (fp != NULL) {
                char line[256];
                while (fgets(line, sizeof(line), fp)) {
                    printf("%s", line);
                }
                fclose(fp);
            } else {
                fprintf(stderr, "Error: Could not read from %s\n", config_file);
            }
        } else {
            printf("No configuration found at %s\n", config_file);
        }
        return 0;
    }

    // Get system data and compute MD5 hash
    char system_data[1024];
    get_system_data(system_data);

    unsigned char hash[EVP_MAX_MD_SIZE];
    char hash_string[EVP_MAX_MD_SIZE * 2 + 1];
    md5_hash(system_data, hash);
    md5_to_string(hash, hash_string);

    // Generate emojis and code
    char emoji[64];
    char emoji_set[256];
    select_emojis(hash_string, 1, emoji);       // Single emoji for 'emojis'
    select_emojis(hash_string, 4, emoji_set);   // Four emojis for 'emojis_set'

    char code[6];
    generate_code(hash_string, code);

    // Output based on flags
    if (output_json) {
        if (full_output) {
            // Full JSON output
            printf("{\n  \"emojis\": \"%s\",\n  \"emojis_set\": \"%s\",\n  \"code\": \"%s\"\n}\n", emoji, emoji_set, code);
        } else {
            // JSON output with emojis and code
            if (num_emojis == 4) {
                printf("{\n  \"emojis\": \"%s\",\n  \"code\": \"%s\"\n}\n", emoji_set, code);
            } else {
                printf("{\n  \"emojis\": \"%s\",\n  \"code\": \"%s\"\n}\n", emoji, code);
            }
        }
    } else {
        if (full_output) {
            // Full text output
            printf("Server Identifier: %s\n", emoji);
            printf("Server Identifier Set: %s\n", emoji_set);
            printf("Server Code: %s\n", code);
        } else {
            // Standard text output
            if (num_emojis == 4) {
                printf("Server Identifier: %s%s\n", emoji_set, code);
            } else {
                printf("Server Identifier: %s%s\n", emoji, code);
            }
        }
    }

    // Save to file if requested
    if (output_file != NULL) {
        if (write_to_file(output_file, emoji, emoji_set, code, output_json, full_output)) {
            printf("Configuration saved to %s\n", output_file);
        }
    }

    return 0;
}
