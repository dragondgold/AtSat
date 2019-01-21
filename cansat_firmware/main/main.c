/*
 Firmware para el satélite CanSat desarrollado por AXTEC y distribuido
  bajo la licencia GNU GPL V3.0.

  Autor: Andrés Fernando Torti
  Fecha: 21/01/2019
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "aux_ps/aux_ps.h"
#include "i2c_manager/i2c_manager.h"
#include "spi_manager/spi_manager.h"
#include "gps_manager/gps_manager.h"

void app_main()
{
    printf("System Init\n");

    // Get chip information and output it
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("ESP32 with %d CPU cores. Revision %d.",
            chip_info.cores,
            chip_info.revision);

    // Flash information
    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    fflush(stdout);

    // Init every system in the satellite
    aux_ps_init();
    i2c_manager_init();
    spi_manager_init();
    gps_manager_init();
}
