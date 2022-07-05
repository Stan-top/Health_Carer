 
#include "FT6336.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "app_error.h"
#include "nrf_drv_gpiote.h"

TouchPointRefTypeDef TPR_Structure;
 uint8_t ft6336_info[100];
/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;
/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
//��FT6336д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
void FT6336_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
    ret_code_t err_code;
 

    uint8_t data[200] ;
    if(len> 200 -1)
    {
        return ;
    }
   
  data[0] = reg;
    memcpy(&data[1], buf, len);

    m_xfer_done = false;
    err_code = nrf_drv_twi_tx(&m_twi, FT6336_ADDR, data, len+1, false);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false);
 
	
}
//��FT6336����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void FT6336_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
 ret_code_t err_code;

    m_xfer_done = false;
    err_code = nrf_drv_twi_tx(&m_twi, FT6336_ADDR, &reg, 1, true);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false);

    m_xfer_done = false;
    err_code = nrf_drv_twi_rx(&m_twi, FT6336_ADDR, buf, len);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false);


} 


/**
 * @brief Function for handling data from temperature sensor.
 *
 * @param[in] temp          Temperature in Celsius degrees read from sensor.
 */
__STATIC_INLINE void data_handler(uint8_t temp)
{
   
}
/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                data_handler(0);
            }
            m_xfer_done = true;
            break;
        default:
            break;
    }
}


uint8_t cnt=0;
void FT6336_IRQ_Handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
 
 
//	PEN_INT=true;
//cnt++;
//printf("tp event %d\r\n",cnt);
			TPR_Structure.TouchSta |= TP_COORD_UD;				//���������и���

 
}
/* 
**��������FT6336_Init
**�����������
**����ֵ����
**���ܣ���ʼ��FT6336����
*/  
void FT6336_Init(void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_6336b_config = {
       .scl                = FT6336_SCL_PIN,
       .sda                = FT6336_SDA_PIN,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = true
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_6336b_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);


	//Initialize non-SPI GPIOs
	nrf_gpio_cfg_output(FT6336_RST_PIN);


        //�ж�
//	nrf_gpio_cfg_input(FT6336_INT_PIN);
	err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(FT6336_INT_PIN, &in_config, FT6336_IRQ_Handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(FT6336_INT_PIN, true);

	//reset
	nrf_gpio_pin_set(FT6336_RST_PIN);
	nrf_delay_ms(100);
	nrf_gpio_pin_clear(FT6336_RST_PIN);
	nrf_delay_ms(200);
	nrf_gpio_pin_set(FT6336_RST_PIN);
	nrf_delay_ms(100);
	
	uint8_t temp=0;
	FT6336_WR_Reg(FT_DEVIDE_MODE,&temp,1);	//������������ģʽ 
//// 	temp=22;								//������Чֵ��22��ԽСԽ����	
//// 	FT6336_WR_Reg(FT_ID_G_THGROUP,&temp,1);	//���ô�����Чֵ
//// 	temp=12;								//�������ڣ�����С��12�����4
//// 	FT6336_WR_Reg(FT_ID_G_PERIODACTIVE,&temp,1); 
	FT6336_GetChipInfo(ft6336_info);
/******************************************************/
}
const uint16_t FT6336_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};
TouchPointRefTypeDef TPR_Structure; 
void FT6336_Scan(void)
{
	uint8_t i=0;
	uint8_t sta = 0;
	uint8_t buf[4] = {0};    
	FT6336_RD_Reg(0x02,&sta,1);//��ȡ�������״� 	   
 	if(sta & 0x0f)	//�ж��Ƿ��д����㰴�£�0x02�Ĵ����ĵ�4λ��ʾ��Ч�������
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))����ĸ���ת��Ϊ�����㰴����Ч���
 		for(i=0;i<5;i++)	                                //�ֱ��жϴ�����1-5�Ƿ񱻰���
 		{
 			if(TPR_Structure.TouchSta & (1<<i))			    //��ȡ����������
 			{											    //���������ȡ��Ӧ��������������
 				FT6336_RD_Reg(FT6336_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
				TPR_Structure.x[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1];
				TPR_Structure.y[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
 				if((buf[0]&0XC0)!=0X80)
 				{
					TPR_Structure.x[i]=TPR_Structure.y[i]=0;//������contact�¼�������Ϊ��Ч	
					return;
				}
 			}
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;     //�������±��
                printf("x:%d   y:%d\r\n",240-TPR_Structure.x[0],240-TPR_Structure.y[0]);
 	}
 	else
 	{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN) 	//֮ǰ�Ǳ����µ�
 			TPR_Structure.TouchSta &= ~0x80;        //�����ɿ����
 		else
 		{
 			TPR_Structure.x[0] = 0;
 			TPR_Structure.y[0] = 0;
 			TPR_Structure.TouchSta &=0xe0;	//�����������Ч���
 		}
 	}
}

void FT6336_GetChipInfo(uint8_t *info)
{
//	


	FT6336_RD_Reg(0x00,info,100);
}

void touch_state(void)
{
	if(gpio_get_level(FT6336_INT_PIN)==0)
	{
		TPR_Structure.TouchSta |= TP_COORD_UD;	
	}	 
}


bool state;
bool FT6336_read(lv_indev_data_t *data)
{
 
	static int16_t last_x = 0;
    static int16_t last_y = 0;
 
	
	if(TPR_Structure.TouchSta &TP_COORD_UD)		//�����а���
	{
		TPR_Structure.TouchSta &= ~TP_COORD_UD;	//����
		FT6336_Scan();							//��ȡ��������
		data->point.x = 240-TPR_Structure.x[0];
		data->point.y = 240-TPR_Structure.y[0];

		last_x = data->point.x;
                last_y = data->point.y;
		data->state = LV_INDEV_STATE_PR;
                printf("x:%d   y:%d\r\n",last_x,last_y);

	}
	else
	{
		data->point.x = last_x; 
		data->point.y = last_y; 
		data->state = LV_INDEV_STATE_REL; 

	}
 
 
    return false;
}




