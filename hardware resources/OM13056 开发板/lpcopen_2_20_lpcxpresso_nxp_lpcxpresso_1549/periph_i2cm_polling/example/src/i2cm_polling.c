/*
 * @brief I2CM bus master example using polling mode
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* I2CM transfer record */
static I2CM_XFER_T  i2cmXferRec;
/* I2C clock is set to 1.8MHz */
#define I2C_CLK_DIVIDER         (40)
/* 100KHz I2C bit-rate */
#define I2C_BITRATE         (100000)
/* Standard I2C mode */
#define I2C_MODE    (0)

#if defined(BOARD_NXP_LPCXPRESSO_1549)
/** 7-bit I2C addresses of Temperature Sensor */
#define I2C_TEMP_ADDR_7BIT  (0x48)
#endif

#if defined(BOARD_KEIL_MCB1500)
/** 7-bit I2C address of STMPE811 Touch Screen Controller */
#define I2C_STMPE811_ADDR_7BIT 0x41
#endif

/* SysTick rate in Hz */
#define TICKRATE_HZ         (10)

/* Current state for LED control via I2C cases */
static volatile int state;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for I2C interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_I2C_PinMux(void)
{
#if defined(BOARD_KEIL_MCB1500)||defined(BOARD_NXP_LPCXPRESSO_1549)
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);
#else
#error "No I2C Pin Muxing defined for this example"
#endif
}

/* Setup I2C handle and parameters */
static void setupI2CMaster()
{
	/* Enable I2C clock and reset I2C peripheral - the boot ROM does not
	   do this */
	Chip_I2C_Init(LPC_I2C0);

	/* Setup clock rate for I2C */
	Chip_I2C_SetClockDiv(LPC_I2C0, I2C_CLK_DIVIDER);

	/* Setup I2CM transfer rate */
	Chip_I2CM_SetBusSpeed(LPC_I2C0, I2C_BITRATE);
	
	/* Enable Master Mode */
	Chip_I2CM_Enable(LPC_I2C0);
}

static void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize)
{
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cmXferRec);
}

/* Master I2CM receive in polling mode */
#if defined(BOARD_NXP_LPCXPRESSO_1549)
/* Function to read LM75 I2C temperature sensor and output result */
static void ReadTemperatureI2CM(void)
{
	uint8_t temperature[2];
	uint8_t lm75TempRegisterAddress = 0;

	/* Read LM75 temperature sensor */
	SetupXferRecAndExecute(

		/* The LM75 I2C bus address */
		I2C_TEMP_ADDR_7BIT, 

		/* Transmit one byte, the LM75 temp register address */
		&lm75TempRegisterAddress, 1, 

		/* Receive back two bytes, the contents of the temperature register */
		temperature, 2);

	/* Test for valid operation */
	if (i2cmXferRec.status == I2CM_STATUS_OK) {
		/* Output temperature. */
		DEBUGOUT("Temperature read over I2C is %d.%d degrees C.\r\n", 
			(int) temperature[0], (temperature[1] & 0x80) ? 5 : 0 );
	}
	else {
		DEBUGOUT("Error %d reading temperature.\r\n", i2cmXferRec.status );
	}
}
#endif

#if defined(BOARD_KEIL_MCB1500)
/* Function to read ID register of STMPE811 touch screen controller and output result */
static void ReadTouchID(void)
{
	uint8_t idAndRevision[3];
	uint8_t registerAddress = 0;

	/* Read STMPE811 CHIP_ID and ID_VER registers */
	SetupXferRecAndExecute(

		/* STMPE811 I2C bus address */
		I2C_STMPE811_ADDR_7BIT, 

		/* Transmit one byte, the register address */
		&registerAddress, 1, 

		/* Receive back three bytes, the contents of the CHIP_ID and ID_VER registers */
		idAndRevision, 3);

	/* Test for valid operation */
	if (i2cmXferRec.status == I2CM_STATUS_OK) {
		/* Output CHIP_ID and ID_VER values. */
		DEBUGOUT("STMPE811 CHIP_ID:0x%04x ID_VER:0x%02x.\r\n", 
			(int) (idAndRevision[0] << 8) | idAndRevision[1], idAndRevision[2] );
	}
	else {
		DEBUGOUT("Error %d reading CHIP_ID and ID_VER registers of STMPE811.\r\n", 
			i2cmXferRec.status );
	}
}
#endif

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	static int ticks = 0;

	ticks++;
	if (ticks > TICKRATE_HZ) {
		ticks = 0;
		state = 1 - state;
	}
}

/**
 * @brief	Main routine for I2CM example
 * @return	Function should not exit
 */
int main(void)
{
	int lastState = 0;
	state = 0;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Clear activity LED */
	Board_LED_Set(0, false);

	/* Setup I2C pin muxing */
	Init_I2C_PinMux();

	/* Allocate I2C handle, setup I2C rate, and initialize I2C
	   clocking */
	setupI2CMaster();

	/* Disable the interrupt for the I2C */
	NVIC_DisableIRQ(I2C0_IRQn);

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/* Loop forever, toggle LED on board */
	while (1) {

		/* Sleep until a state change occurs in SysTick */
		while (lastState == state) {
			__WFI();
		}

#if defined(BOARD_NXP_LPCXPRESSO_1549)
		/* Read Motor Control board's I2C temperature sensor and output result */
		ReadTemperatureI2CM();
#endif

#if defined(BOARD_KEIL_MCB1500)
		/* Read Touch Controller's ID & ver registers and output result */
		ReadTouchID();
#endif

		/* Reset lastState to allow for WFI */
		lastState = state;

		/* Toggle LED to show activity. */
		Board_LED_Toggle(0);
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
