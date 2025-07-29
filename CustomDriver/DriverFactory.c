#include "DriverFactory.h"
#include <stdlib.h>
#include "flash2click/flash2click.h"
#include "spi.h"
#include "gpio.h"


extern SPI_HandleTypeDef hspi3;


DriverHandle DriverFactory_Create(DriverType type)
{
    switch(type)
    {
        case DRIVER_FLASH2CLICK:
            return  DriverFactory_flash2clickCreate();
        default:
            return NULL;
    }
}

DriverHandle DriverFactory_flash2clickCreate(){
	flash2click_t* drv = (DriverHandle)flash2click_create();
	 drv->spi = &hspi3;
     drv->cs_port = SPI3_CS_GPIO_Port;
     drv->cs_pin = SPI3_CS_Pin;
	return (DriverHandle)drv;
}

void DriverFactory_Destroy(DriverHandle handle)
{
    if(handle)
    {
    	  HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);

        flash2click_destroy((flash2click_t*)handle);
    }
}
