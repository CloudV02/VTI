#ifndef MPU_DRIVER_H
#define MPU_DRIVER_H

#include "stm32f4xx.h"
#include "CMSIS_MPU.h"

/* macro config for MPU */
#define MPU_ENABLE_DEFAULT_MEM		1
#define MPU_DISABLE_DEFAULT_MEM		0

#define MPU_ENABLE_IN_EXCEPTION		1
#define MPU_DISABLE_IN_EXCEPTION	0

/* macro config for region MPU*/
/* REGION */
#define MPU_REGION_0				0
#define MPU_REGION_1				1
#define MPU_REGION_2				2
#define MPU_REGION_3				3
#define MPU_REGION_4				4
#define MPU_REGION_5				5
#define MPU_REGION_6				6
#define MPU_REGION_7				7

/* MEMORY TYPE */
#define MPU_MEM_STRONG_ORDER			0x00
#define MPU_MEM_NORMAL_CACHEABLE	0x01
#define MPU_MEM_NORMAL_NONCACHE		0x02

/* ACCESS RIGHTS*/
#define MPU_ACCESS_NO_ACCESS			0x00
#define MPU_ACCESS_FULL						0x03
#define MPU_ACCESS_READ_ONLY			0x05

// Fault Status (MMFSR bits) MemManage Fault Status Register
#define MPU_FAULT_IACCVIOL          (1 << 0) // Instruction Access Violation
#define MPU_FAULT_DACCVIOL          (1 << 1) // Data Access Violation
#define MPU_FAULT_MUNSTKERR         (1 << 3) // Unstacking Error
#define MPU_FAULT_MSTKERR           (1 << 4) // Stacking Error
#define MPU_FAULT_MLSPERR           (1 << 5) // Lazy State Preservation Error
#define MPU_FAULT_MMARVALID         (1 << 7) // MMFAR Valid


typedef struct{
	uint8_t RegionNumber;
	uint32_t StartAddress;
	uint8_t Size;
	uint8_t MemoryType;
	uint8_t AccessRight;
	uint8_t Enable;
	
}MPU_RegionConfig_t;


typedef struct{
	uint8_t EnableDefaultMem;
	uint8_t EnableInException;
	uint8_t EnableMemManage;		// Enable Memory Manage Handler in SCB
	MPU_RegionConfig_t* Regions;	// Array Regions
	uint8_t RegionCount;			// Number of Region
}MPU_Config_t;

/* FUNCTION DRIVER MPU */
void MPU_Init(MPU_Config_t *config);
void MPU_DeInit(void);
void MPU_SetRegionConfig(MPU_RegionConfig_t* region);
void MPU_GetErrorDetails(uint32_t *faultAddr, uint8_t *faultStatus);


#endif

