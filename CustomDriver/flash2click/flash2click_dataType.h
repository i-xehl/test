


typedef enum {
    READ = 0x03,              // Normal Read
    FAST_READ = 0x0B,         // Fast Read
    DUAL_OUT_READ = 0x3B,     // Dual Output Fast Read
    DUAL_IO_READ = 0xBB,      // Dual I/O Fast Read
    QUAD_OUT_READ = 0x6B,     // Quad Output Fast Read
    QUAD_IO_READ = 0xEB,      // Quad I/O Fast Read
    SQI_READ = 0xAF,          // SQI Mode Read
    HIGH_PERF_READ = 0x0B,    // High-Performance Read (alias of FAST_READ)
} sst26_read_cmd_t;


typedef enum {
    PAGE_PROGRAM = 0x02,            // Page Program
    QUAD_PAGE_PROGRAM = 0x32,       // Quad Page Program
    SQI_PAGE_PROGRAM = 0x38,        // SQI Page Program
    WRITE_ENABLE = 0x06,            // Write Enable
    WRITE_DISABLE = 0x04,           // Write Disable
} sst26_prog_cmd_t;


typedef enum {
    SECTOR_ERASE_4K = 0x20,        // 4KB Sector Erase
    BLOCK_ERASE_32K = 0x52,        // 32KB Block Erase
    BLOCK_ERASE_64K = 0xD8,        // 64KB Block Erase
    CHIP_ERASE = 0x60,             // Chip Erase (alternate: 0xC7)
    CHIP_ERASE_ALT = 0xC7,         // Chip Erase (alternate: 0x60)
    ERASE_SUSPEND = 0x75,          // Suspend Erase
    ERASE_RESUME = 0x7A,           // Resume Erase
} sst26_erase_cmd_t;


typedef enum {
    READ_STATUS = 0x05,            // Read Status Register
    WRITE_STATUS = 0x01,           // Write Status Register
    READ_CONFIG = 0x35,            // Read Configuration Register
    READ_BANK_REG = 0x16,          // Read Bank Register
    WRITE_BANK_REG = 0x17,         // Write Bank Register
    GLOBAL_BLOCK_UNLOCK = 0x98,    // Global Block Unlock
    READ_BLOCK_LOCK = 0x3D,        // Read Block Lock Status
	WRITE_BLOCK_PROTECTION_REGISTER = 0x42,
} sst26_status_cfg_cmd_t;


typedef enum {
    READ_ID = 0x9F,               // Read JEDEC ID
    READ_SFDP = 0x5A,             // Read SFDP Register
    READ_UNIQUE_ID = 0x4B,        // Read Unique ID
    READ_SECURITY_ID = 0x88,      // Read Security ID
    PROGRAM_SECURITY_ID = 0xA5,   // Program Security ID
    LOCKOUT_SECURITY_ID = 0x85,   // Lockout Security ID
} sst26_id_cmd_t;


typedef enum {
    ENABLE_RESET = 0x66,          // Enable Reset
    RESET_CHIP = 0x99,                 // Reset
    SLEEP = 0xB9,                 // Deep Power Down (Sleep)
    WAKE = 0xAB,                  // Release from Deep Power Down
    NOP = 0x00,                   // No Operation (dummy)
} sst26_misc_cmd_t;


typedef enum {
	FLASH_ON = 0x01,
	FLASH_OFF = 0x00,
} sst26_flash_status;


