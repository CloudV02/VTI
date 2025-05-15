#include "MPU_Driver.h"

void MPU_Init(MPU_Config_t *config){

    MPU->CTRL &= ~MPU_CRTL_ENABLE_Msk;
		uint8_t loop;
    /* Config Regions */
    for(loop = 0; loop < config -> RegionCount; loop++){
        MPU_RegionConfig_t *region = &config -> Regions[loop];
        MPU->RNR = region->RegionNumber;
        MPU->RBAR = region->StartAddress;
        MPU->RASR = (region->Size << MPU_RASR_SIZE_Pos) | (region->MemoryType == MPU_MEM_STRONG_ORDER ? (0x0 << MPU_RASR_TEX_Pos): region->MemoryType == MPU_MEM_NORMAL_CACHEABLE ? ((0x1 << MPU_RASR_TEX_Pos) | MPU_RASR_C_Msk ) : (0x1 << MPU_RASR_TEX_Pos)) | (region->AccessRight << MPU_RASR_AP_Pos) | (region->Enable << MPU_RASR_ENABLE_Pos);
    }

    /* Config MPU Control*/
    uint32_t ctrl = 0;
    if(config->EnableDefaultMem) ctrl |= MPU_CRTL_PRIVDEFENA_Msk;
    if(config->EnableInException) ctrl |= MPU_CRTL_HFNMIENA_Msk;
    ctrl |= MPU_CRTL_ENABLE_Msk;

    MPU -> CTRL = ctrl;

    /*MemManage Handler*/
    
    if (config->EnableMemManage) {
        NVIC_EnableIRQ(MemoryManagement_IRQn);
    }
    
    

    /* Dong bo hoa */
    __DSB();
    __ISB(); 
}

void MPU_DeInit(void){
    MPU->CTRL = 0;
		int loop; 
    for(loop = 0; loop < 8; loop++)
    {
        MPU->RNR = loop;
        MPU->RASR = 0;
        MPU->RBAR = 0;
    }    

    __DSB();
    __ISB();
}

void MPU_SetRegionConfig(MPU_RegionConfig_t *region){
    if(region->RegionNumber >7) return;
    uint32_t ctrl = MPU->CTRL;
    MPU->CTRL &= ~MPU_CRTL_ENABLE_Msk;

    MPU->RNR = region->RegionNumber;
    MPU->RBAR = region->StartAddress;
    MPU->RASR = (region->Size << MPU_RASR_SIZE_Pos)|(region->MemoryType == MPU_MEM_STRONG_ORDER ? (0x0 <<MPU_RASR_TEX_Pos): region->MemoryType ==MPU_MEM_NORMAL_CACHEABLE ? (0x1 << MPU_RASR_TEX_Pos) | MPU_RASR_C_Msk : (0x1<<MPU_RASR_TEX_Pos)) | (region->AccessRight << MPU_RASR_AP_Pos) | (region->Enable << MPU_RASR_ENABLE_Pos);

    MPU->CTRL = ctrl;

    __DSB();
    __ISB();
}

// use in IDE

void MPU_GetErrorDetails(uint32_t* faultAddr, uint8_t* faultStatus) {
    *faultAddr = SCB->MMFAR;   // Địa chỉ gây lỗi
    *faultStatus = SCB->CFSR & 0xFF; // Trạng thái lỗi (MMFSR)
	
		// Xóa cờ lỗi sau khi đọc
    SCB->CFSR |= SCB->CFSR & 0xFF;
}

void MemManage_Handler(void) {
    uint32_t faultAddr;
    uint8_t faultStatus;
    MPU_GetErrorDetails(&faultAddr, &faultStatus);
    while (1); // Debug lỗi
}
