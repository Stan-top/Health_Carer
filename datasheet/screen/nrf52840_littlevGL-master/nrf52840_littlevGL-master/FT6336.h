#ifndef __FT6336_H_
#define __FT6336_H_

 
#include <stdint.h>

/* TWI instance ID. */
#define TWI_INSTANCE_ID     1
 

#define FT6336_SCL_PIN	13
#define FT6336_SDA_PIN	14   
	 
#define FT6336_RST_PIN	8	    
#define FT6336_RST_H	 
#define FT6336_RST_L	 

#define FT6336_INT_PIN	26    

#define FT6336_ADDR 	0x38 
 
#define WRITE_BIT           I2C_MASTER_WRITE    //�0
#define READ_BIT            I2C_MASTER_READ     //�1 
 
#define ACK_CHECK_EN        0x1                 //主机检查从机的ACK 
#define ACK_CHECK_DIS		0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL				0x0              /*!< I2C ack value */
#define NACK_VAL			0x1              /*!< I2C nack value */

#define TP_PRES_DOWN 0x80  //����������	
#define TP_COORD_UD  0x40  //����������±��

//������������ݽṹ�嶨��
typedef struct			
{
	uint8_t TouchSta;	//���������b7:����1/�ɿ�0; b6:0û�а�������/1�а�������;bit5:����bit4-bit0�����㰴����Ч��־����ЧΪ1���ֱ��Ӧ������-1��
	uint16_t x[5];		//֧��5�㴥������Ҫʹ��5������洢����������
	uint16_t y[5];
	
}TouchPointRefTypeDef;
extern TouchPointRefTypeDef TPR_Structure;

/*********************IO��������*********************/									
//#define	FT6336_RST			PCout(7)	//�������ģ�					
//#define FT6336_SCL 			PGout(8)	//�������ģ�
//#define FT6336_SDA 			PGout(7)	//�������ģ�
//#define FT6336_SDA_Read 	PGin(7)		//��������ģʽ									
//#define FT6336_INT 			PCin(6)		//��������ģʽ
							
//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
//FT6336 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT6336ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�� 
 
#define FT_ID_G_MAX_X_HIGH		0x98		//x������ֱ��ʸ��ֽ�
#define	FT_ID_G_LIB_VERSION		0xA1		//�汾		
#define FT_ID_G_MODE 			0xA4   		//FT6336�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���  
#define Chip_Vendor_ID          0xA3        //оƬID(0x36)
#define ID_G_FT6336ID			0xA8		//0x11

void FT6336_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void FT6336_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void FT6336_Init(void);
void FT6336_Scan(void);
void FT6336_GetChipInfo(uint8_t *info);
void touch_state(void);



#include "lvgl.h"
bool FT6336_read(lv_indev_data_t *data);


#endif












