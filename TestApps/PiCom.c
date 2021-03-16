#include <wiringPiI2C.h>
#include <stdio.h>

/*
To enable ssh after burning system image, use "touch ssh" in the "boot" directory of the SD card. (not the "rootfs")

I2C enabled trough raspi-config and reboot is required after this step!

Git basic setup:
git config --global user.name "xyz"
git config --global user.email "xyz@xyz.com"
git config --global credential.helper cache
git pull "https:/..."

Installed packages over Raspbian:
qt4-dev-tools
xterm

Build command: "gcc -Wall -o PiCom PiCom.c -lwiringPi"

Output of "i2cdetect -l" when connecting sensor to Pi 1B:
i2c-1	i2c       	bcm2835 (i2c@7e804000)          	I2C adapter

Output of "i2cdetect -y 1" when connecting sensor to Pi 1B 512mb:
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- 23 -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --   

Output of code:
Current light: 16 Lux (0%) - Standard
Current light: 16 Lux (0%)
Current light: 15 Lux (0%)
Current light: 16 Lux (0%)
Current light: 83 Lux (0%)
Current light: 137 Lux (0%)
Current light: 1193 Lux (1%) - Flashlight
Current light: 2063 Lux (3%)
Current light: 115 Lux (0%)
Current light: 17 Lux (0%)
*/


int main (void) 
{
  int handle = wiringPiI2CSetup(0x23);
  while(1)
  {
    //wiringPiI2CWrite(handle, 0x10); // Continously measurement at 1 lx resolution. Measurement Time is typically 120ms.
    wiringPiI2CWrite(handle, 0x21);   // One-time measurement at 0.5 lx resolution. Measurement Time is typically 120ms. It is automatically set to Power Down mode after measurement.
    sleep(1);
    int word = wiringPiI2CReadReg16(handle, 0x00);
    int lux = ((word & 0xff00)>>8) | ((word & 0x00ff)<<8);
    int percent = (100.0/65535.0)*lux;
    printf("Current light: %d Lux (%d%)\n", lux, percent);
  }
  return 0;
}
