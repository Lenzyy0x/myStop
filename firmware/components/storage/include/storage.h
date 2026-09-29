#pragma once // Header Datei wird nur einmal eingebunden

#include <stdbool.h>
#include <stddef.h>

#include "esp_err.h"

esp_err_t storage_init(void);

esp_err_t storage_get_string(const char *key, char *value, size_t value_size);

esp_err_t storage_set_string(const char *key, const char *value);

esp_err_t storage_erase(const char *key);