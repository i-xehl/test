#include "flash2click.h"
#include "SEGGER_RTT.h"
#include <string.h>
#include <assert.h>
#include <stdint.h>

// Use correct flash size for your chip
#define FLASH_TOTAL_SIZE     (8 * 1024 * 1024) // 8MB for SST26VF064B
#define PAGE_SIZE            256
#define SECTOR_SIZE          (4 * 1024)
#define BLOCK32K_SIZE        (32 * 1024)
#define BLOCK64K_SIZE        (64 * 1024)



#define ASSERT_PRINT(cond, msg) \
    if (!(cond)) { SEGGER_RTT_printf(0, "ASSERT FAIL: %s\n", msg); while(1); }

void print_array(const char* name, const uint8_t* arr, int len) {
    SEGGER_RTT_printf(0, "%s: ", name);
    for (int i = 0; i < len; ++i) SEGGER_RTT_printf(0, "%02X ", arr[i]);
    SEGGER_RTT_printf(0, "\n");
}

void flash2click_test_all_extended()
{
    flash2click_t* drv = flash2click_create();
    SEGGER_RTT_printf(0, "\n=== flash2click EXTENDED HIL Test Start ===\n");

    sst26_flash_status stat = flash2click_init(drv);
    ASSERT_PRINT(stat == FLASH_ON, "Flash init failed");
    SEGGER_RTT_printf(0, "Flash init OK\n");

    uint32_t t0 = HAL_GetTick();
    flash2click_chip_erase(drv);
    uint32_t t1 = HAL_GetTick();
    SEGGER_RTT_printf(0, "Chip erase done, time: %lu ms\n", (unsigned long)(t1-t0));

    uint8_t buf[PAGE_SIZE];
    flash2click_read(drv, 0x000000, buf, PAGE_SIZE);
    int erased = 1;
    for (int i = 0; i < PAGE_SIZE; ++i) if (buf[i] != 0xFF) erased = 0;
    ASSERT_PRINT(erased, "First page not erased");

    flash2click_read(drv, FLASH_TOTAL_SIZE - PAGE_SIZE, buf, PAGE_SIZE);
    erased = 1;
    for (int i = 0; i < PAGE_SIZE; ++i) if (buf[i] != 0xFF) erased = 0;
    ASSERT_PRINT(erased, "Last page not erased");
    SEGGER_RTT_printf(0, "Boundary erase OK\n");

    for (int i = 0; i < PAGE_SIZE; ++i) buf[i] = (uint8_t)i;
    flash2click_page_program(drv, 0x000000, buf, PAGE_SIZE);
    memset(buf, 0, PAGE_SIZE);
    flash2click_read(drv, 0x000000, buf, PAGE_SIZE);
    for (int i = 0; i < PAGE_SIZE; ++i) ASSERT_PRINT(buf[i] == (uint8_t)i, "Write/read min addr failed");
    SEGGER_RTT_printf(0, "Write/read min addr OK\n");

    for (int i = 0; i < PAGE_SIZE; ++i) buf[i] = (uint8_t)(255 - i);
    flash2click_page_program(drv, FLASH_TOTAL_SIZE - PAGE_SIZE, buf, PAGE_SIZE);
    memset(buf, 0, PAGE_SIZE);
    flash2click_read(drv, FLASH_TOTAL_SIZE - PAGE_SIZE, buf, PAGE_SIZE);
    for (int i = 0; i < PAGE_SIZE; ++i) ASSERT_PRINT(buf[i] == (uint8_t)(255 - i), "Write/read max addr failed");
    SEGGER_RTT_printf(0, "Write/read max addr OK\n");

    t0 = HAL_GetTick();
    flash2click_page_program(drv, 0x10000, buf, PAGE_SIZE);
    t1 = HAL_GetTick();
    SEGGER_RTT_printf(0, "Page write time: %lu ms\n", (unsigned long)(t1-t0));
    t0 = HAL_GetTick();
    flash2click_read(drv, 0x10000, buf, PAGE_SIZE);
    t1 = HAL_GetTick();
    SEGGER_RTT_printf(0, "Page read time: %lu ms\n", (unsigned long)(t1-t0));

    uint8_t sec_id[8] = {0xA5, 0x5A, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
    flash2click_program_security_id(drv, sec_id);
    memset(sec_id, 0, 8);
    flash2click_read_security_id(drv, sec_id);
    print_array("Security ID", sec_id, 8);

    uint8_t bank = flash2click_read_bank_reg(drv);
    SEGGER_RTT_printf(0, "Bank reg: 0x%02X\n", bank);
    flash2click_write_bank_reg(drv, bank);

    uint8_t sfdp[16] = {0};
    flash2click_read_sfdp(drv, 0x000000, sfdp, 16);
    print_array("SFDP[0:15]", sfdp, 16);

    uint8_t uid[8];
    flash2click_read_unique_id(drv, uid);
    print_array("Unique ID", uid, 8);
    uint8_t prod_id[3];
    flash2click_productID(drv, prod_id, 3);
    print_array("JEDEC ID", prod_id, 3);
    uint8_t data[PAGE_SIZE];
    memset(data, 0xAA, PAGE_SIZE);
    flash2click_page_program(drv, 0, &data[0] , PAGE_SIZE);

    flash2click_destroy(drv);
    SEGGER_RTT_printf(0, "=== flash2click EXTENDED HIL Test Complete ===\n");
}
