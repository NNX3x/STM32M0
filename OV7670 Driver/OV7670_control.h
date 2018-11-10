// Includes
#include <stdint.h>

// OV7670 camera settings
#define OV7670_REG_NUM 			121
#define OV7670_WRITE_ADDR 	0x42
#define OV7670_READ_ADDR    0x43

// Image settings
#define IMG_ROWS   			144
#define IMG_COLUMNS   		174

#define FIFO_WR_Set 		GPIOB->BSRR=GPIO_BSRR_BS_10; //set
#define FIFO_WR_Clear		GPIOB->BRR=GPIO_BRR_BR_10;//clear

#define FIFO_RRST_Set 		GPIOB->BSRR=GPIO_BSRR_BS_9; //set
#define FIFO_RRST_Clear		GPIOB->BRR=GPIO_BRR_BR_9;//clear

#define FIFO_RCLK_Set 		GPIOB->BSRR=GPIO_BSRR_BS_11; //set
#define FIFO_RCLK_Clear		GPIOB->BRR=GPIO_BRR_BR_11;//clear

#define VSYNC 0x100         //PB8
#define CameraDataPort 0xFF //PB0-PB7

uint8_t OV7670_ReadByte(void);
uint16_t OV7670_Read2Bytes(void);
void OV7670_rrst(void);
void OV7670_capture(void);
void SetupCameraPins(void);
uint8_t OV7670_Init(void);
void OV7670_ReadLine(uint8_t* buff);
uint8_t OV7670_Init2(void);





