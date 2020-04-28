/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ZLG9021 �û������ļ�
 * \sa am_hwconf_zlg9021.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-28  pea, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg9021.h"
#include "am_hc32l13x_inst_init.h"
#include "hc32x3x_pin.h"
/**
 * \addtogroup am_if_src_hwconf_zlg9021
 * \copydoc am_hwconf_zlg9021.c
 * @{
 */

am_local uint8_t __g_zlg9021_txbuf[128]; /**< \brief ���ͻ����� */
am_local uint8_t __g_zlg9021_rxbuf[128]; /**< \brief ���ջ����� */

/** \brief �豸ʵ�� */
am_local am_zlg9021_dev_t __g_zlg9021_dev;

/** \brief �豸��Ϣ */
am_local am_const am_zlg9021_devinfo_t __g_zlg9021_devinfo = {
    -1,                   /**< \brief EN ���ţ������� MCU ���ƣ���ֵ����Ϊ -1 */
    -1,                   /**< \brief BRTS ���ţ����������� ZLG9021 ǰ�����ͣ����������� */
    PIOD_4,                   /**< \brief ��λ���� */
    -1,                        /**< \brief RESTORE ���ţ����ڻָ��������� */
    9600,                      /**< \brief ģ�鵱ǰʹ�õĲ����� */
    __g_zlg9021_rxbuf,         /**< \brief ���ջ��� */
    __g_zlg9021_txbuf,         /**< \brief ���ͻ��� */
    sizeof(__g_zlg9021_rxbuf), /**< \brief ���ջ��泤�� */
    sizeof(__g_zlg9021_txbuf)  /**< \brief ���ͻ��泤�� */
};

/**
 * \brief  ZLG9021 ʵ����ʼ������� ZLG9021 ��׼����������
 *
 * \return  ZLG9021 ��׼����������
 */
am_zlg9021_handle_t am_zlg9021_inst_init (void)
{
    am_uart_handle_t uart_handle = am_hc32_uart1_inst_init();

    return am_zlg9021_init(&__g_zlg9021_dev,
                           &__g_zlg9021_devinfo,
                           uart_handle);
}

/**
 * @}
 */

/* end of file */
