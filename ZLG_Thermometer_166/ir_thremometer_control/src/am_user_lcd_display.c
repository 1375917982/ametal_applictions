/**
 * \file
 * \brief LCD������ʵ��
 *
 */

#include "ametal.h"
#include "am_gpio.h"
#include "am_zlg116.h"
#include "zlg116_pin.h"
#include "am_aml166_inst_init.h"
#include "am_user_lcd_display.h"
#include "am_hwconf_pcf8551.h"

/* �Դ���ֹ��ַ */
#define DISP_ADDR_START                    (0X04)
#define DISP_ADDR_END                      (0X17)

/* ������ʾ�궨�� */
#define AMHW_USER_BT_LCDRAM                 (0)
#define AMHW_USER_BZ_LCDRAM                 (2)
#define AMHW_USER_0F_LCDRAM                 (4)
#define AMHW_USER_0C_LCDRAM                 (6)
#define AMHW_USER_P1_LCDRAM                 (0)
#define AMHW_USER_BT3_LCDRAM                (0)
#define AMHW_USER_LOG_LCDRAM                (0)
#define AMHW_USER_SUR_LCDRAM                (1)
#define AMHW_USER_KU_LCDRAM                 (1)
#define AMHW_USER_XIAO_LCDRAM               (1)
#define AMHW_USER_BLUE_LCDRAM               (1)
#define AMHW_USER_BODY_LCDRAM               (3)
#define AMHW_USER_BT2_LCDRAM                (5)
#define AMHW_USER_BT1_LCDRAM                (7)

#define AMHW_USER_BT_DISP                   (0x01)
#define AMHW_USER_BZ_DISP                   (0x01)
#define AMHW_USER_0F_DISP                   (0x01)
#define AMHW_USER_0C_DISP                   (0x01)
#define AMHW_USER_P1_DISP                   (0x04)
#define AMHW_USER_BT3_DISP                  (0x10)
#define AMHW_USER_LOG_DISP                  (0x40)
#define AMHW_USER_SUR_DISP                  (0x01)
#define AMHW_USER_KU_DISP                   (0x04)
#define AMHW_USER_XIAO_DISP                 (0x10)
#define AMHW_USER_BLUE_DISP                 (0x20)
#define AMHW_USER_BODY_DISP                 (0x20)
#define AMHW_USER_BT2_DISP                  (0x20)
#define AMHW_USER_BT1_DISP                  (0x20)

/* ����LCD��ʾ����*/
typedef enum {
    AM_DISP_FLOAT = 0,
    AM_DISP_ALARM,
    AM_DISP_MEM,
    AM_DISP_MEM_ALARM,
    AM_DISP_FLAG
} am_lcd_disp_type_t;

/* ����LCDRAM */
static uint8_t __g_lcdram[8] = {0};

/**
 * \brief �Դ��ַ
 *
 * \note : 8����ַ����
 */
static uint8_t lcd_addr[8] = 
{ 
   0x04, 0x05, /* COM0��seg0~seg13 */
   0x09, 0x0A, /* COM1��seg0~seg13 */
   0x0E, 0x0F, /* COM2��seg0~seg13 */
   0X13, 0X14  /* COM3��seg0~seg13 */
};

/**
 * \brief ��ʾ����
 *
 * \note : LCD��һ���ĸ�������ʾ��Ԫ��LCDÿ��������ʾ��4���ֽڵ�2λ����
 */
static uint8_t lcd_datacode[][4] = 
{ 
  /* COM0, COM1, COM2, COM3 */
    {0x01, 0x03, 0x01, 0x03},    /* ��ʾ0 */
    {0x00, 0x01, 0x01, 0x00},    /* ��ʾ1 */
    {0x01, 0x02, 0x03, 0x01},    /* ��ʾ2 */
    {0x01, 0x01, 0x03, 0x01},    /* ��ʾ3 */
    {0x00, 0x01, 0x03, 0x02},    /* ��ʾ4 */
    {0x01, 0x01, 0x02, 0x03},    /* ��ʾ5 */
    {0x01, 0x03, 0x02, 0x03},    /* ��ʾ6 */
    {0x00, 0x01, 0x01, 0x01},    /* ��ʾ7 */
    {0x01, 0x03, 0x03, 0x03},    /* ��ʾ8 */
    {0x01, 0x01, 0x03, 0x03},    /* ��ʾ9 */
    
    {0x00, 0x00, 0x00, 0x00},    /* ����ʾ */
    {0x01, 0x02, 0x02, 0x03},    /* ��ʾE */
    {0x00, 0x03, 0x03, 0x03},    /* ��ʾR */
    {0x01, 0x02, 0x00, 0x02},    /* ��ʾL */
    {0x01, 0x03, 0x02, 0x00},    /* ��ʾo */
    {0x00, 0x03, 0x03, 0x02},    /* ��ʾH */
    {0x00, 0x00, 0x02, 0x00},    /* ��ʾ- */
    {0x00, 0x02, 0x02, 0x03}     /* ��ʾF */
};

/* ��ʾ�ض��ַ�λ�ã������ַ��ڶ�����е�λ�� 
 * ���� 0~9 ռ�����ǰ10�����ض�λ�ô� 10 ��ʼ
 */
#define DISP_POS_BLANK          (10)  /* ����ʾ */
#define DISP_POS_E              (11)  /* ��ʾE */
#define DISP_POS_R              (12)  /* ��ʾR */
#define DISP_POS_L              (13)  /* ��ʾL */
#define DISP_POS_o              (14)  /* ��ʾo */
#define DISP_POS_H              (15)  /* ��ʾH */
#define DISP_POS_SEG_g          (16)  /* ��ʾ- */
#define DISP_POS_F              (17)  /* ��ʾF */

/**
 * \brief LCD��ʾ����
 */
static void __lcd_disp_data (am_lcd_handle_t     handle,
                             uint8_t            *data_bit,
                             am_lcd_disp_type_t  disp_type)
{
    uint8_t i = 0;

    if ((disp_type == AM_DISP_FLOAT) || (disp_type == AM_DISP_ALARM)) {

        /* ���1��2λ���� */
        __g_lcdram[1] &= ~0x0A;
        __g_lcdram[3] &= ~0x1E;
        __g_lcdram[5] &= ~0x1E;
        __g_lcdram[7] &= ~0x1E;

        /* ���3��4λ���� */
        __g_lcdram[0] &= ~0x0A;
        __g_lcdram[2] &= ~0x1E;
        __g_lcdram[4] &= ~0x1E;
        __g_lcdram[6] &= ~0x1E;
        
        /* д��1��2λ���� */
        __g_lcdram[1] |= (lcd_datacode[data_bit[3]][0] << 3) |  /* 05h */
                         (lcd_datacode[data_bit[2]][0] << 1);
        __g_lcdram[3] |= (lcd_datacode[data_bit[3]][1] << 3) |  /* 0Ah */
                         (lcd_datacode[data_bit[2]][1] << 1);
        __g_lcdram[5] |= (lcd_datacode[data_bit[3]][2] << 3) |  /* 0Fh */
                         (lcd_datacode[data_bit[2]][2] << 1);
        __g_lcdram[7] |= (lcd_datacode[data_bit[3]][3] << 3) |  /* 14h */
                         (lcd_datacode[data_bit[2]][3] << 1);
        
        /* С���� */
        __g_lcdram[0] |= 0x04;
        
        /* д��3��4λ���� */
        __g_lcdram[0] |= (lcd_datacode[data_bit[1]][0] << 3) |  /* 04h */
                         (lcd_datacode[data_bit[0]][0] << 1);   
        __g_lcdram[2] |= (lcd_datacode[data_bit[1]][1] << 3) |  /* 09h */
                         (lcd_datacode[data_bit[0]][1] << 1);   
        __g_lcdram[4] |= (lcd_datacode[data_bit[1]][2] << 3) |  /* 0Eh */
                         (lcd_datacode[data_bit[0]][2] << 1);   
        __g_lcdram[6] |= (lcd_datacode[data_bit[1]][3] << 3) |  /* 13h */
                         (lcd_datacode[data_bit[0]][3] << 1);

    } else if ((disp_type == AM_DISP_MEM) || (disp_type == AM_DISP_MEM_ALARM)) {
        
        /* ���5��6λ���� */
        __g_lcdram[1] &= ~0x00;
        __g_lcdram[3] &= ~0x01;
        __g_lcdram[5] &= ~0x01;
        __g_lcdram[7] &= ~0x01;

        __g_lcdram[0] &= ~0xA0;
        __g_lcdram[2] &= ~0xE0;
        __g_lcdram[4] &= ~0xE0;
        __g_lcdram[6] &= ~0xE0;
        
        /* д��5��6λ���� */
        __g_lcdram[1] |= (lcd_datacode[data_bit[1]][0] >> 1);   /* 05h */
        __g_lcdram[3] |= (lcd_datacode[data_bit[1]][1] >> 1);   /* 0Ah */
        __g_lcdram[5] |= (lcd_datacode[data_bit[1]][2] >> 1);   /* 0Fh */
        __g_lcdram[7] |= (lcd_datacode[data_bit[1]][3] >> 1);   /* 14h */

        __g_lcdram[0] |= (lcd_datacode[data_bit[1]][0] << 7) |  /* 04h */
                         (lcd_datacode[data_bit[0]][0] << 5);   
        __g_lcdram[2] |= (lcd_datacode[data_bit[1]][1] << 7) |  /* 09h */
                         (lcd_datacode[data_bit[0]][1] << 5);   
        __g_lcdram[4] |= (lcd_datacode[data_bit[1]][2] << 7) |  /* 0Eh */
                         (lcd_datacode[data_bit[0]][2] << 5);   
        __g_lcdram[6] |= (lcd_datacode[data_bit[1]][3] << 7) |  /* 13h */
                         (lcd_datacode[data_bit[0]][3] << 5);

    } else if (disp_type == AM_DISP_FLAG) {

    }
    
    for (i = 0; i < 8; i++) {

        am_pcf8551_display(handle, lcd_addr[i], &__g_lcdram[i], 1);
    }
}

/**
 * \brief �û� LCD ��ʼ��
 */
am_lcd_handle_t am_user_lcd_disp_init (void)
{
    am_lcd_handle_t handle = am_pcf8551_inst_init();

    am_pcf8551_display_on(handle);

    return handle;
}

/**
 * \brief �û� LCD ����
 */
int am_user_lcd_disp_clr (am_lcd_handle_t handle)
{
    int i = 0;
    uint8_t temp = 0x00;

    if (handle == NULL){

        return AM_ERROR;
    }

    /* ����Դ����� */
    for (i = DISP_ADDR_START; i <= DISP_ADDR_END; i++){

        am_pcf8551_display(handle, i, &temp, 1);
    }

    return AM_OK;
}

/****************(�������º����൱��һ��ȫ����������ʾdata)*******************/

/**
 * \brief �û� LCD д������
 */
int am_user_lcd_disp_float (am_lcd_handle_t handle,
                            float           data)
{
    if (handle == NULL){
        return AM_ERROR;
    }

    uint8_t         i = 0;
    int             data_ten_times = 0;
    uint8_t         data_bit[4] = {0};
    uint32_t        data_div = 0;

    if (data < -0.000001) {
        /* data Ϊ���� */
        data_ten_times = (int)(data * 10);      /* �� data �Ŵ� 10 ����ȡ�� */
        data_ten_times = 0 - data_ten_times;    /* ȡ����ֵ     */

        data_div = data_ten_times;
        /* �������λ */
        while (data_div != 0) {
            *(data_bit + i) = data_div % 10;
            i++;
            data_div /= 10;
        }

        /* �жϵڶ�������ܵ�ֵ�Ƿ�Ϊ0 */
        for (i = 2; i > 1; i--) {
            if (data_bit[i] != 0) {
                break;
            } else {
                data_bit[i] = DISP_POS_BLANK;
            }
        }

        /* ��λ��ʾ���� ��-�� */
        data_bit[3] = DISP_POS_SEG_g;
    } else if ((data >= 0) && (data <= 999.9)) { 

        data_ten_times = (int)(data * 10);  /* �� data �Ŵ� 10 ����ȡ�� */
        data_div = data_ten_times;
        /* ��data��Ϊ0ʱ�������λ��dataΪ0ʱ����while */
        while (data_div != 0) {
            *(data_bit + i) = data_div % 10;
            i++;
            data_div /= 10;
        }

        /*  
            �˶����ã�����λΪ0ʱ����ʾ���� data = 0 ʱ��ֻ��ʾ��λ��0��
        ����
            ��λ����ȫ��ʾ                                  1357    ��ʾ 1357
            ��λ�������λ0����ʾ                           103     ��ʾ x103
            ��λ�������λ�ʹθ�λ��0����ʾ                 78      ��ʾ xx78
            һλ�����������λ��0����ʾ��ֻ��ʾ���λ       5       ��ʾ xxx5
                    ��������Ϊ0ʱ����ʾxxx0��
        */
        for (i = 3; i > 1; i--) {        /* �����λ���ε�λ�ж� */
            if (data_bit[i] != 0) {
                break;
            } else {
                data_bit[i] = DISP_POS_BLANK;
            }
        }
    } else {

    }
    
    /* ��ʾ���� */
    __lcd_disp_data(handle, data_bit, AM_DISP_FLOAT);
    
    return AM_OK;
}

/**
 * \brief �û� LCD ��ʾ��ر���
 */
int am_user_lcd_disp_alarm(am_lcd_handle_t              handle,
                           am_user_lcd_disp_alarm_t     alarm_type)
{
    if (handle == NULL){
        return AM_ERROR;
    }

    uint8_t data_bit[4] = {0};

    if (alarm_type == AM_USER_DISP_ERR) {

        data_bit[0] = DISP_POS_R;       /*  ��ĸR           */
        data_bit[1] = DISP_POS_R;       /*  ��ĸR           */
        data_bit[2] = DISP_POS_E;       /*  ��ĸE           */
        data_bit[3] = DISP_POS_BLANK;   /*  �հף�����ʾ    */

    } else if (alarm_type == AM_USER_DISP_LO) {

        data_bit[0] = DISP_POS_BLANK;   /* �հף�����ʾ��   */
        data_bit[1] = DISP_POS_o;       /* ��ĸo            */
        data_bit[2] = DISP_POS_L;       /* ��ĸL            */
        data_bit[3] = DISP_POS_BLANK;   /* �հף�����ʾ��   */

    } else if (alarm_type == AM_USER_DISP_HO) {

        data_bit[0] = DISP_POS_BLANK;   /* �հף�����ʾ��   */
        data_bit[1] = DISP_POS_o;       /* ��ĸo            */
        data_bit[2] = DISP_POS_H;       /* ��ĸH            */
        data_bit[3] = DISP_POS_BLANK;   /* �հף�����ʾ��   */

    } else if (alarm_type == AM_USER_DISP_STARTUP_DISP){

        data_bit[0] = DISP_POS_SEG_g;   /* G����            */
        data_bit[1] = DISP_POS_SEG_g;   /* G����            */
        data_bit[2] = DISP_POS_SEG_g;   /* G����            */
        data_bit[3] = DISP_POS_BLANK;   /* �հף�����ʾ��   */

    } else if (alarm_type == AM_USER_DISP_F2){
		
        data_bit[0] = 2;  				/* ����2            */
        data_bit[1] = DISP_POS_SEG_g;   /* G����            */
        data_bit[2] = DISP_POS_F;       /* ��ĸF            */
        data_bit[3] = DISP_POS_BLANK;   /* �հף�����ʾ��   */
    } else if (alarm_type == AM_USER_DISP_F4){
		
        data_bit[0] = 4;  				/* ����4            */
        data_bit[1] = DISP_POS_SEG_g;   /* G����            */
        data_bit[2] = DISP_POS_F;       /* ��ĸF            */
        data_bit[3] = DISP_POS_BLANK;   /* �հף�����ʾ��   */
    } else {
		
	}

    /* ��ʾ���� */
    __lcd_disp_data(handle, data_bit, AM_DISP_ALARM);

    return AM_OK;
}

/*****************************************************************************/

/**
 * \brief �û� LCD д��������ȡ����
 */
int am_user_lcd_disp_mem(am_lcd_handle_t    handle,
                         uint32_t           data)
{
    if (handle == NULL){
        return AM_ERROR;
    }

    uint8_t  i = 0;
    uint8_t  data_bit[2] = {0};
    uint32_t data_div = data;

    if (data > 99) {
		if (data == AM_USER_NO_MEM) {
			data_bit[0] = DISP_POS_SEG_g;
			data_bit[1] = DISP_POS_SEG_g;
		}
    } else {
        /* ��data��Ϊ0ʱ�������λ��dataΪ0ʱ����while */
        while (data_div != 0) {
            *(data_bit + i) = data_div % 10;
            i++;
            data_div /= 10;
        }
    }

    /* ��ʾ���� */
    __lcd_disp_data(handle, data_bit, AM_DISP_MEM);

    return AM_OK;
}

/**
 * \brief  �û� LCD ��������ʾ��ر���
 */
int am_user_lcd_disp_mem_alarm(am_lcd_handle_t                  handle,
                               am_user_lcd_disp_mem_alarm_t     alarm_type)
{
    if (handle == NULL){
        return AM_ERROR;
    }

    uint8_t data_bit[2] = {0};

    if (alarm_type == AM_USER_MEM_DISP_F2){

        data_bit[0] = 2;                /* ����2 */
        data_bit[1] = DISP_POS_F;       /* ��ĸF */

    } else if (alarm_type == AM_USER_MEM_DISP_F4) {

        data_bit[0] = 4;                /* ����4 */
        data_bit[1] = DISP_POS_F;       /* ��ĸF */

    } else {

    }

    /* ��ʾ���� */
    __lcd_disp_data(handle, data_bit, AM_DISP_MEM_ALARM);

    return AM_OK;
}

/**
 * \brief �û� LCD ������ʾʹ��
 */
int am_user_lcd_disp_flag_enable(am_lcd_handle_t    handle,
                                 uint32_t           flags)
{
    if (handle == NULL){
        return AM_ERROR;
    }

    if (flags & AM_USER_BT_DISP) {
        __g_lcdram[AMHW_USER_BT_LCDRAM] |= AMHW_USER_BT_DISP;
    } else {
        __g_lcdram[AMHW_USER_BT_LCDRAM] &= ~AMHW_USER_BT_DISP;
    }

    if (flags & AM_USER_BZ_DISP) {
        __g_lcdram[AMHW_USER_BZ_LCDRAM] |= AMHW_USER_BZ_DISP;
    } else {
        __g_lcdram[AMHW_USER_BZ_LCDRAM] &= ~AMHW_USER_BZ_DISP;
    }

    if (flags & AM_USER_0F_DISP) {
        __g_lcdram[AMHW_USER_0F_LCDRAM] |= AMHW_USER_0F_DISP;
    } else {
        __g_lcdram[AMHW_USER_0F_LCDRAM] &= ~AMHW_USER_0F_DISP;
    }

    if (flags & AM_USER_0C_DISP) {
        __g_lcdram[AMHW_USER_0C_LCDRAM] |= AMHW_USER_0C_DISP;
    } else  {
        __g_lcdram[AMHW_USER_0C_LCDRAM] &= ~AMHW_USER_0C_DISP;
    }

    if (flags & AM_USER_P1_DISP) {
        __g_lcdram[AMHW_USER_P1_LCDRAM] |= AMHW_USER_P1_DISP;
    } else {
        __g_lcdram[AMHW_USER_P1_LCDRAM] &= ~AMHW_USER_P1_DISP;
    }

    if (flags & AM_USER_BT3_DISP) {
        __g_lcdram[AMHW_USER_BT3_LCDRAM] |= AMHW_USER_BT3_DISP;
    } else {
        __g_lcdram[AMHW_USER_BT3_LCDRAM] &= ~AMHW_USER_BT3_DISP;
    }

    if (flags & AM_USER_LOG_DISP) {
        __g_lcdram[AMHW_USER_LOG_LCDRAM] |= AMHW_USER_LOG_DISP;
    } else {
        __g_lcdram[AMHW_USER_LOG_LCDRAM] &= ~AMHW_USER_LOG_DISP;
    }

    if (flags & AM_USER_SUR_DISP) {
        __g_lcdram[AMHW_USER_SUR_LCDRAM] |= AMHW_USER_SUR_DISP;
    } else {
        __g_lcdram[AMHW_USER_SUR_LCDRAM] &= ~AMHW_USER_SUR_DISP;
    }

    if (flags & AM_USER_KU_DISP) {
        __g_lcdram[AMHW_USER_KU_LCDRAM] |= AMHW_USER_KU_DISP;
    } else {
        __g_lcdram[AMHW_USER_KU_LCDRAM] &= ~AMHW_USER_KU_DISP;
    }

    if (flags & AM_USER_XIAO_DISP) {
        __g_lcdram[AMHW_USER_XIAO_LCDRAM] |= AMHW_USER_XIAO_DISP;
    } else {
        __g_lcdram[AMHW_USER_XIAO_LCDRAM] &= ~AMHW_USER_XIAO_DISP;
    }


    if (flags & AM_USER_BLUE_DISP) {
        __g_lcdram[AMHW_USER_BLUE_LCDRAM] |= AMHW_USER_BLUE_DISP;
    } else {
        __g_lcdram[AMHW_USER_BLUE_LCDRAM] &= ~AMHW_USER_BLUE_DISP;
    }

    if (flags & AM_USER_BODY_DISP) {
        __g_lcdram[AMHW_USER_BODY_LCDRAM] |= AMHW_USER_BODY_DISP;
    } else {
        __g_lcdram[AMHW_USER_BODY_LCDRAM] &= ~AMHW_USER_BODY_DISP;
    }
    if (flags & AM_USER_BT2_DISP) {
        __g_lcdram[AMHW_USER_BT2_LCDRAM] |= AMHW_USER_BT2_DISP;
    } else  {
        __g_lcdram[AMHW_USER_BT2_LCDRAM] &= ~AMHW_USER_BT2_DISP;
    } 
    if (flags & AM_USER_BT1_DISP) {
        __g_lcdram[AMHW_USER_BT1_LCDRAM] |= AMHW_USER_BT1_DISP;
    } else {
        __g_lcdram[AMHW_USER_BT1_LCDRAM] &= ~AMHW_USER_BT1_DISP;
    }

    /* ��ʾ���� */
    __lcd_disp_data(handle, NULL, AM_DISP_FLAG);

    return AM_OK;
}

/**
 * \brief �û� LCD ������ʾ����
 */
int am_user_lcd_disp_flag_disable(am_lcd_handle_t    handle,
                                  uint32_t           flags)
{
    if (handle == NULL){
        return AM_ERROR;
    }

    if (flags & AM_USER_BT_DISP) {
        __g_lcdram[AMHW_USER_BT_LCDRAM] &= (~AMHW_USER_BT_DISP);
    }

    if (flags & AM_USER_BZ_DISP) {
        __g_lcdram[AMHW_USER_BZ_LCDRAM] &= (~AMHW_USER_BZ_DISP);
    }

    if (flags & AM_USER_0F_DISP) {
        __g_lcdram[AMHW_USER_0F_LCDRAM] &= (~AMHW_USER_0F_DISP);
    }

    if (flags & AM_USER_0C_DISP) {
        __g_lcdram[AMHW_USER_0C_LCDRAM] &= (~AMHW_USER_0C_DISP);
    }

    if (flags & AM_USER_P1_DISP) {
        __g_lcdram[AMHW_USER_P1_LCDRAM] &= (~AMHW_USER_P1_DISP);
    }

    if (flags & AM_USER_BT3_DISP) {
        __g_lcdram[AMHW_USER_BT3_LCDRAM] &= (~AMHW_USER_BT3_DISP);
    }

    if (flags & AM_USER_LOG_DISP) {
        __g_lcdram[AMHW_USER_LOG_LCDRAM] &= (~AMHW_USER_LOG_DISP);
    }

    if (flags & AM_USER_SUR_DISP) {
        __g_lcdram[AMHW_USER_SUR_LCDRAM] &= (~AMHW_USER_SUR_DISP);
    }

    if (flags & AM_USER_KU_DISP) {
        __g_lcdram[AMHW_USER_KU_LCDRAM] &= (~AMHW_USER_KU_DISP);
    }

    if (flags & AM_USER_XIAO_DISP) {
        __g_lcdram[AMHW_USER_XIAO_LCDRAM] &= (~AMHW_USER_XIAO_DISP);
    }

    if (flags & AM_USER_BLUE_DISP) {
        __g_lcdram[AMHW_USER_BLUE_LCDRAM] &= (~AMHW_USER_BLUE_DISP);
    }

    if (flags & AM_USER_BODY_DISP) {
        __g_lcdram[AMHW_USER_BODY_LCDRAM] &= (~AMHW_USER_BODY_DISP);
    }

    if (flags & AM_USER_BT2_DISP) {
        __g_lcdram[AMHW_USER_BT2_LCDRAM] &= (~AMHW_USER_BT2_DISP);
    }

    if (flags & AM_USER_BT1_DISP) {
        __g_lcdram[AMHW_USER_BT1_LCDRAM] &= (~AMHW_USER_BT1_DISP);
    }

    /* ��ʾ���� */
    __lcd_disp_data(handle, NULL, AM_DISP_FLAG);

    return AM_OK;
}

/* end of file */

