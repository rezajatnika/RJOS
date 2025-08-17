#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void config_init(config_t *config) {
    config->entries     = NULL;
    config->num_entries = 0;
    pthread_mutex_init(&config->mutex, NULL);
}

int config_load(config_t *config, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("config_load: fopen");
        return -1;
    }

    char line[CONFIG_MAX_KEY_LEN + CONFIG_MAX_VAL_LEN + 2];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        char *equal_sign = strchr(line, '=');
        if (!equal_sign) {
            fprintf(stderr, "config_load: invalid line: %s\n", line);
            continue;
        }
        *equal_sign = '\0';
        const char *key = line;
        const char *val = equal_sign + 1;

        char *newline = strchr(val, '\n');
        if (newline) {
            *newline = '\0';
        }
        config_entry_t *temp = realloc(config->entries, (config->num_entries + 1) * sizeof(config_entry_t));
        if (!temp) {
            fclose(fp);
            perror("config_load: realloc");
            return -1;
        }
        config->entries = temp;

        if (strlen(key) >= CONFIG_MAX_KEY_LEN) {
            perror("config_load: key too long");
        }
        if (strlen(val) >= CONFIG_MAX_VAL_LEN) {
            perror("config_load: value too long");
        }

        /* Add the key-value pair to the configuration. */
        strncpy(config->entries[config->num_entries].key, key, CONFIG_MAX_KEY_LEN - 1);
        strncpy(config->entries[config->num_entries].val, val, CONFIG_MAX_VAL_LEN - 1);
        config->entries[config->num_entries].key[CONFIG_MAX_KEY_LEN - 1] = '\0';
        config->entries[config->num_entries].val[CONFIG_MAX_VAL_LEN - 1] = '\0';
        config->num_entries++;
    }
    fclose(fp);
    return 0;
}

const char *config_get(config_t *config, const char *key) {
    pthread_mutex_lock(&config->mutex);
    for (size_t i = 0; i < config->num_entries; ++i) {
        if (strcmp(config->entries[i].key, key) == 0) {
            pthread_mutex_unlock(&config->mutex);
            return config->entries[i].val;
        }
    }
    pthread_mutex_unlock(&config->mutex);
    return NULL; /* Key not found. */
}

void config_destroy(config_t *config) {
    free(config->entries);
    pthread_mutex_destroy(&config->mutex);
    config->entries = NULL;
    config->num_entries = 0;
}