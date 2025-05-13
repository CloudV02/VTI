#include <stm32f401x.h>
#include <MPU_Driver.h>

int main(){
    MPU_RegionConfig_t regions[] = {
		{MPU_REGION_0, 0x00000000, 31,MPU_MEM_STRONG_ORDER,MPU_ACCESS_FULL,1},
		{MPU_REGION_3, 0x40000000, 9,MPU_MEM_NORMAL_NONCACHE,MPU_ACCESS_READ_ONLY,1}
	}
	
	MPU_Config_t config = { MPU_ENABLE_DEFAULT_MEM, MPU_ENABLE_IN_EXCEPTION, 1, 
	regions, 2 }
	
	MPU_Init();
}