/************************************************************************************
 * Author: Khooly																	*
 * Date: 19 March 2024																*
 * Version: 0.1																		*
 ***********************************************************************************/

#include <stdint.h>
#include "BIT_MATH.h"
#include "../06-STK/STK_module.h"
#include "SPI_interface.h"
#include "SPI_private.h"

/* static global array of function pointers to use in the set call back */
static void(*Glo_pfCallBacks[6])(void) = {0};

/**
 * @fn SPI_t SPI_pxPtrSelect*(uint8_t)
 * @brief Select the peripheral handler
 *
 * @param Copy_u8SPIx	Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @retval None
 */
static SPI_t* SPI_pxPtrSelect(uint8_t Copy_u8SPIx)
{
	SPI_t* Loc_pxSPI_t = nullptr;

	switch(Copy_u8SPIx)
	{
		case SPI1: Loc_pxSPI_t = (SPI_t*)SPI1_BASE_ADDRESS;		break;
		case SPI2: Loc_pxSPI_t = (SPI_t*)SPI2_BASE_ADDRESS;		break;
		case SPI3: Loc_pxSPI_t = (SPI_t*)SPI3_BASE_ADDRESS;		break;
		default: 												break;
	}

	return Loc_pxSPI_t;
}

/**
 * @fn void SPI1_vInit(uint8_t, bool, bool, bool, bool, bool, uint8_t, bool)
 *
 * @brief Initialize the SPI according to the specified parameters
 *
 * @param Copy_u8SPIx					Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @param Copy_boolMode					Specifies the SPI operating mode
 * This parameter can be a value of @ref SPI_Mode
 *
 * @param Copy_boolDataSize				Specifies the SPI data size
 * This parameter can be a value of @ref SPI_Data_Size
 *
 * @param Copy_boolCLKPolarity			Specifies the serial clock steady state
 * This parameter can be a value of @ref SPI_Clock_Polarity
 *
 * @param Copy_boolCLKPhase				Specifies the clock active edge for the bit capture
 * This parameter can be a value of @ref SPI_Clock_Phase
 *
 * @param Copy_boolSSM					Specifies whether the NSS signal is managed by hardware (NSS pin) or by software using the SSI bit
 * This parameter can be a value of @ref SPI_Slave_Select_management
 *
 * @param Copy_boolSSI					This bit has an effect only when the SSM is managed by software, The value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored
 * This parameter can be a value of @ref SPI_SSI_Internal_Slave_Select
 *
 * @param Copy_boolSSOE					This bit has an effect only when the SSM is managed by hardware, Specifies whether the NSS pin output is enabled (single slave) or disabled (multi slave)
 * This parameter can be a value of @ref SPI_SSOE_NSS_PIN_OUTPUT
 *
 * @param Copy_u8BaudRatePrescaler		Specifies the Baud Rate prescaler value which will be used to configure the transmit and receive SCK clock generation done by the master
 * This parameter can be a value of @ref SPI_BaudRate_Prescaler
 *
 * @param Copy_boolFirstBit				Specifies whether data transfers start from MSB or LSB bit
 * This parameter can be a value of @ref SPI_MSB_LSB_transmission
 *
 * @retval None
 */
void SPI_vInit(uint8_t Copy_u8SPIx, bool Copy_boolMode, bool Copy_boolDataSize, bool Copy_boolCLKPolarity, bool Copy_boolCLKPhase, bool Copy_boolSSM, bool Copy_boolSSI, bool Copy_boolSSOE, uint8_t Copy_u8BaudRatePrescaler, bool Copy_boolFirstBit)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if(Loc_pxSPI_t != nullptr)
	{
		Loc_pxSPI_t -> CR1.RegisterAccess  		= 0U;
		Loc_pxSPI_t -> CR1.BitAccess.CPHA  		= Copy_boolCLKPhase ;
		Loc_pxSPI_t -> CR1.BitAccess.CPOL  		= Copy_boolCLKPolarity;
		Loc_pxSPI_t -> CR1.BitAccess.MSTR  		= Copy_boolMode;
		Loc_pxSPI_t -> CR1.BitAccess.BR			= Copy_u8BaudRatePrescaler;
		Loc_pxSPI_t -> CR1.BitAccess.LSBFIRST	= Copy_boolFirstBit;
		Loc_pxSPI_t -> CR1.BitAccess.SSM 		= Copy_boolSSM;
		Loc_pxSPI_t -> CR1.BitAccess.SSI 		= Copy_boolSSI;
		Loc_pxSPI_t -> CR1.BitAccess.DFF 		= Copy_boolDataSize;
		Loc_pxSPI_t -> CR2.RegisterAccess		= 0U;
		Loc_pxSPI_t -> CR2.BitAccess.SSOE		= Copy_boolSSOE;
		Loc_pxSPI_t -> CR1.BitAccess.SPE 		= 1U;
	}
}

/**
 * @fn void SPI_vEnable(uint8_t)
 * @brief Enables a specified SPI peripheral
 *
 * @param Copy_u8SPIx	Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @retval None
 */
void SPI_vEnable(uint8_t Copy_u8SPIx)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if(Loc_pxSPI_t != nullptr)
	{
		Loc_pxSPI_t -> CR1.BitAccess.SPE = 1;
	}
}

/**
 * @fn void SPI_vDisable(u8)
 * @brief Disables a specified SPI peripheral
 *
 * @param Copy_u8SPIx	Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @retval None
 */
void SPI_vDisable(uint8_t Copy_u8SPIx)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if(Loc_pxSPI_t != nullptr)
	{
		Loc_pxSPI_t -> CR1.BitAccess.SPE = 0;
	}
}

/**
 * @fn void SPI_vTransmit(uint8_t, uint16_t*, uint16_t, uint32_t)
 * @brief Transmit an amount of data in blocking mode.
 *
 * @param Copy_u8SPIx			Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @param Copy_pu8Data			Pointer to transmission data buffer
 * @param Copy_u16ElementsNo	Amount of data elements to be transmitted, Elements No value start from one
 *
 * @param Copy_boolDataSize		Specifies the SPI data size
 * This parameter can be a value of @ref SPI_Data_Size
 *
 * @param Copy_u32Timeout		Timeout Timeout duration
 *
 * @retval None
 */
void SPI_vTransmit(uint8_t Copy_u8SPIx, uint8_t *Copy_pu8Data, uint16_t Copy_u16ElementsNo, bool Copy_boolDataSize, uint32_t Copy_u32Timeout)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if((Loc_pxSPI_t != nullptr) && (Copy_pu8Data != nullptr) && (Copy_u16ElementsNo != 0U))
	{
		/* Init tickstart for timeout management*/
		uint64_t Loc_u64tickstart = micros();

		/* Transmit data in 16 Bit mode */
		if(Copy_boolDataSize == SPI_DATASIZE_16BIT)
		{
			while (Copy_u16ElementsNo > 0U)
			{
				/* Wait until TXE flag is set to send data */
				if(Loc_pxSPI_t->SR.BitAccess.TXE)
				{
					Loc_pxSPI_t -> DR = *(uint16_t*)(Copy_pu8Data);
					Copy_pu8Data += 2;
					Copy_u16ElementsNo--;
				}
				else
				{
					/* Timeout management */
					if ((micros() - Loc_u64tickstart) >=  Copy_u32Timeout)
					{
						break;
					}
				}
			}
		}
		/* Transmit data in 8 Bit mode */
		else
		{
			while (Copy_u16ElementsNo > 0U)
			{
				/* Wait until TXE flag is set to send data */
				if(Loc_pxSPI_t->SR.BitAccess.TXE)
				{
					Loc_pxSPI_t -> DR = *Copy_pu8Data;
					Copy_pu8Data++;
					Copy_u16ElementsNo--;
				}
				else
				{
					/* Timeout management */
					if ((micros() - Loc_u64tickstart) >=  Copy_u32Timeout)
					{
						break;
					}
				}
			}
		}
	}
}

/**
 * @fn void SPI_vReceive(uint8_t, uint16_t*, uint16_t, bool, uint32_t)
 * @brief Receive an amount of data in blocking mode.
 *
 * @param Copy_u8SPIx			Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @param Copy_pu8Data			Pointer to reception data buffer
 * @param Copy_u16ElementsNo	Amount of data elements to be transmitted, Elements No value start from one
 *
 * @param Copy_boolDataSize		Specifies the SPI data size
 * This parameter can be a value of @ref SPI_Data_Size
 *
 * @param Copy_u32Timeout		Timeout Timeout duration
 *
 * @retval None
 */
void SPI_vReceive(uint8_t Copy_u8SPIx, uint8_t *Copy_pu8Data, uint16_t Copy_u16ElementsNo, bool Copy_boolDataSize, uint32_t Copy_u32Timeout)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if((Loc_pxSPI_t != nullptr) && (Copy_pu8Data != nullptr) && (Copy_u16ElementsNo != 0U))
	{
		if(Loc_pxSPI_t -> CR1.BitAccess.MSTR == SPI_MODE_MASTER)
		{
		    /* Call transmit-receive function to send Dummy data on Tx line and generate clock on CLK line */
		    return SPI_vTransmitReceive(Copy_u8SPIx, Copy_pu8Data, Copy_pu8Data, Copy_u16ElementsNo, Copy_boolDataSize, 1000000);
		}
		else
		{
			/* Init tickstart for timeout management*/
			uint64_t Loc_u64tickstart = micros();

			/* Receive data in 16 Bit mode */
			if(Copy_boolDataSize == SPI_DATASIZE_16BIT)
			{
				/* Transfer loop */
				while (Copy_u16ElementsNo > 0U)
				{
					/* Check the RXNE flag */
					if(Loc_pxSPI_t -> SR.BitAccess.RXNE)
					{
						*(uint16_t*)Copy_pu8Data = (uint16_t)Loc_pxSPI_t -> DR;
						Copy_pu8Data += 2;
						Copy_u16ElementsNo--;
					}
					else
					{
						/* Timeout management */
						if ((micros() - Loc_u64tickstart) >=  Copy_u32Timeout)
						{
							break;
						}
					}
				}
			}
			/* Receive data in 8 Bit mode */
			else
			{
				/* Transfer loop */
				while (Copy_u16ElementsNo > 0U)
				{
					/* Check the RXNE flag */
					if(Loc_pxSPI_t -> SR.BitAccess.RXNE)
					{
						*Copy_pu8Data = (uint8_t)Loc_pxSPI_t -> DR;
						Copy_pu8Data++;
						Copy_u16ElementsNo--;
					}
					else
					{
						/* Timeout management */
						if ((micros() - Loc_u64tickstart) >=  Copy_u32Timeout)
						{
							break;
						}
					}
				}
			}
		}
	}
}

/**
 * @fn void SPI_vTransmitReceive(uint8_t, uint16_t*, uint16_t*, uint16_t, bool, uint32_t)
 * @brief Transmit and Receive an amount of data in blocking mode.
 *
 * @param Copy_u8SPIx					Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @param Copy_pu8TxData				Pointer to transmission data buffer
 * @param Copy_pu8RxData				Pointer to reception data buffer
 * @param Copy_u16ElementsNo			Amount of data elements to be transmitted, Elements No value start from one
 *
 * @param Copy_boolDataSize				Specifies the SPI data size
 * This parameter can be a value of @ref SPI_Data_Size
 *
 * @param Copy_u32Timeout				Timeout Timeout duration
 *
 * @retval None
 */
void SPI_vTransmitReceive(uint8_t Copy_u8SPIx, uint8_t *Copy_pu8TxData, uint8_t *Copy_pu8RxData ,uint16_t Copy_u16ElementsNo, bool Copy_boolDataSize, uint32_t Copy_u32Timeout)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if((Loc_pxSPI_t != nullptr) && (Copy_pu8TxData != nullptr) && (Copy_pu8RxData != nullptr) && (Copy_u16ElementsNo != 0U))
	{
		uint16_t Loc_u16TxSize, Loc_u16RxSize;

		Loc_u16TxSize = Loc_u16RxSize = Copy_u16ElementsNo;

		bool txallowed = true;

		/* Init tickstart for timeout management*/
		uint64_t Loc_u64tickstart = micros();

		/* Transmit and receive data in 16 Bit mode */
		if(Copy_boolDataSize == SPI_DATASIZE_16BIT)
		{
			while ((Loc_u16TxSize > 0U) || (Loc_u16RxSize > 0U))
			{
				/* Check TXE flag */
				if ((Loc_pxSPI_t -> SR.BitAccess.TXE) && (Loc_u16TxSize > 0U) && (txallowed == true))
				{
					Loc_pxSPI_t -> DR = *(uint16_t*)(Copy_pu8TxData);
					Copy_pu8TxData += 2;
					Loc_u16TxSize--;
					/* Next Data is a reception (Rx). Tx not allowed */
					txallowed = false;
				}

				/* Check RXNE flag */
				if ((Loc_pxSPI_t -> SR.BitAccess.RXNE) && (Loc_u16RxSize > 0U))
				{
					*(uint16_t*)(Copy_pu8RxData) = (uint16_t)Loc_pxSPI_t -> DR;
					Copy_pu8RxData += 2;
					Loc_u16RxSize--;
					/* Next Data is a Transmission (Tx). Tx is allowed */
					txallowed = true;
				}

				/* Timeout management */
				if ((micros() - Loc_u64tickstart) >=  Copy_u32Timeout)
				{
					break;
				}
			}
		}
		/* Transmit and receive data in 8 Bit mode */
		else
		{
		    while ((Loc_u16TxSize > 0U) || (Loc_u16RxSize > 0U))
			{
				/* Check TXE flag */
				if ((Loc_pxSPI_t -> SR.BitAccess.TXE) && (Loc_u16TxSize > 0U) && (txallowed == true))
				{
					Loc_pxSPI_t -> DR = *Copy_pu8TxData;
					Copy_pu8TxData++;
					Loc_u16TxSize--;
					/* Next Data is a reception (Rx). Tx not allowed */
					txallowed = false;
				}

				/* Check RXNE flag */
				if ((Loc_pxSPI_t -> SR.BitAccess.RXNE) && (Loc_u16RxSize > 0U))
				{
					*Copy_pu8RxData = (uint8_t)Loc_pxSPI_t -> DR;
					Copy_pu8RxData++;
					Loc_u16RxSize--;
					/* Next Data is a Transmission (Tx). Tx is allowed */
					txallowed = true;
				}

				/* Timeout management */
				if ((micros() - Loc_u64tickstart) >=  Copy_u32Timeout)
				{
					break;
				}
			}
		}
	}
}

/**
 * @brief  Enable the specified SPI interrupts.
 * @param  Copy_u8SPIx			Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @param  Copy_u8Interrupt 	Specifies the interrupt source to enable.
 * This parameter can be a value of @ref SPI_Interrupt_definition
 *
 * @retval None
 */
void SPI_ENABLE_IT(uint8_t Copy_u8SPIx, uint8_t Copy_u8Interrupt)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if(Loc_pxSPI_t != nullptr)
	{
		Loc_pxSPI_t->CR2.RegisterAccess |= 1 << Copy_u8Interrupt;
	}
}

/**
 * @brief  Disable the specified SPI interrupts.
 * @param  Copy_u8SPIx			Specifies which SPI handler to use
 * This parameter can be a value of @ref SPIx
 *
 * @param  Copy_u8Interrupt 	Specifies the interrupt source to disable.
 * This parameter can be a value of @ref SPI_Interrupt_definition
 *
 * @retval None
 */
void SPI_DISABLE_IT(uint8_t Copy_u8SPIx, uint8_t Copy_u8Interrupt)
{
	SPI_t* Loc_pxSPI_t = SPI_pxPtrSelect(Copy_u8SPIx);

	if(Loc_pxSPI_t != nullptr)
	{
		Loc_pxSPI_t->CR2.RegisterAccess &= ~(1 << Copy_u8Interrupt);
	}
}

/**
 * @fn void SPI_SetTxCallback(uint8_t, void(*)(void))
 * @brief
 *
 * @param Copy_u8SPIx
 * This parameter can be a value of @ref SPIx
 *
 * @param Copy_pfCallBackFunc	Specifies the function to be called when the SPI_IT_TXE interrupt happen.
 *
 * @retval None
 */
void SPI_SetTxCallback(uint8_t Copy_u8SPIx, void(*Copy_pfCallBackFunc)(void))
{
	if(Copy_pfCallBackFunc != nullptr)
	{
		switch(Copy_u8SPIx)
		{
			case 	SPI1:	Glo_pfCallBacks[0] = Copy_pfCallBackFunc;	break;
			case 	SPI2:	Glo_pfCallBacks[2] = Copy_pfCallBackFunc;	break;
			case 	SPI3:	Glo_pfCallBacks[4] = Copy_pfCallBackFunc;	break;
			default  	:												break;
		}
	}
}

/**
 * @fn void SPI_SetRxCallback(uint8_t, void(*)(void))
 * @brief
 *
 * @param Copy_u8SPIx
 * This parameter can be a value of @ref SPIx
 *
 * @param Copy_pfCallBackFunc	Specifies the function to be called when the SPI_IT_RXNE interrupt happen.
 *
 * @retval None
 */
void SPI_SetRxCallback(uint8_t Copy_u8SPIx, void(*Copy_pfCallBackFunc)(void))
{
	if(Copy_pfCallBackFunc != nullptr)
	{
		switch(Copy_u8SPIx)
		{
			case 	SPI1:	Glo_pfCallBacks[1] = Copy_pfCallBackFunc;	break;
			case 	SPI2:	Glo_pfCallBacks[3] = Copy_pfCallBackFunc;	break;
			case 	SPI3:	Glo_pfCallBacks[5] = Copy_pfCallBackFunc;	break;
			default  	:												break;
		}
	}
}

extern "C"{
void SPI1_IRQHandler(void)
{
	/* SPI in mode Transmitter ----------------------------------------------------------*/
	if((((SPI_t*)SPI1_BASE_ADDRESS)->SR.BitAccess.TXE != 0) && (Glo_pfCallBacks[0] != 0))
	{
		Glo_pfCallBacks[0]();
	}
	/* SPI in mode Receiver -------------------------------------------------------------------*/
	else if((((SPI_t*)SPI1_BASE_ADDRESS)->SR.BitAccess.RXNE != 0) && (Glo_pfCallBacks[1] != 0))
	{
		Glo_pfCallBacks[1]();
	}
	else{

	}
	return;
}

void SPI2_IRQHandler(void)
{
	/* SPI in mode Transmitter ---------------------------------------------------------*/
	if((((SPI_t*)SPI2_BASE_ADDRESS)->SR.BitAccess.TXE != 0) && (Glo_pfCallBacks[2] != 0))
	{
		Glo_pfCallBacks[2]();
	}
	/* SPI in mode Receiver ------------------------------------------------------------------*/
	else if((((SPI_t*)SPI2_BASE_ADDRESS)->SR.BitAccess.RXNE != 0) && (Glo_pfCallBacks[3] != 0))
	{
		Glo_pfCallBacks[3]();
	}
	else{

	}
	return;
}

void SPI3_IRQHandler(void)
{
	/* SPI in mode Transmitter ----------------------------------------------------------*/
	if((((SPI_t*)SPI3_BASE_ADDRESS)->SR.BitAccess.TXE != 0) && (Glo_pfCallBacks[4] != 0))
	{
		Glo_pfCallBacks[4]();
	}
	/* SPI in mode Receiver ------------------------------------------------------------------*/
	else if((((SPI_t*)SPI3_BASE_ADDRESS)->SR.BitAccess.RXNE != 0) && (Glo_pfCallBacks[5] != 0))
	{
		Glo_pfCallBacks[5]();
	}
	else{

	}
	return;
}}
