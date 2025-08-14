#ifndef RJOS_CONFIG_H
#define RJOS_CONFIG_H

#include <stddef.h>

#define CONFIG_MAX_KEY_LEN 128
#define CONFIG_MAX_VAL_LEN 256

/**
 * @typedef config_entry_t
 *
 * @brief Represents a single key-value pair in a configuration.
 *
 * This structure is used to store a configuration entry, consisting of a key
 * and its associated value. Both the key and the value are stored as character
 * arrays, with their maximum lengths defined by CONFIG_MAX_KEY_LEN and
 * CONFIG_MAX_VAL_LEN, respectively.
 */
typedef struct config_entry {
    char key[CONFIG_MAX_KEY_LEN];
    char val[CONFIG_MAX_VAL_LEN];
} config_entry_t;

/**
 * @typedef config_t
 *
 * @brief Represents a configuration consisting of multiple key-value entries.
 *
 * This structure is designed to hold an array of configuration entries, where
 * each entry comprises a key and a value. The number of entries in the
 * configuration is specified by the `num_entries` field.
 */
typedef struct config {
    config_entry_t *entries;
    size_t          num_entries;
} config_t;

/**
 * @brief Initializes a configuration structure.
 *
 * This function initializes a provided configuration structure, setting up its
 * internal state so that it is ready for use. The `config` parameter must point
 * to a valid `config_t` structure. After initialization, the structure will
 * contain no entries, and its memory will be properly prepared for further
 * operations.
 *
 * @param config Pointer to a `config_t` structure to be initialized.
 */
void config_init(config_t *config);


/**
 * @brief Loads configuration key-value pairs from a file into a config object.
 *
 * This function reads the specified file line-by-line, parsing each line into a key-value
 * pair, and adds these pairs to the provided configuration structure. Lines starting with
 * '#' or empty lines are ignored. Each line should follow the "key=value" format.
 *
 * If any line does not conform to the expected format or any error occurs during file
 * operations, the function handles such cases appropriately.
 *
 * @param config Pointer to the configuration object where the key-value pairs will be stored.
 *        The caller must ensure the configuration is properly initialized before calling.
 * @param filename Path to the configuration file to be loaded.
 *
 * @return 0 if the configuration is loaded successfully, or -1 if an error occurs (e.g., a file
 *         could not be opened or memory allocation fails).
 */
int config_load(config_t *config, const char *filename);

/**
 * @brief Retrieves the value associated with a specified key from the configuration.
 *
 * This function searches through the configuration entries to find a matching key
 * and returns its associated value. If the key is not found in the configuration,
 * or if either the config or key parameter is NULL, the function returns NULL.
 *
 * @param config Pointer to the configuration structure to search in.
 * @param key The key whose value is to be retrieved.
 * @return The value associated with the key if found, NULL otherwise.
 */
const char *config_get(const config_t *config, const char *key);

/**
 * @brief Frees the resources associated with a configuration object.
 *
 * This function releases the memory allocated for the configuration entries
 * and resets the configuration object to an empty state.
 *
 * @param config Pointer to the configuration object to be destroyed.
 */
void config_destroy(config_t *config);

#endif