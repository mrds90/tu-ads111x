#include "unity.h"
#include "ads111x_driver.h"
#include "mock_ads111x_driver_private.h"

// #include "ads111x_driver_private.h"

static ads111x_obj_t ads111x_obj;

void dummy_i2c_init(void) {
    printf("dummy_i2c_init\n");
}

void dummy_i2c_write(uint8_t name1, uint8_t name2, uint16_t name3) {
    printf("dummy_i2c_write\n");
}

void dummy_i2c_read(uint8_t name1, uint8_t name2, uint16_t *name3) {
    printf("dummy_i2c_read\n");

    *name3 = 0x1234;
    
}

void setUp(void) {
    
    ads111x_i2c_t port = {
        .Init      = dummy_i2c_init,
        .Write     = dummy_i2c_write,
        .WriteRead = dummy_i2c_read,
    };

    ADS111x_Init(&ads111x_obj, ADS111X_ADDR_0, ADS111X_PGA_2048, ADS1115, &port);
}

/**
 * @brief Test if the StartConversion function works properly
 * 
 */
void test_ADS111x_StartConversion(void) {
    ads111x_obj.configuration = 0xFFFF;
    ADS111x_StartConversion(&ads111x_obj);
    TEST_ASSERT_EQUAL(0x7FFF, ads111x_obj.configuration);
}

/**
 * @brief Test if set the multiplexer function works properly looking for the configuration register
 * 
 */
void test_ADS111x_SetMultiplexer(void) {
    ads111x_obj.configuration = 0xFFFF;
    ADS111x_SetMultiplexer(&ads111x_obj, ADS111X_MUX_AN0_GND);
    TEST_ASSERT_EQUAL(0xCFFF, ads111x_obj.configuration);
}

void test_ADS111x_SetDataRate(void) {
    ads111x_obj.configuration = 0xFFFF;
    ADS111x_SetDataRate(&ads111x_obj, ADS111X_DATA_RATE_475SPS);
    TEST_ASSERT_EQUAL(0xFFDF, ads111x_obj.configuration);
}

void test_ADS111x_Read(void) {
    // ADS111x_ReadRegister_ExpectAndReturn(ADS111X_ADDR_0, ADS111X_CONVERSION_REG, 0x1234); // Not working, Ask why
    int16_t data = ADS111x_Read(&ads111x_obj);
    TEST_ASSERT_EQUAL(0x1234,data);
}

void test_ADS111x_SetComparatorMode(void) {
    // ADS111x_ReadRegister_ExpectAndReturn(ADS111X_ADDR_0, ADS111X_CONVERSION_REG, 0x1234); // Not working, Ask why
    ads111x_obj.configuration = 0xFFFF;
    ADS111x_SetComparatorMode(&ads111x_obj, ADS111X_COMP_MODE_TRADITIONAL);
    TEST_ASSERT_EQUAL(0xFFEF, ads111x_obj.configuration);
}

void test_ADS111x_SetComparatorModeWithInvalidDevice(void) {
    // ADS111x_ReadRegister_ExpectAndReturn(ADS111X_ADDR_0, ADS111X_CONVERSION_REG, 0x1234); // Not working, Ask why
    ads111x_obj.configuration = 0xFFFF;
    ads111x_obj.device = ADS1115 + 1;
    ADS111x_SetComparatorMode(&ads111x_obj, ADS111X_COMP_MODE_TRADITIONAL);
    TEST_ASSERT_EQUAL(0xFFFF, ads111x_obj.configuration);
}
