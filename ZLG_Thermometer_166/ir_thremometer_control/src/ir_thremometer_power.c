#include "am_gpio.h"
#include "zlg116_pin.h"
#include "ir_thremometer_power.h"

/* ��Դ�������� */
#define IR_THREM_POWER_PIN  PIOA_11

/* ��Դ��ʼ�� */
void ir_thremometer_power_init(void)
{
	am_gpio_pin_cfg(IR_THREM_POWER_PIN, AM_GPIO_OUTPUT_INIT_HIGH);
}

/* �򿪵�Դ */
void ir_thremometer_power_on(void)
{
	am_gpio_set(IR_THREM_POWER_PIN, 1);
}

/* �رյ�Դ */
void ir_thremometer_power_off(void)
{
	am_gpio_set(IR_THREM_POWER_PIN, 0);
}

/* end of file */
