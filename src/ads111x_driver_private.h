#include "ads111x_driver.h"

#define ADS111X_GENERAL_CALL_ADDR 0x00
#define ADS111X_CMD_RESET         0x06
#define ADS111X_HS_MASTER_CODE    0x03
#define ADS111X_HS_MODE           (0x08 | ADS111X_HS_MASTER_CODE)

#define ADS_111x_RESET_CONFIG    0x8583

#define ADS111X_CONFIG_FIELD_COMP_QUE_BIT       0x00
#define ADS111X_CONFIG_FIELD_COMP_LAT_BIT       0x02
#define ADS111X_CONFIG_FIELD_COMP_POL_BIT       0x03
#define ADS111X_CONFIG_FIELD_COMP_MODE_BIT      0x04
#define ADS111X_CONFIG_FIELD_DR_BIT             0x05
#define ADS111X_CONFIG_FIELD_MODE_BIT           0x08
#define ADS111X_CONFIG_FIELD_PGA_BIT            0x09
#define ADS111X_CONFIG_FIELD_MUX_BIT            0x0C
#define ADS111X_CONFIG_FIELD_OS_BIT             0x0F

#define ADS111X_CONFIG_FIELD_COMP_QUE_MASK (0x03 << ADS111X_CONFIG_FIELD_COMP_QUE_BIT)
#define ADS111X_CONFIG_FIELD_COMP_LAT_MASK (0x01 << ADS111X_CONFIG_FIELD_COMP_LAT_BIT)
#define ADS111X_CONFIG_FIELD_COMP_POL_MASK (0x01 << ADS111X_CONFIG_FIELD_COMP_POL_BIT)
#define ADS111X_CONFIG_FIELD_COMP_MODE_MASK (0x01 << ADS111X_CONFIG_FIELD_COMP_MODE_BIT)
#define ADS111X_CONFIG_FIELD_DR_MASK (0x07 << ADS111X_CONFIG_FIELD_DR_BIT)
#define ADS111X_CONFIG_FIELD_MODE_MASK (0x01 << ADS111X_CONFIG_FIELD_MODE_BIT)
#define ADS111X_CONFIG_FIELD_PGA_MASK (0x07 << ADS111X_CONFIG_FIELD_PGA_BIT)
#define ADS111X_CONFIG_FIELD_MUX_MASK (0x07 << ADS111X_CONFIG_FIELD_MUX_BIT)
#define ADS111X_CONFIG_FIELD_OS_MASK (0x01 << ADS111X_CONFIG_FIELD_OS_BIT)

static ads111x_i2c_t ads111x_i2c;

static const uint16_t ads111x_config_mask[ADS111X_CONFIG_FIELD_QTY] = {
    [ADS111X_CONFIG_FIELD_COMP_QUE] = ADS111X_CONFIG_FIELD_COMP_QUE_MASK,
    [ADS111X_CONFIG_FIELD_COMP_LAT] = ADS111X_CONFIG_FIELD_COMP_LAT_MASK,
    [ADS111X_CONFIG_FIELD_COMP_POL] = ADS111X_CONFIG_FIELD_COMP_POL_MASK,
    [ADS111X_CONFIG_FIELD_COMP_MODE] = ADS111X_CONFIG_FIELD_COMP_MODE_MASK,
    [ADS111X_CONFIG_FIELD_DR] = ADS111X_CONFIG_FIELD_DR_MASK,
    [ADS111X_CONFIG_FIELD_MODE] = ADS111X_CONFIG_FIELD_MODE_MASK,
    [ADS111X_CONFIG_FIELD_PGA] = ADS111X_CONFIG_FIELD_PGA_MASK,
    [ADS111X_CONFIG_FIELD_MUX] = ADS111X_CONFIG_FIELD_MUX_MASK,
    [ADS111X_CONFIG_FIELD_OS] = ADS111X_CONFIG_FIELD_OS_MASK
};
static const uint16_t ads111x_config_bit[ADS111X_CONFIG_FIELD_QTY] = {
    [ADS111X_CONFIG_FIELD_COMP_QUE] = ADS111X_CONFIG_FIELD_COMP_QUE_BIT,
    [ADS111X_CONFIG_FIELD_COMP_LAT] = ADS111X_CONFIG_FIELD_COMP_LAT_BIT,
    [ADS111X_CONFIG_FIELD_COMP_POL] = ADS111X_CONFIG_FIELD_COMP_POL_BIT,
    [ADS111X_CONFIG_FIELD_COMP_MODE] = ADS111X_CONFIG_FIELD_COMP_MODE_BIT,
    [ADS111X_CONFIG_FIELD_DR] = ADS111X_CONFIG_FIELD_DR_BIT,
    [ADS111X_CONFIG_FIELD_MODE] = ADS111X_CONFIG_FIELD_MODE_BIT,
    [ADS111X_CONFIG_FIELD_PGA] = ADS111X_CONFIG_FIELD_PGA_BIT,
    [ADS111X_CONFIG_FIELD_MUX] = ADS111X_CONFIG_FIELD_MUX_BIT,
    [ADS111X_CONFIG_FIELD_OS] = ADS111X_CONFIG_FIELD_OS_BIT
};

static void ADS111x_WriteRegister(ads111x_addr_t i2c_address, ads111x_reg_t reg, uint16_t value);

static uint16_t ADS111x_ReadRegister(uint8_t i2c_address, uint8_t reg);

static void ADS111x_SetConfiguration(ads111x_obj_t *ptr_asd111x, ads111x_config_field_t field, uint8_t field_config);