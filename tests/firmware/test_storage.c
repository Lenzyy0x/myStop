#include "unity.h"

#include "esp_err.h"
#include "nvs.h"

#include "storage.h"

#define TEST_KEY "test_key"

void setUp(void) {
    // Wird vor jedem Test aufgerufen
    storage_erase(TEST_KEY);
}

void tearDown(void) {
    // Wird nach jedem Test aufgerufen, sodass kein Test Müll im NVS überbleibt
    storage_erase(TEST_KEY);
}

void app_main(void) {
    
    // Initialisiere den Speicher, bevor die Tests ausgeführt werden
    ESP_ERROR_CHECK(storage_init());

    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
}

TEST_CASE("storage_init erfolgreich", "[storage]") {
    TEST_ASSERT_EQUAL(ESP_OK, storage_init());
}