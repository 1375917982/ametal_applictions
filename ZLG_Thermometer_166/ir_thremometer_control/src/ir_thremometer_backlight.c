#include "am_gpio.h"
#include "zlg116_pin.h"
#include "ir_thremometer_backlight.h"

/**
 * \brief  ���������
 */
#define BACKLIGHT_GREEN_PIN PIOA_15
#define BACKLIGHT_RED_PIN   PIOA_1

/**
 * \brief  ����Ƴ�ʼ��
 */
void ir_thremometer_backlight_init(void)
{
	am_gpio_pin_cfg(BACKLIGHT_GREEN_PIN, AM_GPIO_OUTPUT_INIT_HIGH);
	am_gpio_pin_cfg(BACKLIGHT_RED_PIN, AM_GPIO_OUTPUT_INIT_HIGH);

    ir_thremometer_backlight_off();
}

/**
 * \brief  ����ƿ���
 *
 * \param[in] mode : �����ģʽ
 *
 * \retval ��
 */
void ir_thremometer_backlight_on(backlight_mode_t mode)
{
	if (mode == BACKLIGNT_RED) {
		am_gpio_set(BACKLIGHT_GREEN_PIN, 1);
		am_gpio_set(BACKLIGHT_RED_PIN, 0);
	} else if (mode == BACKLIGNT_GREEN) {
		am_gpio_set(BACKLIGHT_GREEN_PIN, 0);
		am_gpio_set(BACKLIGHT_RED_PIN, 1);
	} else {
		am_gpio_set(BACKLIGHT_GREEN_PIN, 0);
		am_gpio_set(BACKLIGHT_RED_PIN, 0);
	}
}

/**
 * \brief  ����ƹر�
 */
void ir_thremometer_backlight_off(void)
{
	am_gpio_set(BACKLIGHT_GREEN_PIN, 1);
	am_gpio_set(BACKLIGHT_RED_PIN, 1);
}

/* end of file */
