#include "driver_w25qxx.h"
#include "driver_w25qxx_interface.h"

static w25qxx_handle_t w25q128handle;

uint8_t w25q128_user_init(){
	DRIVER_W25QXX_LINK_INIT(&w25q128handle,w25qxx_handle_t);
	DRIVER_W25QXX_LINK_DEBUG_PRINT(&w25q128handle,w25qxx_interface_debug_print);
	DRIVER_W25QXX_LINK_DELAY_MS(&w25q128handle,w25qxx_interface_delay_ms);
	DRIVER_W25QXX_LINK_DELAY_US(&w25q128handle,w25qxx_interface_delay_us);
	DRIVER_W25QXX_LINK_SPI_QSPI_DEINIT(&w25q128handle,w25qxx_interface_spi_qspi_deinit);
	DRIVER_W25QXX_LINK_SPI_QSPI_INIT(&w25q128handle,w25qxx_interface_spi_qspi_init);
	DRIVER_W25QXX_LINK_SPI_QSPI_WRITE_READ(&w25q128handle,w25qxx_interface_spi_qspi_write_read);
	w25qxx_set_type(&w25q128handle,W25Q128);
	return w25qxx_init(&w25q128handle);
}

uint8_t w25q128_user_read(uint32_t addr,uint8_t * buf,uint32_t len){
	return w25qxx_fast_read(&w25q128handle,addr,buf,len);
}


uint8_t w25q128_user_write(uint32_t addr,uint8_t * buf,uint32_t len){
	return w25qxx_write(&w25q128handle,addr,buf,len);
}

uint8_t w25q128_user_erase_sector(uint32_t sector){
	return w25qxx_sector_erase_4k(&w25q128handle,sector * 0x1000);
}

