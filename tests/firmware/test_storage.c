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

TEST_CASE("nichtexisten key lesen returnt not found", "[storage]") {
    char value[64];
    TEST_ASSERT_EQUAL(ESP_ERR_NVS_NOT_FOUND, storage_get_string(TEST_KEY, value, sizeof(value)));
}

TEST_CASE("string kann gespeichert werden", "[storage]") {
    const char *test_value = "Hello, World!";
    TEST_ASSERT_EQUAL(ESP_OK, storage_set_string(TEST_KEY, test_value));
}

// Dafür muss auch speichern funktionieren
TEST_CASE("String kann gelesen werden", "[storage]") {
    const char *test_value = "Hello, World!";
    storage_set_string(TEST_KEY, test_value);

    char value[64];
    TEST_ASSERT_EQUAL(ESP_OK, storage_get_string(TEST_KEY, value, sizeof(value)));
    TEST_ASSERT_EQUAL_STRING(test_value, value);
}

// Dafür muss speichern und lesen funktionieren
TEST_CASE("String kann überschrieben werden", "[storage]") {
    const char *test_value1 = "Hello, World!";
    const char *test_value2 = "Tschüss, World!";
    storage_set_string(TEST_KEY, test_value1);
    storage_set_string(TEST_KEY, test_value2);

    char value[64];
    TEST_ASSERT_EQUAL(ESP_OK, storage_get_string(TEST_KEY, value, sizeof(value)));
    TEST_ASSERT_EQUAL_STRING(test_value2, value);
}

TEST_CASE("String kann gelöscht werden", "[storage]") {
    const char *test_value = "Hello, World!";
    storage_set_string(TEST_KEY, test_value);
    storage_erase(TEST_KEY);

    char value[64];
    TEST_ASSERT_EQUAL(ESP_ERR_NVS_NOT_FOUND, storage_get_string(TEST_KEY, value, sizeof(value)));
}

TEST_CASE("Löschen eines nicht existierenden Keys returnt not found", "[storage]") {
    TEST_ASSERT_EQUAL(ESP_ERR_NVS_NOT_FOUND, storage_erase(TEST_KEY));
}