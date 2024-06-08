/************************************************************************************
 * Author: Khooly																	*
 * Date: 19 March 2024																*
 * Version: 0.1																		*
 ***********************************************************************************/

#ifndef SPI_INTERFACE_H
#define SPI_INTERFACE_H

/*************************************************************************************************************
									Single Master Single Slave Application									 *
																											 *
				  MASTER															SLAVE                    *
	     -----------------------			|MISO          MISO|           -----------------------           *
*---<---| 8 bits shift register |<----------o--------<---------o----<-----| 8 bits shift register |<-------* *
|	     -----------------------			|				   |	       -----------------------	       | *
|					 ^						|MOSI          MOSI|      		      	^ 	         		   | *
*--->--------->----  |  ---->---------->----o-------->---------o---->-------->----  |  ----------->--------* *
 	 	 	 	 	 ^	 	 	 	 	 	| 				   |			        ^                        *
 	 	  -----------|-----------			|SCK            SCK|			        |                        *
	 	 |  SPI Clock Generator  |----->----o-------->---------o-------->-----------*                        *
 	 	  -----------------------			|				   |                                             *
 	 	  	  	  	  	  	  	  	  	  	|NSS            NSS|                                             *
								        ----o	               o----                                         *
									   | Not used if NSS Managed by |                                        *
									   |   	     Software	        |                                        *
									  VDD                          GND                                       *
*************************************************************************************************************/
/************************************************************************************************************
*      DataSize = SPI_DATASIZE_8BIT:                                                                        *
*      +----------------------------------------------------------------------------------------------+     *
*      |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |     *
*      | Process | Transfer mode  |---------------------|----------------------|----------------------|     *
*      |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |     *
*      |==============================================================================================|     *
*      |         |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |     *
*      | TX / RX |----------------|----------|----------|-----------|----------|-----------|----------|     *
*      |         |     Interrupt  | Fpclk/4  | Fpclk/8  |    NA     |    NA    |    NA     |   NA     |     *
*      |=========|================|==========|==========|===========|==========|===========|==========|     *
*      |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |     *
*      |   RX    |----------------|----------|----------|-----------|----------|-----------|----------|     *
*      |         |     Interrupt  | Fpclk/8  | Fpclk/8  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |     *
*      |=========|================|==========|==========|===========|==========|===========|==========|     *
*      |         |     Polling    | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |     *
*      |   TX    |----------------|----------|----------|-----------|----------|-----------|----------|     *
*      |         |     Interrupt  | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |     *
*      +----------------------------------------------------------------------------------------------+     *
*                                                                                                           *
*      DataSize = SPI_DATASIZE_16BIT:                                                                       *
*      +----------------------------------------------------------------------------------------------+     *
*      |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |     *
*      | Process | Transfer mode  |---------------------|----------------------|----------------------|     *
*      |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |     *
*      |==============================================================================================|     *
*      |         |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |     *
*      | TX / RX |----------------|----------|----------|-----------|----------|-----------|----------|     *
*      |         |     Interrupt  | Fpclk/4  | Fpclk/4  |    NA     |    NA    |    NA     |   NA     |     *
*      |=========|================|==========|==========|===========|==========|===========|==========|     *
*      |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/32  | Fpclk/2  |     *
*      |   RX    |----------------|----------|----------|-----------|----------|-----------|----------|     *
*      |         |     Interrupt  | Fpclk/4  | Fpclk/4  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |     *
*      |=========|================|==========|==========|===========|==========|===========|==========|     *
*      |         |     Polling    | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/32 |     *
*      |   TX    |----------------|----------|----------|-----------|----------|-----------|----------|     *
*      |         |     Interrupt  | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |     *
*      +----------------------------------------------------------------------------------------------+     *
************************************************************************************************************/
/************************************************* DEFINES *************************************************/
/** @defgroup SPI_Data_Size SPI Data Size
  * @{
  */
#define SPI_DATASIZE_8BIT						0U
#define SPI_DATASIZE_16BIT						1U
/**
  * @}
  */

/** @defgroup SPI_Slave_Select_management SPI Slave Select Management
  * @{
  */
#define SPI_SSM_HW_MANAGE						0U
#define SPI_SSM_SW_MANAGE         				1U
/**
  * @}
  */

/** @defgroup SPI_MSB_LSB_transmission SPI MSB LSB Transmission
  * @{
  */
#define SPI_FIRSTBIT_MSB                		0U
#define SPI_FIRSTBIT_LSB                		1U
/**
  * @}
  */

/** @defgroup SPI_BaudRate_Prescaler SPI BaudRate Prescaler
  * @{
  */
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_2          0U
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_4 			1U
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_8          2U
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_16         3U
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_32         4U
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_64         5U
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_128 		6U
#define SPI_CLOCK_RATE_FREQ_DIVID_BY_256 		7U
/**
  * @}
  */

/** @defgroup SPI_Mode SPI Mode
  * @{
  */
#define SPI_MODE_SLAVE                  		0U
#define SPI_MODE_MASTER                 		1U
/**
  * @}
  */

/** @defgroup SPIx SPIx
  * @{
  */
#define SPI1                  					1U
#define SPI2                  					2U
#define SPI3                  					3U
/**
  * @}
  */

/** @defgroup SPI_Clock_Polarity SPI Clock Polarity
  * @{
  */
#define SPI_POLARITY_LOW                		0U
#define SPI_POLARITY_HIGH               		1U
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase SPI Clock Phase
  * @{
  */
#define SPI_PHASE_1EDGE                 		0U
#define SPI_PHASE_2EDGE                 		1U
/**
  * @}
  */

/** @defgroup SPI_Interrupt_definition SPI Interrupt Definition
  * @{
  */
#define SPI_IT_TXE                     			7U
#define SPI_IT_RXNE                    			6U
#define SPI_IT_ERR                     			5U
/**
  * @}
  */

/** @defgroup SPI_SSOE_NSS_PIN_OUTPUT SPI SSOE NSS PIN OUTPUT
  * @{
  */
#define SPI_SSOE_OUTPUT_DIS               		0U
#define SPI_SSOE_OUTPUT_EN                		1U
/**
  * @}
  */

/** @defgroup SPI_SSI_Internal_Slave_Select SPI SSI Internal Slave Select
  * @{
  */
#define SPI_SSI_LOW		                		0U
#define SPI_SSI_HIGH	                		1U
/**
  * @}
  */
/***********************************************************************************************************/
/************************************************* PROTOTYPES **********************************************/
void SPI_vInit(uint8_t Copy_u8SPIx, bool Copy_boolMode, bool Copy_boolDataSize, bool Copy_boolCLKPolarity,
		bool Copy_boolCLKPhase, bool Copy_boolSSM, bool Copy_boolSSI, bool Copy_boolSSOE,
		uint8_t Copy_u8BaudRatePrescaler, bool Copy_boolFirstBit);
void SPI_vEnable(uint8_t Copy_u8SPIx);
void SPI_vDisable(uint8_t Copy_u8SPIx);
void SPI_vTransmit(uint8_t Copy_u8SPIx, uint8_t *Copy_pu8Data, uint16_t Copy_u16ElementsNo, bool Copy_boolDataSize, uint32_t Copy_u32Timeout);
void SPI_vReceive(uint8_t Copy_u8SPIx, uint8_t *Copy_pu8Data, uint16_t Copy_u16ElementsNo, bool Copy_boolDataSize, uint32_t Copy_u32Timeout);
void SPI_vTransmitReceive(uint8_t Copy_u8SPIx, uint8_t *Copy_pu8TxData, uint8_t *Copy_pu8RxData ,uint16_t Copy_u16ElementsNo, bool Copy_boolDataSize, uint32_t Copy_u32Timeout);
void SPI_ENABLE_IT(uint8_t Copy_u8SPIx, uint8_t Copy_u8Interrupt);
void SPI_DISABLE_IT(uint8_t Copy_u8SPIx, uint8_t Copy_u8Interrupt);
void SPI_SetTxCallback(uint8_t Copy_u8SPIx, void(*Copy_pfCallBackFunc)(void));
void SPI_SetRxCallback(uint8_t Copy_u8SPIx, void(*Copy_pfCallBackFunc)(void));
/***********************************************************************************************************/
#endif
