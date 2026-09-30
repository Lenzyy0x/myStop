#include "storage.h"

#include "nvs.h"
#include "nvs_flash.h"

#define STORAGE_NAMESPACE "myStop"

esp_err_t storage_init(void) {
    return nvs_flash_init();
}

esp_err_t storage_set_string(const char *key, const char *value) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_str(handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    return err;
}

esp_err_t storage_get_string(const char *key, char *value, size_t value_size) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return err;
    }

    size_t required_size = value_size;

    err = nvs_get_str(handle, key, value, &required_size);

    nvs_close(handle);
    return err;
}

// Für zum Beispiel reset der WLAN Konfiguration
esp_err_t storage_erase(const char *key) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_erase_key(handle, key);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    return err;
}