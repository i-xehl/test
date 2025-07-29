#ifndef FLASH2CLICK_H
#define FLASH2CLICK_H

#include <stdint.h>
#include "spi.h"
#include "flash2click_dataType.h"


typedef struct {
    SPI_HandleTypeDef* spi;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
} flash2click_t;


flash2click_t* flash2click_create(void);

void flash2click_destroy(flash2click_t* drv);

sst26_flash_status flash2click_init(flash2click_t* drv);

uint8_t flash2click_read_status(flash2click_t* drv);

void flash2click_nop(flash2click_t* drv);

// --------------- Command & Status ---------------

void flash2click_write_enable(flash2click_t* drv);
void flash2click_write_disable(flash2click_t* drv);

uint8_t flash2click_read_status(flash2click_t* drv);
void flash2click_write_status(flash2click_t* drv, uint8_t value);
uint8_t flash2click_read_config(flash2click_t* drv);
void flash2click_write_config(flash2click_t* drv, uint8_t value);


// --------------- ID/Security ---------------

void flash2click_productID(flash2click_t* drv, uint8_t* data, uint32_t len);        // JEDEC ID
void flash2click_read_unique_id(flash2click_t* drv, uint8_t* out8);
void flash2click_read_security_id(flash2click_t* drv, uint8_t* outN);
void flash2click_program_security_id(flash2click_t* drv, const uint8_t* data);
void flash2click_lockout_security_id(flash2click_t* drv);
// --------------- Read/Write ---------------


void flash2click_read(flash2click_t* drv, uint32_t addr, uint8_t* data, uint32_t len);
void flash2click_fast_read(flash2click_t* drv, uint32_t addr, uint8_t* data, uint32_t len);

void flash2click_page_program(flash2click_t* drv, uint32_t addr, const uint8_t* data, uint32_t len);
void flash2click_quad_page_program(flash2click_t* drv, uint32_t addr, const uint8_t* data, uint32_t len);

// --------------- Erase ---------------

void flash2click_sector_erase(flash2click_t* drv, uint32_t addr);
void flash2click_block_erase_32k(flash2click_t* drv, uint32_t addr);
void flash2click_block_erase_64k(flash2click_t* drv, uint32_t addr);
void flash2click_chip_erase(flash2click_t* drv);

// --------------- Suspend/Resume ---------------

void flash2click_erase_suspend(flash2click_t* drv);
void flash2click_erase_resume(flash2click_t* drv);

// --------------- Power/Reset ---------------

void flash2click_sleep(flash2click_t* drv);
void flash2click_wakeup(flash2click_t* drv);
void flash2click_enable_reset(flash2click_t* drv);
void flash2click_reset(flash2click_t* drv);

// --------------- Block Lock ---------------
void flash2click_global_block_unlock(flash2click_t* drv);
void flash2click_indiv_block_lock(flash2click_t* drv, uint32_t addr);
void flash2click_indiv_block_unlock(flash2click_t* drv, uint32_t addr);
void flash2click_read_block_lock(flash2click_t* drv, uint32_t addr, uint8_t* out);

// --------------- Bank Register ---------------
uint8_t flash2click_read_bank_reg(flash2click_t* drv);
void flash2click_write_bank_reg(flash2click_t* drv, uint8_t value);

// --------- SFDP ---------
void flash2click_read_sfdp(flash2click_t* drv, uint32_t addr, uint8_t* data, uint32_t len);



#endif // FLASH2CLICK_H
