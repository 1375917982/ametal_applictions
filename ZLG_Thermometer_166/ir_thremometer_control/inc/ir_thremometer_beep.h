#ifndef _IR_THREMOMETER_BEEP_H_
#define _IR_THREMOMETER_BEEP_H_

#include "ametal.h"

typedef enum {
	BEEP_LONG_NOISE = 0,    /* ���� */
	BEEP_SHORT_NOISE,       /* �̽� */
}beep_nosie_t;

/**
 * \brief  ��������ʼ��
 */
void ir_thremometer_beep_init(void);

/**
 * \brief  ����������
 *
 * \param[in] times : ���д���
 * \param[in] nosie : ��������
 *
 * \retval ��
 */
void ir_thremometer_beep_on(uint32_t times, beep_nosie_t nosie);

#endif

/* end of file */
