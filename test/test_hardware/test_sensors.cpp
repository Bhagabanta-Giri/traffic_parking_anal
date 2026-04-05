#include <Arduino.h>
#include <unity.h>
#include "Hardware.h"

void test_slot_logic(void) {
    int slots = getAvailableSlots();
    TEST_ASSERT_TRUE(slots >= 0 && slots <= 6);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_slot_logic);
    UNITY_END();
}

void loop() {}