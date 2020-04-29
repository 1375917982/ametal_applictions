/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/
/**
 * \file
 * \brief AML166 ����ǹ����
 *
 * - �������裺
 *   1. ������¼��
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-13  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "ir_thremometer.h"
/**
 * \brief AMetal Ӧ�ó������
 */
int am_main (void)
{
    AM_DBG_INFO("Start up successful!\r\n");

	ir_thremometer_init();

	ir_thremometer_run();

    return  0;
}

