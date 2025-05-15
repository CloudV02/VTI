#include "stm32f401xc.h"
#include "stm32f4xx.h"
#include "MPU_Driver.h"

int main(){
    MPU_RegionConfig_t regions[] = {
		{MPU_REGION_0, 0x00000000, 31,MPU_MEM_STRONG_ORDER,MPU_ACCESS_FULL,1},
		{MPU_REGION_3, 0x40000000, 9,MPU_MEM_NORMAL_NONCACHE,MPU_ACCESS_READ_ONLY,1}
	};
	
	MPU_Config_t config = { MPU_ENABLE_DEFAULT_MEM, MPU_ENABLE_IN_EXCEPTION, 1, regions, 2 };
	
	MPU_Init(&config);
  /* Write to region 3 (read only) -> gay fault */
  volatile uint32_t* test_addr = (uint32_t*)0x40000000;
	
	/* create fault for test */
  //*test_addr = 0x12345678;
	
	/* change by full access */
  MPU_RegionConfig_t newRegion = {MPU_REGION_3, 0x40000000, 9, MPU_MEM_NORMAL_NONCACHE, MPU_ACCESS_FULL, 1};
  MPU_SetRegionConfig(&newRegion);

  /* ghi lai */
  *test_addr = 0x12345678;
  while (1);
}

