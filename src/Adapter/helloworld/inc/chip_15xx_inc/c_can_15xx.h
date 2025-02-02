/*
 * c_can0_15xx.h
 *
 *  Created on: 2016年12月15日
 *      Author: cheri
 */



/*@brief introduction to the c_can funtion of the LPC15xx
 *
 * Controller Area Network (CAN) is the definition of a high performance communication
protocol for serial data communication. The C_CAN controller is designed to provide a full
implementation of the CAN protocol according to the CAN Specification Version 2.0B. The
C_CAN controller allows to build powerful local networks with low-cost multiplex wiring by
supporting distributed real-time control with a very high level of security.
The CAN controller consists of a CAN core, message RAM, a message handler, control
registers, and the APB interface.
 *
 * For communication on a CAN network, individual Message Objects are configured. The
Message Objects and Identifier Masks for acceptance filtering of received messages are
stored in the Message RAM.
 *
 * All functions concerning the handling of messages are implemented in the Message
Handler. Those functions are the acceptance filtering, the transfer of messages between
the CAN Core and the Message RAM, and the handling of transmission requests as well
as the generation of the module interrupt.
The register set of the CAN controller can be accessed directly by an external CPU via the
APB bus. These registers are used to control/configure the CAN Core and the Message
Handler and to access the Message RAM.
 *
 */

#ifndef CHIP_15XX_INC_C_CAN_15XX_H_
#define CHIP_15XX_INC_C_CAN_15XX_H_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
	__IO uint32_t CNTL;      /*CAN control */
	__IO uint32_t STAT;      /*CAN control */
    __I  uint32_t EC;        /*CAN control */
    __IO uint32_t BT;        /*CAN control */
    __I  uint32_t INT;       /*CAN control */
    __IO uint32_t TEST;
    __IO uint32_t BRPE;
    __IO uint32_t RESERVED0;
    __IO uint32_t IF1_CMDREQ;

    union {
    	__O uint32_t IF1_CMDMSK_W;
    	__I uint32_t IF1_CMDMSK_R;
    };

    __IO uint32_t IF1_MSK1;
    __IO uint32_t IF1_MSK2;
    __IO uint32_t IF1_ARB1;
    __IO uint32_t IF1_ARB2;



}LPC_C_CAN_T;







#ifdef __cplusplus
}
#endif

#endif /* CHIP_15XX_INC_C_CAN_15XX_H_ */
