//Code based on https://github.com/desaster/ov7670fifotest

#include "OV7670_control.h"
#include "stm32f0xx.h"
#include "stdint.h"
#include "systick_rtc.h"
#include "OV7670reg.h"
#include "i2c.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"


uint8_t OV7670_Init(void)
{
    if(I2C_Read(OV7670_READ_ADDR,REG_PID) != 0x76)
    {
        return 0;
    }

    I2C_Write(OV7670_WRITE_ADDR,REG_COM7, 0x80); // reset to default values
    I2C_Write(OV7670_WRITE_ADDR,REG_CLKRC, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM11, 0x0A);
    I2C_Write(OV7670_WRITE_ADDR,REG_TSLB, 0x04);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM7, COM7_RGB); // output format: rgb

    I2C_Write(OV7670_WRITE_ADDR,REG_RGB444, 0x00); // disable RGB444
    I2C_Write(OV7670_WRITE_ADDR,REG_COM15, COM15_RGB565|0xC0); //RGB565 and full output data range
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM1, 0x0); //CCIR601
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM9, 0x30); // 16x gain ceiling; 0x8 is reserved bit

    I2C_Write(OV7670_WRITE_ADDR,REG_HSTART, 0x16);  //23 22
    I2C_Write(OV7670_WRITE_ADDR,REG_HSTOP, 0x04);   //24 4
    I2C_Write(OV7670_WRITE_ADDR,REG_HREF, 0x24);
    I2C_Write(OV7670_WRITE_ADDR,REG_VSTART, 0x02); //25 2
    I2C_Write(OV7670_WRITE_ADDR,REG_VSTOP, 0x7a);  //26 122
    I2C_Write(OV7670_WRITE_ADDR,REG_VREF, 0x0a);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM10, 0x02);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM3, 0x4);
    I2C_Write(OV7670_WRITE_ADDR,REG_MVFP, 0x3f);

    /*
    I2C_Write(OV7670_WRITE_ADDR,REG_COM7, 0x80); // reset to default values
    I2C_Write(OV7670_WRITE_ADDR,REG_CLKRC, 0x1);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM11, 0x0A);
    I2C_Write(OV7670_WRITE_ADDR,REG_TSLB, 0x04);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM7, COM7_RGB); // output format: rgb

    I2C_Write(OV7670_WRITE_ADDR,REG_HSTART, 0x13);  //23 22
    I2C_Write(OV7670_WRITE_ADDR,REG_HSTOP, 0x01);   //24 4
    I2C_Write(OV7670_WRITE_ADDR,REG_HREF, 0x24);
    I2C_Write(OV7670_WRITE_ADDR,REG_VSTART, 0x02); //25 2
    I2C_Write(OV7670_WRITE_ADDR,REG_VSTOP, 0x7a);  //26 122
    I2C_Write(OV7670_WRITE_ADDR,REG_VREF, 0x0a);
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM10, 0x02); //21
    I2C_Write(OV7670_WRITE_ADDR,REG_COM3, 0x4);
    I2C_Write(OV7670_WRITE_ADDR,REG_MVFP, 0x3f); //mirror/vflip

    I2C_Write(OV7670_WRITE_ADDR,REG_RGB444, 0x00); // disable RGB444
    I2C_Write(OV7670_WRITE_ADDR,REG_COM15, COM15_RGB565|0xC0); //RGB565 and full output data range
    */
    // 160x120
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM14, 0x1a); // divide by 4
    //I2C_Write(OV7670_WRITE_ADDR,0x72, 0x22); // //vertical and horizontal down sample by 4
    //I2C_Write(OV7670_WRITE_ADDR,0x73, 0xf2); // enable clock divider for DSP scale control, dived by 2

    // 320x240
    I2C_Write(OV7670_WRITE_ADDR,REG_COM14, 0x19); //divide by 2
    //I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_XSC, 0x3a);
    //I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_YSC, 0x35);
    I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_DCWCTR, 0x11); //vertical and horizontal down sample by 2
    I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_PCLK_DIV, 0xf1); //enable clock divider for DSP scale control, dived by 2
    //I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_PCLK_DELAY, 0x02);

    // Gamma curve values linux
    /*
    I2C_Write(OV7670_WRITE_ADDR, 0x7a, 0x20 );
    I2C_Write(OV7670_WRITE_ADDR, 0x7b, 0x10 );
    I2C_Write(OV7670_WRITE_ADDR, 0x7c, 0x1e );
    I2C_Write(OV7670_WRITE_ADDR, 0x7d, 0x35 );
    I2C_Write(OV7670_WRITE_ADDR, 0x7e, 0x5a );
    I2C_Write(OV7670_WRITE_ADDR, 0x7f, 0x69 );
    I2C_Write(OV7670_WRITE_ADDR, 0x80, 0x76 );
    I2C_Write(OV7670_WRITE_ADDR, 0x81, 0x80 );
    I2C_Write(OV7670_WRITE_ADDR, 0x82, 0x88 );
    I2C_Write(OV7670_WRITE_ADDR, 0x83, 0x8f );
    I2C_Write(OV7670_WRITE_ADDR, 0x84, 0x96 );
    I2C_Write(OV7670_WRITE_ADDR, 0x85, 0xa3 );
    I2C_Write(OV7670_WRITE_ADDR, 0x86, 0xaf );
    I2C_Write(OV7670_WRITE_ADDR, 0x87, 0xc4 );
    I2C_Write(OV7670_WRITE_ADDR, 0x88, 0xd7 );
    I2C_Write(OV7670_WRITE_ADDR, 0x89, 0xe8 );
    */
    // AGC and AEC parameters.  Note we start by disabling those features,
    //  then turn them only after tweaking the values. MAYBE doesn't affect color
    //

    //I2C_Write(OV7670_WRITE_ADDR,REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT);
    //I2C_Write(OV7670_WRITE_ADDR,REG_GAIN, 0);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AECH, 0);
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM4, 0x40); // magic reserved bit
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM9, 0x18); // 4x gain + magic rsvd bit
    //I2C_Write(OV7670_WRITE_ADDR,REG_BD50MAX, 0x05);
    //I2C_Write(OV7670_WRITE_ADDR,REG_BD60MAX, 0x07);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AEW, 0x95);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AEB, 0x33);
    //I2C_Write(OV7670_WRITE_ADDR,REG_VPT, 0xe3);
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC1, 0x78);
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC2, 0x68);
    //I2C_Write(OV7670_WRITE_ADDR,0xa1, 0x03); // magic
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC3, 0xd8);
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC4, 0xd8);
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC5, 0xf0);
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC6, 0x90);
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC7, 0x94);
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_BFILT|COM8_AGC|COM8_AEC);

    //8-bar color test pattern
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM17, 0x08); // turn on dsp color bar, 0x04 off. 66 4 decimal

    //Setting Camera Average Based AEC/AGC Registers

    //I2C_Write(OV7670_WRITE_ADDR,REG_AEW, 0X95);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AEB, 0X33);
    //I2C_Write(OV7670_WRITE_ADDR,REG_VPT, 0XE3);
    //I2C_Write(OV7670_WRITE_ADDR,REG_HAECC7, 0X00);

    //Setting Camera Histogram Based AEC/AGC Registers (automatic exposure control)
    I2C_Write(OV7670_WRITE_ADDR,REG_AEW, 0x95);
    I2C_Write(OV7670_WRITE_ADDR,REG_AEB, 0x33);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC1, 0x78);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC2, 0x68);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC3, 0xd8);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC4, 0xd8);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC5, 0xf0);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC6, 0x90);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC7, 0x94);

    //Setting Camera Advanced Auto White Balance Configs
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC1, AWBC1_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC2, AWBC2_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC3, AWBC3_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC4, AWBC4_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC5, AWBC5_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC6, AWBC6_VALUE);
    //Not in datasheet...
    //I2C_Write(OV7670_WRITE_ADDR,REG_AWBC7, AWBC7_VALUE);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AWBC8, AWBC8_VALUE);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AWBC9, AWBC9_VALUE);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AWBC10, AWBC10_VALUE);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AWBC11, AWBC11_VALUE);
    //I2C_Write(OV7670_WRITE_ADDR,REG_AWBC12, AWBC12_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBCTR3, AWBCTR3_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBCTR2, AWBCTR2_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBCTR1, AWBCTR1_VALUE);

    //adding this improve the color a little bit
    //Setting Camera Undocumented Registers
    I2C_Write(OV7670_WRITE_ADDR,0xB0, 0x84);

    // Set Camera Saturation
    I2C_Write(OV7670_WRITE_ADDR,REG_SATCTR, 0X60);

    // Denoise and Edge Enhancement
    I2C_Write(OV7670_WRITE_ADDR,REG_DNSTH, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x77, 0x00);

    I2C_Write(OV7670_WRITE_ADDR,REG_EDGE, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x75, 0x3);
    I2C_Write(OV7670_WRITE_ADDR,0x76, 0xe1);

    // Set up Camera Array Control
    I2C_Write(OV7670_WRITE_ADDR,REG_CHLF, 0x0b);
    I2C_Write(OV7670_WRITE_ADDR,REG_ARBLM, 0x11);

    // Set ADC Control
    I2C_Write(OV7670_WRITE_ADDR,REG_ADCCTR1, 0x02);
    I2C_Write(OV7670_WRITE_ADDR,REG_ADCCTR2, 0x91);
    I2C_Write(OV7670_WRITE_ADDR,REG_ADC, 0x1d);
    I2C_Write(OV7670_WRITE_ADDR,REG_ACOM, 0x71);
    I2C_Write(OV7670_WRITE_ADDR,REG_OFON, 0x2a);

    // Set Automatic Black Level Calibration
    I2C_Write(OV7670_WRITE_ADDR,ABLC1, ABLC1_VALUE);
    I2C_Write(OV7670_WRITE_ADDR,THL_ST, THL_ST_VALUE);

    // Set Maximum Gain
    I2C_Write(OV7670_WRITE_ADDR,REG_COM9, 0x10);
    // Set Blue Gain
    I2C_Write(OV7670_WRITE_ADDR,REG_BLUE, 0x40); //1
    // Set Red Gain
    I2C_Write(OV7670_WRITE_ADDR,REG_RED, 0x40); //2
    // Set Green Gain
    I2C_Write(OV7670_WRITE_ADDR,REG_GGAIN, 0x40); //106
    // Enable AWB Gain On
    I2C_Write(OV7670_WRITE_ADDR,REG_COM16, 0x08);

    //Custom stuff i messed with that helped
    //I2C_Write(OV7670_WRITE_ADDR,REG_RED, 0x4B); 75
    //I2C_Write(OV7670_WRITE_ADDR,REG_BLUE, 0x64); 100

    //old new is linux
    /*
    I2C_Write(OV7670_WRITE_ADDR,0x56, 0x40);
    I2C_Write(OV7670_WRITE_ADDR,0x58, 0x9e);
    I2C_Write(OV7670_WRITE_ADDR,0x59, 0x88);
    I2C_Write(OV7670_WRITE_ADDR,0x5a, 0x88);
    I2C_Write(OV7670_WRITE_ADDR,0x5b, 0x44);
    I2C_Write(OV7670_WRITE_ADDR,0x5c, 0x67);
    I2C_Write(OV7670_WRITE_ADDR,0x5d, 0x49);
    I2C_Write(OV7670_WRITE_ADDR,0x5e, 0x0e);
    I2C_Write(OV7670_WRITE_ADDR,0x69, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x6a, 0x40);
    I2C_Write(OV7670_WRITE_ADDR,0x6b, 0x0a);
    I2C_Write(OV7670_WRITE_ADDR,0x6c, 0x0a);
    I2C_Write(OV7670_WRITE_ADDR,0x6d, 0x55);
    I2C_Write(OV7670_WRITE_ADDR,0x6e, 0x11);
    I2C_Write(OV7670_WRITE_ADDR,0x6f, 0x9f);
    I2C_Write(OV7670_WRITE_ADDR,0xb0, 0x84);
    */

    // color conversion matrix
    I2C_Write(OV7670_WRITE_ADDR,0x4f, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,0x50, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,0x51, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x52, 0x22);
    I2C_Write(OV7670_WRITE_ADDR,0x53, 0x5e);
    I2C_Write(OV7670_WRITE_ADDR,0x54, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM13, 0xC2); //61 194/200 for color bar

    return 1;
}

uint8_t OV7670_Init2(void)
{
    if(I2C_Read(OV7670_READ_ADDR,REG_PID) != 0x76)
    {
        return 0;
    }
    I2C_Write(OV7670_WRITE_ADDR,REG_COM7, 0x80); // reset to default values
    I2C_Write(OV7670_WRITE_ADDR,REG_CLKRC, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,REG_TSLB, 0x04);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM7, 0x14);
    I2C_Write(OV7670_WRITE_ADDR,REG_RGB444, 0x00); // disable RGB444
    I2C_Write(OV7670_WRITE_ADDR,REG_COM15, COM15_RGB565|0xC0); //RGB565 and full output data range

    //I2C_Write(OV7670_WRITE_ADDR,REG_COM15, 0x10);
    //I2C_Write(OV7670_WRITE_ADDR,REG_COM7, 0x14);

    I2C_Write(OV7670_WRITE_ADDR,REG_HSTART, 0x16);
    I2C_Write(OV7670_WRITE_ADDR,REG_HSTOP, 0x04);//5
    I2C_Write(OV7670_WRITE_ADDR,REG_HREF, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,REG_VSTART, 0x02);
    I2C_Write(OV7670_WRITE_ADDR,REG_VSTOP, 0x7b);//0x7a,
    I2C_Write(OV7670_WRITE_ADDR,REG_VREF, 0x06);//0x0a.
    I2C_Write(OV7670_WRITE_ADDR,REG_COM10, 0x02);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM3, 0x4);
    I2C_Write(OV7670_WRITE_ADDR,REG_MVFP, 0x3f);

    /*
    I2C_Write(OV7670_WRITE_ADDR,REG_COM3, REG_COM3);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM14, 0x00);//10
    I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_XSC, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_YSC, 0x01);
    I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_DCWCTR, 0x11);
    I2C_Write(OV7670_WRITE_ADDR,REG_SCALING_PCLK_DIV, 0x09);
    */
    I2C_Write(OV7670_WRITE_ADDR,0xa2, 0x02);//15

    I2C_Write(OV7670_WRITE_ADDR,REG_SLOP, 0x20);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM1, 0x1c);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM2, 0x28);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM3, 0x3c);//20
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM4, 0x55);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM5, 0x68);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM6, 0x76);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM7, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM8, 0x88);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM9, 0x8f);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM10, 0x96);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM11, 0xa3);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM12, 0xaf);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM13, 0xc4);//30
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM14, 0xd7);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAM15, 0xe8);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM8, 0xe0);
    I2C_Write(OV7670_WRITE_ADDR,REG_GAIN, REG_GAIN);//AGC
    I2C_Write(OV7670_WRITE_ADDR,REG_AECH, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM4, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM9, 0x20);//0x38, limit the max gain
    I2C_Write(OV7670_WRITE_ADDR,REG_BD50MAX, 0x05);
    I2C_Write(OV7670_WRITE_ADDR,REG_BD60MAX, 0x07);
    I2C_Write(OV7670_WRITE_ADDR,REG_AEW, 0x75);//40
    I2C_Write(OV7670_WRITE_ADDR,REG_AEB, 0x63);
    I2C_Write(OV7670_WRITE_ADDR,REG_VPT, 0xA5);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC1, 0x78);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC2, 0x68);
    I2C_Write(OV7670_WRITE_ADDR,0xa1, 0x03);//0x0b,
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC3, 0xdf);//0xd8,
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC4, 0xdf);//0xd8,
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC5, 0xf0);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC6, 0x90);
    I2C_Write(OV7670_WRITE_ADDR,REG_HAECC7, 0x94);//50
    I2C_Write(OV7670_WRITE_ADDR,REG_COM8, 0xe5);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM5, 0x61);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM6, 0x4b);
    I2C_Write(OV7670_WRITE_ADDR,0x16, 0x02);
    I2C_Write(OV7670_WRITE_ADDR,REG_MVFP, 0x17);//0x07,
    I2C_Write(OV7670_WRITE_ADDR,REG_ADCCTR1, 0x02);
    I2C_Write(OV7670_WRITE_ADDR,REG_ADCCTR2, 0x91);
    I2C_Write(OV7670_WRITE_ADDR,0x29, 0x07);
    I2C_Write(OV7670_WRITE_ADDR,REG_CHLF, 0x0b);
    I2C_Write(OV7670_WRITE_ADDR,0x35, 0x0b);//60
    I2C_Write(OV7670_WRITE_ADDR,REG_ADC, 0x1d);
    I2C_Write(OV7670_WRITE_ADDR,REG_ACOM, 0x71);
    I2C_Write(OV7670_WRITE_ADDR,REG_OFON, 0x2a);
    I2C_Write(OV7670_WRITE_ADDR,REG_COM12, 0x78);
    I2C_Write(OV7670_WRITE_ADDR,0x4d, 0x40);
    I2C_Write(OV7670_WRITE_ADDR,0x4e, 0x20);
    I2C_Write(OV7670_WRITE_ADDR,REG_GFIX, 0x5d);

    I2C_Write(OV7670_WRITE_ADDR,0x6b, 0x80); //PLL, input clock x6
    I2C_Write(OV7670_WRITE_ADDR,0x74, 0x19);
    I2C_Write(OV7670_WRITE_ADDR,0x8d, 0x4f);
    I2C_Write(OV7670_WRITE_ADDR,0x8e, 0x00);//70
    I2C_Write(OV7670_WRITE_ADDR,0x8f, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x90, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x91, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x92, 0x00);//0x19,//0x66
    I2C_Write(OV7670_WRITE_ADDR,0x96, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,0x9a, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,0xb0, 0x84);
    I2C_Write(OV7670_WRITE_ADDR,0xb1, 0x0c);
    I2C_Write(OV7670_WRITE_ADDR,0xb2, 0x0e);

    I2C_Write(OV7670_WRITE_ADDR,THL_ST, 0x82);//80
    I2C_Write(OV7670_WRITE_ADDR,0xb8, 0x0a);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC1, 0x14);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC2, 0xf0);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC3, 0x34);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC4, 0x58);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC5, 0x28);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBC6, 0x3a);

    I2C_Write(OV7670_WRITE_ADDR,0x59, 0x88);
    I2C_Write(OV7670_WRITE_ADDR,0x5a, 0x88);
    I2C_Write(OV7670_WRITE_ADDR,0x5b, 0x44);//90
    I2C_Write(OV7670_WRITE_ADDR,0x5c, 0x67);
    I2C_Write(OV7670_WRITE_ADDR,0x5d, 0x49);
    I2C_Write(OV7670_WRITE_ADDR,0x5e, 0x0e);

    I2C_Write(OV7670_WRITE_ADDR,REG_LCC3, 0x04);
    I2C_Write(OV7670_WRITE_ADDR,REG_LCC4, 0x20);
    I2C_Write(OV7670_WRITE_ADDR,REG_LCC5, 0x05);
    I2C_Write(OV7670_WRITE_ADDR,0x94, 0x04);
    I2C_Write(OV7670_WRITE_ADDR,0x95, 0x08);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBCTR3, 0x0a);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBCTR2, 0x55);
    I2C_Write(OV7670_WRITE_ADDR,REG_MTX1, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,REG_MTX2, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,REG_MTX3, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,REG_MTX4, 0x22);
    I2C_Write(OV7670_WRITE_ADDR,REG_MTX5, 0x5e);
    I2C_Write(OV7670_WRITE_ADDR,REG_MTX6, 0x80);
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBCTR1, 0x11);//100
    I2C_Write(OV7670_WRITE_ADDR,REG_AWBCTR0, 0x9f);//0x9e for advance AWB
    I2C_Write(OV7670_WRITE_ADDR,REG_BRIGHT, 0x00);
    I2C_Write(OV7670_WRITE_ADDR,REG_CONTRAS, 0x40);
    I2C_Write(OV7670_WRITE_ADDR,REG_CONTRAS_CENTER, 0x80);//0x40,  change according to Jim's request


    return 1;
}

//Captures a frame, slow part of code. ~50ms Could see if camera can be clocked faster or use a faster xtal
void OV7670_capture(void)
{
    while ((GPIOB->IDR & VSYNC));     // wait for an old frame to end
    while (!(GPIOB->IDR & VSYNC));    // wait for a new frame to start
    FIFO_WR_Set;                     // enable writing to fifo
    while ((GPIOB->IDR & VSYNC));    // wait for the current frame to end
    FIFO_WR_Clear;                  // disable writing to fifo
    //FIFO_RRST_Set;
}

// Reset the al422 read pointer
void OV7670_rrst(void)
{
    FIFO_RRST_Clear;
    FIFO_RCLK_Set;
    FIFO_RRST_Set;
    FIFO_RCLK_Clear;
}

//Read one byte from the al422
uint8_t OV7670_ReadByte(void)
{
    uint8_t val;

    FIFO_RCLK_Set;
    val = GPIOB->IDR & CameraDataPort; //we only want 8 bits of data
    FIFO_RCLK_Clear;

    return val;
}

//Read 2 bytes from the al422
uint16_t OV7670_Read2Bytes(void)
{
    uint8_t val,val2=0;

    FIFO_RCLK_Set;
    val = GPIOB->IDR & CameraDataPort; //we only want 8 bits of data
    FIFO_RCLK_Clear;

    FIFO_RCLK_Set;
    val2 = GPIOB->IDR & CameraDataPort;
    FIFO_RCLK_Clear;
    return val2<<8|val;
}

//Reads an entire line of 320 pixels
//Note that 640 bytes gives us the 320 uint16_t, no bit shift!
//Runs at about 3MHz pin toggle
//2560/4= 4 lines
void OV7670_ReadLine(uint8_t* buff)
{
    //uint8_t val,val2=0;
    int H;

    for(H=0; H<2560; H++)
    {
        FIFO_RCLK_Set;
        //buff[H] = GPIOB->IDR & CameraDataPort; //we only want 8 bits of data
        buff[H] = GPIOB->IDR; //safe as long as we dont have the other pins as inputs
        FIFO_RCLK_Clear;
    }
}

void SetupCameraPins(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); // Enable GPIO port B

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); // Enable GPIO port A for testing

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //PB8 is for VSYNC which we need as an input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}
