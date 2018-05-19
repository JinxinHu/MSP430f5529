#include "msp430f5529.h"
#include "DS18B20.h"


void Input_DQ(void)
{
  P3DIR &= ~BIT0;
  P3REN |= BIT0;
  P3OUT |= BIT0;
}

//��λDS18B20
void DS18B20_Reset(void)
{
  Output_DQ;
  Reset_DQ;
  delay_us(750);	//��ʱ750us
  Set_DQ;
  delay_us(30);	        //��ʱ30us
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1��δ��⵽DS18B20
//����0������
uint8_t DS18B20_Check(void)
{
  uint8_t retry=0;
  uint32_t temp=0;
  Input_DQ();
  temp=Status_DQ;
  while (temp && retry<200)
  {
    temp=Status_DQ;
    retry++;
    delay_us(1);//��ʱ1us
  }
  if (retry>=200)
    return 1;
  else
    retry=0;
  temp=Status_DQ;
  while (!temp && retry<240)
  {
    temp=Status_DQ;
    retry++;
    delay_us(1);//��ʱ1us
  }
  if (retry>=240)
    return 1;
  return 0;
}

//��DS18B20��ȡһ��λ
//����ֵ��0/1
uint8_t DS18B20_Read_Bit(void)
{
  uint8_t data=0;
  uint32_t temp=0;
  Output_DQ;
  Reset_DQ;
  delay_us(2);//��ʱ2us
  Set_DQ;
  Input_DQ();
  delay_us(12);//��ʱ12us
  temp=Status_DQ;
  if (temp) data=1;
  else data=0;
  delay_us(50);//��ʱ50us
  return data;
}

//��д���Ǵӵ�λLSB��ʼ��
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
uint8_t DS18B20_Read_Byte(void)
{
  uint8_t i,j,data=0;
  for (i=1;i<=8;i++)
  {
    j=DS18B20_Read_Bit();
    data=(j<<7) | (data>>1);
    delay_us(1);//��ʱ1us���ָ�
  }
  return data; 
}

//дһ���ֽڵ�DS18B20
//data��Ҫд����ֽ�
void DS18B20_Write_Byte(uint8_t data)
{
  uint8_t j,temp=0;
  Output_DQ;
  for (j=1;j<=8;j++)
  {
    temp=data&0x01;
    data=data>>1;
    if (temp)
    {
      Reset_DQ;
      delay_us(2);//��ʱ2us
      Set_DQ;
      delay_us(60);//��ʱ60us
    }
    else
    {
      Reset_DQ;
      delay_us(60);//��ʱ60us
      Set_DQ;
      delay_us(2);//��ʱ2us
    }
    delay_us(1);//��ʱ1us���ָ�
  }
}

//��ʼ�¶�ת��
void DS18B20_Start_Convert(void)
{
  DS18B20_Reset();
  DS18B20_Check();
  DS18B20_Write_Byte(0xcc);
  DS18B20_Write_Byte(0x44);
}

//��ʼ��DS18B20��IO��DQͬʱ���DS18B20�Ĵ���
//DQΪPA6
//����1��δ��⵽DS18B20
//����0������
uint8_t DS18B20_Init(void)
{
  //Configure P4.6(DQ) as output for DS18B20
  //P4DIR |= BIT6;                            // Set P4.6(DQ) to output direction
  Output_DQ;
  Set_DQ;
  DS18B20_Reset();
  return DS18B20_Check();

}

//��DS18B20�õ��¶�ֵ
//����ֵ��ʵ���¶�ֵ*10��-550-1250��
float DS18B20_Get_Temp(void)
{
  uint8_t temp,TL,TH;
  float t;
  DS18B20_Start_Convert();
  DS18B20_Reset();
  DS18B20_Check();
  DS18B20_Write_Byte(0xcc);
  DS18B20_Write_Byte(0xbe);
  TL=DS18B20_Read_Byte();
  TH=DS18B20_Read_Byte();
  if (TH>7)
  {
    TL=TL-1;
    TH=~TH;
    TL=~TL;
    temp=0;
  }
  else temp=1;
  t=TH;
  t=t*256;
  t+=TL;
  t=t*0.625;
  if (temp) return t;
  else return -t;
}
