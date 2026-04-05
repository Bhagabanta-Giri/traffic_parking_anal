#include <Arduino.h>
#include <unity.h>
#include "traffic_police.h"
#include "constants.h"

// This runs before every single test
void setUp(void) {
    initTrafficPolice();
}

// Test 1: Does the system start in Red (State 0)?
void test_initial_state(void) {
    TEST_ASSERT_EQUAL(0, getTrafficState());
}

// Test 2: Does Emergency Mode actually trigger?
void test_emergency_activation(void) {
    triggerEmergency();
    TEST_ASSERT_TRUE(isEmergencyActive());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_initial_state);
    RUN_TEST(test_emergency_activation);
    UNITY_END();
}

void loop() {}