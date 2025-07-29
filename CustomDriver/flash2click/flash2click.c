#include "flash2click.h"
#include <stdlib.h>
#include "SEGGER_RTT.h"



flash2click_t* flash2click_create(void)
{
    flash2click_t* drv = (flash2click_t*)malloc(sizeof(flash2click_t));
    return drv;
}

void flash2click_destroy(flash2click_t* drv)
{
    if(drv) {
        free(drv);
    }
}

void flash2click_csLow(){
	  HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);

}

void flash2click_csHigh(){
	  HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);
}

void flash2click_nop(flash2click_t* drv) {
	uint8_t cmd = NOP; // NOP command
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_global_block_unlock(flash2click_t* drv) {
    uint8_t cmd = GLOBAL_BLOCK_UNLOCK;
    flash2click_csLow();
    HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
    flash2click_csHigh();
}


uint8_t flash2click_read_status(flash2click_t* drv){
	uint8_t cmd = READ_STATUS;
	uint8_t data = 0;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, &data, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
	return data;
}

void flash2click_chip_erase(flash2click_t* drv){
	uint8_t cmd = CHIP_ERASE_ALT;
	flash2click_write_enable(drv);
	HAL_Delay(2);
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();

	uint8_t status = flash2click_read_status(drv);
	SEGGER_RTT_printf(0, "flash2click_chip_erase  erase : %02X\n", status);

	while(status & 0x01){
		status = flash2click_read_status(drv);
	}

	SEGGER_RTT_printf(0, "flash2click_chip_erase  erase : %02X\n", status);
}

void flash2click_productID(flash2click_t* drv, uint8_t* data, uint32_t len){
	  uint8_t cmd = READ_ID;
	  flash2click_csLow();
	  HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	  HAL_SPI_Receive(drv->spi, data, 3, HAL_MAX_DELAY);
	  flash2click_csHigh();
	  SEGGER_RTT_printf(0, "JEDEC ID: %02X %02X %02X\n", data[0], data[1], data[2]);
}

void flash2click_write_enable(flash2click_t* drv){
	uint8_t cmd = WRITE_ENABLE;
	flash2click_csLow();
    HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_write_disable(flash2click_t* drv){
	uint8_t cmd = WRITE_DISABLE;
	flash2click_csLow();
    HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_write_status(flash2click_t* drv, uint8_t value){
	uint8_t cmd = WRITE_STATUS;
	flash2click_csLow();
    HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &value, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

uint8_t flash2click_read_config(flash2click_t* drv){
	uint8_t cmd = READ_CONFIG;
	uint8_t data = 0;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, &data, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
	return data;
}

void flash2click_fast_read(flash2click_t* drv, uint32_t addr, uint8_t* data, uint32_t len){
	uint8_t cmd = FAST_READ;
	uint8_t temp[3];
	temp[0] = (uint8_t)  (addr >> 16);
	temp[1] = (uint8_t)  (( addr & 0x00FF00 ) >> 8);
	temp[2] = (uint8_t)  ( addr & 0x0000FF );

	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &temp[0], 3, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, data, len, HAL_MAX_DELAY);
	flash2click_csHigh();

	for(uint8_t i= 0; i < len; i++){
		   SEGGER_RTT_printf(0, "data : %02X \n", data[i]);
	}

}


void flash2click_page_program(flash2click_t* drv, uint32_t addr, const uint8_t* data, uint32_t len){
	flash2click_write_enable(drv);
	HAL_Delay(10);
	uint8_t cmd = PAGE_PROGRAM;
	uint8_t temp[3];
    temp[0] = (uint8_t)  (addr >> 16);
    temp[1] = (uint8_t)  (( addr & 0x00FF00 ) >> 8);
    temp[2] = (uint8_t)  ( addr & 0x0000FF );

	flash2click_csLow();
    HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(drv->spi, &temp[0], 3, HAL_MAX_DELAY);
    HAL_SPI_Transmit(drv->spi,  (uint8_t*)data, len, HAL_MAX_DELAY);
	flash2click_csHigh();

	uint8_t status = flash2click_read_status(drv);

	while(status & 0x01){
		status = flash2click_read_status(drv);
	}

	flash2click_read_status(drv);
}

void flash2click_read(flash2click_t* drv, uint32_t addr, uint8_t* data, uint32_t len){
	uint8_t cmd = READ;
	uint8_t temp[3];
    temp[0] = (uint8_t)  (addr >> 16);
    temp[1] = (uint8_t)  (( addr & 0x00FF00 ) >> 8);
    temp[2] = (uint8_t)  ( addr & 0x0000FF );

	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &temp[0], 3, HAL_MAX_DELAY);
    HAL_SPI_Receive(drv->spi, data, len, HAL_MAX_DELAY);
	flash2click_csHigh();
}

sst26_flash_status flash2click_init(flash2click_t* drv) {
	uint8_t data[3] = {0,0,0};
	flash2click_reset(drv);
	flash2click_productID(drv, data, sizeof(data));
    flash2click_global_block_unlock(drv);
	if(data[0] == 0xBF && data[1] == 0x26 && data[2] == 0x43){
	    return FLASH_ON;
	}
	return FLASH_OFF;
}


void flash2click_quad_page_program(flash2click_t* drv, uint32_t addr, const uint8_t* data, uint32_t len) {}

void flash2click_sector_erase(flash2click_t* drv, uint32_t addr) {
	uint8_t cmd = SECTOR_ERASE_4K;
	uint8_t temp[3];
	temp[0] = (uint8_t)  (addr >> 16);
	temp[1] = (uint8_t)  (( addr & 0x00FF00 ) >> 8);
	temp[2] = (uint8_t)  ( addr & 0x0000FF );

	flash2click_write_enable(drv);
	HAL_Delay(10);
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &temp[0], 3, HAL_MAX_DELAY);
	flash2click_csHigh();

	uint8_t status = flash2click_read_status(drv);

	while(status & 0x01){
		status = flash2click_read_status(drv);
	}
}


void flash2click_block_erase_32k(flash2click_t* drv, uint32_t addr) {
	uint8_t cmd = BLOCK_ERASE_32K;
	uint8_t temp[3];
	temp[0] = (uint8_t)  (addr >> 16);
	temp[1] = (uint8_t)  (( addr & 0x00FF00 ) >> 8);
	temp[2] = (uint8_t)  ( addr & 0x0000FF );

	flash2click_write_enable(drv);
	HAL_Delay(10);
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &temp[0], 3, HAL_MAX_DELAY);
	flash2click_csHigh();

	uint8_t status = flash2click_read_status(drv);

	while(status & 0x01){
		status = flash2click_read_status(drv);
	}
}

void flash2click_block_erase_64k(flash2click_t* drv, uint32_t addr) {
	uint8_t cmd = BLOCK_ERASE_64K;
	uint8_t temp[3];
	temp[0] = (uint8_t)  (addr >> 16);
	temp[1] = (uint8_t)  (( addr & 0x00FF00 ) >> 8);
	temp[2] = (uint8_t)  ( addr & 0x0000FF );

	flash2click_write_enable(drv);
	HAL_Delay(10);
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &temp[0], 3, HAL_MAX_DELAY);
	flash2click_csHigh();

	uint8_t status = flash2click_read_status(drv);

	while(status & 0x01){
		status = flash2click_read_status(drv);
	}
}

void flash2click_erase_suspend(flash2click_t* drv) {
	uint8_t cmd = ERASE_SUSPEND;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_erase_resume(flash2click_t* drv) {
	uint8_t cmd = ERASE_RESUME;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_sleep(flash2click_t* drv) {
	uint8_t cmd = SLEEP;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_wakeup(flash2click_t* drv) {
	uint8_t cmd = WAKE;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_enable_reset(flash2click_t* drv) {
	uint8_t cmd = ENABLE_RESET;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_reset(flash2click_t* drv) {
	uint8_t cmd = RESET;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
	HAL_Delay(10);
}


void flash2click_read_block_lock(flash2click_t* drv, uint32_t addr, uint8_t* out) {
	uint8_t cmd = READ_BLOCK_LOCK;
	uint8_t temp[3];
	temp[0] = (uint8_t)  (addr >> 16);
	temp[1] = (uint8_t)  (( addr & 0x00FF00 ) >> 8);
	temp[2] = (uint8_t)  ( addr & 0x0000FF );

	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &temp[0], 3, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, out, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_read_unique_id(flash2click_t* drv, uint8_t* out8) {
	uint8_t cmd = READ_UNIQUE_ID;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, out8, 8, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_read_security_id(flash2click_t* drv, uint8_t* outN) {
	uint8_t cmd = READ_SECURITY_ID;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, outN, 8, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_program_security_id(flash2click_t* drv, const uint8_t* data) {
	uint8_t cmd = PROGRAM_SECURITY_ID;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, data, 8, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_lockout_security_id(flash2click_t* drv) {
	uint8_t cmd = LOCKOUT_SECURITY_ID;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}

uint8_t flash2click_read_bank_reg(flash2click_t* drv) {
	uint8_t cmd = READ_BANK_REG;
	uint8_t value = 0;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, &value, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
	return value;
}

void flash2click_read_sfdp(flash2click_t* drv, uint32_t addr, uint8_t* data, uint32_t len) {
	uint8_t cmd = READ_SFDP;
	uint8_t temp[4];
	temp[0] = (uint8_t)  (addr >> 24);
	temp[1] = (uint8_t)  (addr >> 16);
	temp[2] = (uint8_t)  (addr >> 8);
	temp[3] = (uint8_t)  (addr & 0x0000FF);

	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &temp[0], 4, HAL_MAX_DELAY);
	HAL_SPI_Receive(drv->spi, data, len, HAL_MAX_DELAY);
	flash2click_csHigh();
}

void flash2click_write_bank_reg(flash2click_t* drv, uint8_t value) {
	uint8_t cmd = WRITE_BANK_REG;
	flash2click_csLow();
	HAL_SPI_Transmit(drv->spi, &cmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(drv->spi, &value, 1, HAL_MAX_DELAY);
	flash2click_csHigh();
}
