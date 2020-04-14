#include "IMU.h"

#include <string.h>

typedef struct bno055_t bno055_t;
I2C_HandleTypeDef hi2c1;
bno055_t imu;

static s8 i2c_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
    HAL_StatusTypeDef ret = 0;
    ret = HAL_I2C_Master_Transmit(
        &hi2c1,
        dev_addr << 1,
        &reg_addr,
        1,
        HAL_MAX_DELAY
    );
    if (ret != HAL_OK) return ret;

    ret = HAL_I2C_Master_Receive(
            &hi2c1,
            (dev_addr << 1) | 1,
            reg_data,
            cnt,
            HAL_MAX_DELAY
    );
    return ret;
}

static s8 i2c_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
    HAL_StatusTypeDef ret = 0;
    u8 to_send[cnt + 1];
    to_send[0] = reg_addr;
    memcpy(&to_send[1], reg_data, cnt);

    ret = HAL_I2C_Master_Transmit(
            &hi2c1, dev_addr << 1,
            to_send,
            cnt + 1, HAL_MAX_DELAY);
    return ret;
}

//TODO Replace with osDelay
static void delay(u32 ms) {
    HAL_Delay(ms);
}
static void imu_gpio_init() {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void i2c_init(void (*error_handler) (void)) {
    __HAL_RCC_I2C1_CLK_ENABLE();

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        error_handler();
    }
}

static void bno_init() {
    imu.bus_read = i2c_read;
    imu.bus_write = i2c_write;
    imu.delay_msec = delay;
    imu.dev_addr = BNO055_I2C_ADDR1;
    bno055_init(&imu);

    bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
}

void imu_init(void (*error_handler) (void)) {
    imu_gpio_init();
    i2c_init(error_handler);
    bno_init();

}

void imu_read_euler(IMU_Angles *angles) {
    bno055_read_euler_hrp(angles);
}
