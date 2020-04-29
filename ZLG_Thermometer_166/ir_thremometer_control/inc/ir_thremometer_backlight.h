#ifndef _IR_THREMOMETER_BACKLIGHT_H_
#define _IR_THREMOMETER_BACKLIGHT_H_

/**
 * \brief  �����ģʽ
 */
typedef enum {
	BACKLIGNT_RED = 0,
	BACKLIGNT_GREEN,
	BACKLIGNT_ORANGE,
} backlight_mode_t;

/**
 * \brief  ����Ƴ�ʼ��
 */
void ir_thremometer_backlight_init(void);

/**
 * \brief  ����ƿ���
 *
 * \param[in] mode : �����ģʽ
 *
 * \retval ��
 */
void ir_thremometer_backlight_on(backlight_mode_t mode);

/**
 * \brief  ����ƹر�
 */
void ir_thremometer_backlight_off(void);

#endif

/* end of file */
