/************************************************************************************
 * Author: Khooly																	*
 * Date: 19 March 2024																*
 * Version: 0.1																		*
 ***********************************************************************************/

#ifndef SPI_PRIVATE_H
#define SPI_PRIVATE_H

typedef union
{
	struct{
		uint32_t CPHA		: 1;
		uint32_t CPOL		: 1;
		uint32_t MSTR		: 1;
		uint32_t BR			: 3;
		uint32_t SPE		: 1;
		uint32_t LSBFIRST	: 1;
		uint32_t SSI		: 1;
		uint32_t SSM		: 1;
		uint32_t RXONLY		: 1;
		uint32_t DFF		: 1;
		uint32_t CRCNEXT	: 1;
		uint32_t CRCEN		: 3;
		uint32_t BIDIOE		: 1;
		uint32_t BIDIMODE	: 1;
		uint32_t RESERVED	: 14;
	}BitAccess;
	uint32_t RegisterAccess;
}CR1_Reg_t;

typedef union
{
	struct{
		uint32_t  RXDMAEN	:1 ;
		uint32_t  TXDMAEN	:1 ;
		uint32_t  SSOE		:1 ;
		uint32_t  Reserved1	:1 ;
		uint32_t  FRF		:1 ;
		uint32_t  ERRIE		:1 ;
		uint32_t  RXNEIE	:1 ;
		uint32_t  TXEIE		:1 ;
		uint32_t  Reserved2	:24;
	}BitAccess;
	uint32_t RegisterAccess;
}CR2_Reg_t;

typedef union
{
	struct{
		uint32_t  RXNE		:1 ;
		uint32_t  TXE		:1 ;
		uint32_t  CHSIDE	:1 ;
		uint32_t  UDR		:1 ;
		uint32_t  CRCERR	:1 ;
		uint32_t  MODF		:1 ;
		uint32_t  OVR		:1 ;
		uint32_t  BSY		:1 ;
		uint32_t  FRE		:1 ;
		uint32_t  Reserved	:23;
	}BitAccess;
	uint32_t RegisterAccess;
}SR_Reg_t;

typedef struct
{
	volatile CR1_Reg_t	CR1    ;
	volatile CR2_Reg_t 	CR2    ;
	volatile SR_Reg_t 	SR     ;
	volatile uint32_t 	DR     ;
	volatile uint32_t 	CRCPR  ;
	volatile uint32_t 	RXCRCR ;
	volatile uint32_t 	TXCRCR ;
	volatile uint32_t 	I2SCFGR;
	volatile uint32_t 	I2SPR  ;
}SPI_t;

#define SPI1_BASE_ADDRESS		0x40013000
#define SPI2_BASE_ADDRESS		0x40013800
#define SPI3_BASE_ADDRESS		0x40013C00

static SPI_t* SPI_pxPtrSelect(uint8_t Copy_u8SpiNum);

#endif
