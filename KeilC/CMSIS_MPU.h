#include <stdio.h>
#include <stdint.h>

#define __I		volatile const
#define __O 	volatile
#define __IO	volatile

/* following define should be used for structure members*/
#define __IM	volatile const
#define __OM	volatile
#define __IOM	volatile




/*typedef struct{
	__IM  uint32_t TYPE;
	__IOM uint32_t CTRL;
	__IOM uint32_t RNR;
	__IOM uint32_t RBAR;
	__IOM uint32_t RASR;
	__IOM uint32_t RBAR_A1;
	__IOM uint32_t RASR_A1;
	__IOM uint32_t RBAR_A2;
	__IOM uint32_t RASR_A2;
	__IOM uint32_t RBAR_A3;
	__IOM uint32_t RASR_A3;

}MPU_Type;*/

#define MPU_TYPE_RALIASES		4U

/* MPU Type register definitions */ 
#define MPU_TYPE_IREGION_Pos		16U
#define MPU_TYPE_IREGION_Msk		(0xFFUL << MPU_TYPE_IREGION_Pos)

#define MPU_TYPE_DREGION_Pos		8U
#define MPU_TYPE_DREGION_Msk		(0xFFUL << MPU_TYPE_DREGION_Pos)

#define MPU_TYPE_SEPARATE_Pos		0U
#define MPU_TYPE_SEPARATE_Msk		(1UL << MPU_TYPE_SEPARATE_Pos)

/* MPU CTRL register definitions */
#define MPU_CRTL_PRIVDEFENA_Pos		2U
#define MPU_CRTL_PRIVDEFENA_Msk		(1UL << MPU_CRTL_PRIVDEFENA_Pos)

#define MPU_CRTL_HFNMIENA_Pos		1U
#define MPU_CRTL_HFNMIENA_Msk		(1UL << MPU_CRTL_HFNMIENA_Pos)

#define MPU_CRTL_ENABLE_Pos			0U
#define MPU_CRTL_ENABLE_Msk			(1UL << MPU_CRTL_ENABLE_Pos)

/* MPU RNR register definitions */
#define MPU_RNR_REGION_Pos			0U
#define MPU_RNR_REGION_Msk			(0xFFUL << MPU_RNR_REGION_Pos)

/* MPU RBAR register definitions*/
#define MPU_RBAR_ADDR_Pos			5U
#define MPU_RBAR_ADDR_Msk			(0x7FFFFFFUL << MPU_RBAR_ADDR_Pos)

#define MPU_RBAR_VALID_Pos			4U
#define MPU_RBAR_VALID_Msk			(1UL << MPU_RBAR_VALID_Pos)

#define MPU_RBAR_REGION_Pos			0U
#define MPU_RBAR_REGION_Msk			(0xFUL << MPU_RBAR_REGION_Pos)

/*MPU RASR register definitions*/
#define MPU_RASR_XN_Pos				28U
#define MPU_RASR_XN_Msk				(1UL << MPU_RASR_XN_Pos)

#define MPU_RASR_AP_Pos				24U
#define MPU_RASR_AP_Msk				(0x7UL << MPU_RASR_AP_Pos)

#define MPU_RASR_TEX_Pos			19U
#define MPU_RASR_TEX_Msk			(0x7UL << MPU_RASR_TEX_Pos)

#define MPU_RASR_S_Pos				18U
#define MPU_RASR_S_Msk				(1UL << MPU_RASR_S_Pos)

#define MPU_RASR_C_Pos				17U
#define MPU_RASR_C_Msk				(1UL << MPU_RASR_C_Pos)

#define MPU_RASR_B_Pos				16U
#define MPU_RASR_B_Msk				(1UL << MPU_RASR_B_Pos)

#define MPU_RASR_SRD_Pos			8UL
#define MPU_RASR_SRD_Msk			(0xFFUL << MPU_RASR_SRD_Pos)

#define MPU_RASR_SIZE_Pos			1UL
#define MPU_RASR_SIZE_Msk			(0x1FUL << MPU_RASR_SIZE_Pos)

#define MPU_RASR_ENABLE_Pos			0UL
#define MPU_RASR_ENABLE_Msk			(1UL << MPU_RASR_ENABLE_Pos)

//#define MPU_BASE					0xE000ED90UL
//#define MPU							((MPU_Type *)MPU_BASE)
