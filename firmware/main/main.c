#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "nvs.h"

#include "storage.h"

static const char *TAG = "main";

void app_main(void) {
    ESP_ERROR_CHECK(storage_init());

    char value[64];

    esp_err_t err = storage_get_string("test_value", value, sizeof(value));

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "test_value existiert nicht");

        ESP_ERROR_CHECK(storage_set_string("test_value", "Hallo Welt!"));

        ESP_LOGI(TAG, "Test Value wurde gespeichert");
    } else if (err == ESP_OK) {
        ESP_LOGI(TAG, "test_value: %s", value);
    } else {
        ESP_ERROR_CHECK(err);
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
