/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G14 V2.04.01.03 [30 Jan 2015]
* Device(s)    : R5F104AA
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 5/28/2015
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
#pragma interrupt INTTM03 r_tau0_channel3_interrupt
#pragma interrupt INTTRG r_tmr_rg0_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_serial.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */	
	/* compiler appears to be bugged so a lot of variables have to be declared global scope */

  	/* variables used in main routine */
	int16_t count = 0;	
	int16_t countLast = 0;			/* position at last ms */
	uint8_t bufferIn[5];  	/* if you change buffer length, be sure to change in uart receive function*/
	uint8_t bufferOut[5];
	uint8_t masterPacket[5] = {0,0,0,0,0};	/* currently executing verified command */
	extern volatile uint16_t  g_uart2_rx_count;            /* uart2 receive data number */
	int firstCall = 0;
	
	/* functions */
	void packet_process(void);
	void pid_control(void);
	void set_motor_output(long int dutyCycle);
	uint8_t crc(long remainder);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel3_interrupt
* Description  : This function is INTTM03 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel3_interrupt(void)
{
	static uint8_t start = 1;	/* changed to 0 after first cycle */
	static uint8_t isReceiving = 0;	/* have we received a partial packet */
	int i;				/* index for iterating through buffere elements */
	long packetStemIn;		/* first 4 bytes of packet received */
	
    /* Start user code. Do not edit comment generated here */
    	/* if this is the first cycle, start uart receive */
    	if (start == 1)
	{
		R_UART2_Receive(bufferIn, 5); 
		start = 0;
	}
    
	/* if timeout timer is running... */
	if (isReceiving == 1)
	{
		/* if we get our full packet, stop timer */
		if (g_uart2_rx_count == 5)
			isReceiving = 0;
		/* if a ms passes since countdown timer started, clear buffer */
		else 
		{
			R_UART2_Receive(bufferIn, 5);
			isReceiving = 0;
		}
	}
	
	/* if we just got our first byte(s) start timeout timer */
	else if (g_uart2_rx_count > 0 && g_uart2_rx_count < 5)
		isReceiving = 1;

	    
	/* if we just received a full packet... */
	if (g_uart2_rx_count == 5)
	{
		packetStemIn = (((long)bufferIn[0]) << 24) +
		(((long)bufferIn[1]) << 16) +
		(((long)bufferIn[2]) << 8) +
		((long) bufferIn[3]);
		
		/* if crc byte is correct, make master packet buffer packet */
		if ((long)bufferIn[4] == crc(packetStemIn))
		{
			for (i = 0; i<5; i++)
				masterPacket[i] = bufferIn[i];
			countLast = count;
			firstCall = 1;
		}
		
		/* clear buffer for future packets */
		R_UART2_Receive(bufferIn, 5);	
	}	
	
	/* get current angle and process current master packet */
	count = TRG;
	packet_process();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tmr_rg0_interrupt
* Description  : This function is INTTRG interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tmr_rg0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void packet_process(void)
{
	int pwm;	/* duty cycle for duty assignment mode */
	int mode;	/* assigned mode from master packet */
	
	/* if ignore bit set, do nothing */
	if (masterPacket[0] & 2)
		return;
	mode = masterPacket[0] & 12;
	/* servo off mode */
	if (mode == 0)
	{
		set_motor_output(0);
		TRG = 0;
		count = 0;
	}
	/* duty assignment mode */
	if (mode == 4)
	{
		pwm = (double)((int8_t)masterPacket[3]) / 100 * 3100;
		set_motor_output(pwm);
	
	}
	/* position control mode */
	if (mode == 8)
	{
		pid_control();
	}
	/* else reservation mode, don't need to do anything */
		
	/* if this is the time we are processing this packet, and if required send reply */
	if (firstCall == 1 && masterPacket[1] & 1 == 1)
	{
		firstCall = 0;
		bufferOut[0] = masterPacket[0];
		bufferOut[2] = count >> 8;
		bufferOut[3] = count;
		bufferOut[4] = crc((((long)bufferIn[0]) << 24) +
			(((long)bufferIn[1]) << 16) +
			(((long)bufferIn[2]) << 8) +
			((long) bufferIn[3]));*/
		
		P1.5 = 1;
		R_UART2_Send(bufferOut , 5);
		/* when send finishes, it will toggle DE for receive */
	}
}

/* if in position control mode, this function calculates the necessary output */
void pid_control(void)
{
	const double pGain = 530;	
	const double iGain = 0.5;
	const double dGain = 0.01;
	const int tau = 10;			/* time constand for tustin transform */
	const int integralMax = 1000;
	int16_t targetPos;	/* p_desired for position control */
	long derivative = 0;	/* derivative of count(t) */
	static long int integral = 0;		/* integral error */
	long output;
	
	/* calculate target position from input */
	targetPos = ((int16_t)masterPacket[2] << 8) + masterPacket[3];	

	/* calculate integral and derivatives */
	integral += (targetPos - count);
	if (integral > integralMax)
		integral = integralMax;
	if (integral < -integralMax)
		integral = -integralMax;
		
	/* derivative is found using tustin transform */
	derivative = (-1 + 2 * tau) / (1 + 2 * tau) * derivative +
		(2 / (1 + 2 * tau)) * count - (2 / (1 + 2 * tau)) * countLast;

	/* calculate output from pid gains */
	output = pGain * (targetPos - count) + integral * iGain - 
		derivative * dGain;
		
	set_motor_output(output);
}

/* function takes signed time constant as parameter and commands motors */
void set_motor_output(long int dutyCycle)
{
	int temp = dutyCycle;
	const int maxOutput = 3100; /* maximum time constant for pwm */
	
	if (dutyCycle > maxOutput || dutyCycle < -maxOutput)
		temp = maxOutput;
	else if (dutyCycle < 0)
		temp = -temp;
	if (dutyCycle > 0)
	{
		TDR01 = 0;
		TDR02 = temp;
	}
	else
	{
		TDR01 = temp;
		TDR02 = 0;
	}
}

/* function takes 4 byte packet stem and returns the CRC8-CCIT check byte */
uint8_t crc(long remainder)
{			
	const long polynomial = (long int)0x8D << 24;	/* polynomial for key calculation */
	
	/* For each bit position in the message */
	int b;
	for (b = 32; b > 0; b--)
	{
		
		/* If the uppermost bit is a 1 */	
		if (remainder & 0x80000000)
		{
			
			/* XOR the previous remainder with the divisor */
			remainder ^= polynomial;
		}

		
		/* Shift the next bit of the message into the remainder */
		remainder = (remainder << 1);
	}

	/* Return only the relevant bits of the remainder as CRC */
	remainder = (remainder >> 24);
	
	return remainder;	
}

/* End user code. Do not edit comment generated here */
