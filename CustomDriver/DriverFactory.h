

#ifndef DRIVER_FACTORY_H
#define DRIVER_FACTORY_H

#include <stdint.h>

typedef enum {
    DRIVER_FLASH2CLICK = 0,
} DriverType;

typedef void* DriverHandle;

DriverHandle DriverFactory_Create(DriverType type);

void DriverFactory_Destroy(DriverHandle handle);

DriverHandle DriverFactory_flash2clickCreate();

#endif // DRIVER_FACTORY_H
