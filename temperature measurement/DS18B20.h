#include <stdint.h>

//DQ	P4.6

//����DQ
#define		Set_DQ		P4OUT |= BIT5
//����DQ
#define		Reset_DQ	P4OUT &= ~BIT5




//����DQΪ���
#define		Output_DQ	P4DIR |= BIT5
//DQ��ǰ״̬
#define		Status_DQ	(P4IN & BIT5)

//Delays
#define CPU_F                            ((double)25000000) 
#define delay_us(x)                      __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x)                      __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

void DS18B20_Reset(void);
uint8_t DS18B20_Check(void);
uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(uint8_t data);
void DS18B20_Start_Convert(void);
uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(void);
void Input_DQ(void);  //����DQΪ����