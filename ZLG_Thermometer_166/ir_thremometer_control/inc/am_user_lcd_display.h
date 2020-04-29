#ifndef __AM_USER_LCD_DISPLAY_H
#define __AM_USER_LCD_DISPLAY_H

#include "am_common.h"
#include "am_pcf8551.h"

#define AM_USER_BT_DISP         (0x0001)        /* ��������    */
#define AM_USER_BZ_DISP         (0x0002)        /* ������        */
#define AM_USER_0F_DISP         (0x0004)        /* ���϶ȵ�λ    */
#define AM_USER_0C_DISP         (0x0008)        /* ���϶ȵ�λ    */
#define AM_USER_P1_DISP         (0x0010)        /* С����        */
#define AM_USER_BT3_DISP        (0x0020)        /* ������        */
#define AM_USER_LOG_DISP        (0x0040)        /* ����          */
#define AM_USER_SUR_DISP        (0x0080)        /* �����¶�      */
#define AM_USER_KU_DISP         (0x0100)        /* ����          */
#define AM_USER_XIAO_DISP       (0x0200)        /* Ц��          */
#define AM_USER_BLUE_DISP       (0x0400)        /* ����          */
#define AM_USER_BODY_DISP       (0x0800)        /* ����          */
#define AM_USER_BT2_DISP        (0x1000)        /* ������        */
#define AM_USER_BT1_DISP        (0x2000)        /* ������        */

#define AM_USER_NO_MEM          (100)

#define  am_lcd_handle_t  am_pcf8551_handle_t

/**
 * \brief  �û� LCD ��ʼ��
 */
am_lcd_handle_t am_user_lcd_disp_init (void);

/**
 * \brief  �û� LCD ����
 */
int am_user_lcd_disp_clr (am_lcd_handle_t handle);

/****************(�������º����൱��һ��ȫ����������ʾdata)*******************/

/**
 * \brief  �û� LCD д������
 */
int am_user_lcd_disp_float (am_lcd_handle_t handle,
                            float           data);

/**
 * \brief  �û� LCD ��ʾ��ر���
 *
 * \param[in] handle        :   LCD�豸������
 * \param[in] alarm_type    :   �������ͣ��� # AM_USER_DISP_ERR
 *
 * \retval ���� : ��Ӧ������
 * \retval  0 : ִ�гɹ�
 */
typedef enum {
    AM_USER_DISP_ERR = 0,
    AM_USER_DISP_LO,
    AM_USER_DISP_HO,
    AM_USER_DISP_STARTUP_DISP,
	AM_USER_DISP_F2,
	AM_USER_DISP_F4
}am_user_lcd_disp_alarm_t;

int am_user_lcd_disp_alarm(am_lcd_handle_t              handle,
                           am_user_lcd_disp_alarm_t     alarm_type);

/*****************************************************************************/

/**
 * \brief  �û� LCD д��������ȡ����
 */
int am_user_lcd_disp_mem(am_lcd_handle_t    handle,
                         uint32_t           data);

/**
 * \brief  �û� LCD ��������ʾ��ر���
 */
typedef enum {
    AM_USER_MEM_DISP_F2 = 0,
    AM_USER_MEM_DISP_F4,
}am_user_lcd_disp_mem_alarm_t;

int am_user_lcd_disp_mem_alarm(am_lcd_handle_t                  handle,
                               am_user_lcd_disp_mem_alarm_t     alarm_type);

/**
 * \brief  �û� LCD ������ʾʹ��
 *
 * \param[in] handle        :   LCD�豸������
 * \param[in] flags         :   ʹ����Ӧ������ʾ���磺# AM_USER_BT_DISP
 *                              Ҳ����ͬʱ���������꣺# AM_USER_BT_DISP | AM_USER_BODY_DISP
 *
 * \retval ���� : ��Ӧ������
 * \retval  0 : ִ�гɹ�
 */
int am_user_lcd_disp_flag_enable(am_lcd_handle_t    handle,
                                 uint32_t           flags);

/**
 * \brief  �û� LCD ������ʾ����
 *
 * \param[in] handle        :   LCD�豸������
 * \param[in] flags         :   ������Ӧ������ʾ���磺# AM_USER_BT_DISP
 *                              Ҳ����ͬʱ���������꣺# AM_USER_BT_DISP | AM_USER_BODY_DISP
 *
 * \retval ���� : ��Ӧ������
 * \retval  0 : ִ�гɹ�
 */
int am_user_lcd_disp_flag_disable(am_lcd_handle_t    handle,
                                  uint32_t           flags);

#endif

