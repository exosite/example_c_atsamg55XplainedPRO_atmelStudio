/**
 * \file
 *
 * \brief ATSHA204 CryptoAuth driver configuration file
 *
 *
 * Copyright (c) 2011-2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#ifndef CONF_ATECC508_H_INCLUDED
#define CONF_ATECC508_H_INCLUDED

#include <board.h>

#define CONF_ECC508_USE_I2C					(1)
#define CONF_ECC508_USE_SWI					(0)
#define CONF_ECC508_USE_SPI					(0)
#define CONF_ECC508_USE_UART				(0)
#define CONF_ECC508_USE_KIT_HID				(0)
#define CONF_ECC508_USE_KIT_CDC				(0)

#define CONF_ECC508_I2C_MODULE		  EXT2_I2C_MODULE //2 // 1 -  R21, 0 - SAMD21G18A, 2 for others
#define CONF_ECC508_I2C_PINMUX_PAD0   EXT2_I2C_SERCOM_PINMUX_PAD0;
#define CONF_ECC508_I2C_PINMUX_PAD1   EXT2_I2C_SERCOM_PINMUX_PAD1;
#define CONF_ECC508_I2C_BAUD_RATE	  400000

//#define ATCA_HAL_I2C
//#define ATCA_HAL_SWI
//#define ATCA_HAL_SPI
//#define ATCA_HAL_UART
//#define ATCA_HAL_KIT_HID
//#define ATCA_HAL_KIT_CDC

//i2c_buses[0] = CONF_ECC508_I2C_MODULE;
//#ifdef __SAMR21G18A__
////	#define CONF_ECC508_I2C_MODULE				(1) // 1 -  R21, 0 - SAMD21G18A, 2 for others
	//#define CONF_ECC508_I2C_PINMUX_PAD0   PINMUX_PA16C_SERCOM1_PAD0;
	//#define CONF_ECC508_I2C_PINMUX_PAD1   PINMUX_PA17C_SERCOM1_PAD1;
	//#define CONF_ECC508_I2C_BAUD_RATE	  400000
//#elif defined __SAMD21G18A__
////	#define CONF_ECC508_I2C_MODULE				(0) // 1 -  R21, 0 - SAMD21G18A, 2 for others
	//#define CONF_ECC508_I2C_PINMUX_PAD0   EXT1_I2C_SERCOM_PINMUX_PAD0;
	//#define CONF_ECC508_I2C_PINMUX_PAD1   EXT1_I2C_SERCOM_PINMUX_PAD1;
	//#define CONF_ECC508_I2C_BAUD_RATE	  400000
//#else
////	#define CONF_ECC508_I2C_MODULE				(2) // 1 -  R21, 0 - SAMD21G18A, 2 for others
	//#define CONF_ECC508_I2C_PINMUX_PAD0   EXT1_I2C_SERCOM_PINMUX_PAD0;
	//#define CONF_ECC508_I2C_PINMUX_PAD1   EXT1_I2C_SERCOM_PINMUX_PAD1;
	//#define CONF_ECC508_I2C_BAUD_RATE	  400000
//#endif



// #ifdef __SAMR21G18A__
			// #define CONF_ECC508_I2C_PINMUX_PAD0  PINMUX_PA16C_SERCOM1_PAD0
			// #define CONF_ECC508_I2C_PINMUX_PAD1	 PINMUX_PA17C_SERCOM1_PAD1
			// #define CONF_ECC508_I2C_BAUD_RATE	  400000
// #elif defined __SAMD21G18A__
			// config_i2c_master.buffer_timeout = 10000;
			// #define CONF_ECC508_I2C_PINMUX_PAD0   EXT1_I2C_SERCOM_PINMUX_PAD0;
			// #define CONF_ECC508_I2C_PINMUX_PAD1   EXT1_I2C_SERCOM_PINMUX_PAD1;
			// #define CONF_ECC508_I2C_BAUD_RATE	  400000
// #else
			// config_i2c_master.baud_rate = cfg->atcai2c.baud / 1000;
// #endif


// #define CONF_WINC_SPI_MODULE			SERCOM2
// #define CONF_WINC_SPI_SERCOM_MUX		SPI_SIGNAL_MUX_SETTING_D
// #define CONF_WINC_SPI_PINMUX_PAD0		PINMUX_PA12C_SERCOM2_PAD0 /* out */
// #define CONF_WINC_SPI_PINMUX_PAD1		PINMUX_PA13C_SERCOM2_PAD1 /* sck  */
// #define CONF_WINC_SPI_PINMUX_PAD2		PINMUX_UNUSED /* cs driven from software */
// #define CONF_WINC_SPI_PINMUX_PAD3		PINMUX_PA15C_SERCOM2_PAD3 /* in  */
// #define CONF_WINC_SPI_CS_PIN			PIN_PA14



#endif /* CONF_ATECC508_H_INCLUDED */
