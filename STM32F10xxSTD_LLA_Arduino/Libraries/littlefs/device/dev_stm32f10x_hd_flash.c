#include "lfs.h"

#include "LLA_Drivers.h"

static int stm32f10x_hd_flash_read(const struct lfs_config *c, lfs_block_t block,lfs_off_t off, void *buffer, lfs_size_t size){
	LLA_Flash_ReadBytes((block+1) * c->block_size + off,buffer,size);
	return LFS_ERR_OK;
}


static int stm32f10x_hd_flash_prog(const struct lfs_config *c, lfs_block_t block,lfs_off_t off, const void *buffer, lfs_size_t size){
	if(LLA_Flash_WriteBytes((block+1) * c->block_size + off,buffer,size) != 1){
		return LFS_ERR_IO;
	}
	return LFS_ERR_OK;
}


static int stm32f10x_hd_flash_erase(const struct lfs_config *c, lfs_block_t block){
	if(LLA_Flash_Erase((block+1) * c->block_size,c->block_size) != 1){
		return LFS_ERR_IO;
	}
	return LFS_ERR_OK;
}


static int stm32f10x_hd_flash_sync(const struct lfs_config *c){
	return LFS_ERR_OK;
}


const struct lfs_config dev_stm32f10x_hd_cfg = {
 // block device operations
    .read  = stm32f10x_hd_flash_read,
    .prog  = stm32f10x_hd_flash_prog,
    .erase = stm32f10x_hd_flash_erase,
    .sync  = stm32f10x_hd_flash_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 16,
    .block_size = 0x800,
    .block_count = 127,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,
};




