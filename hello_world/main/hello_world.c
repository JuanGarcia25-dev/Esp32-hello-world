/*
- SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
- SPDX-License-Identifier: CC0-1.0
- 
- Descripción:
- Este programa es un ejemplo base ("Hello World") para la plataforma ESP-IDF.
- Muestra mensajes por el puerto serie, imprime información del chip,
- y finalmente reinicia el dispositivo tras una cuenta regresiva.
*/

#include <stdio.h>          // Funciones estándar de entrada/salida (printf, fflush)
#include <inttypes.h>       // Formato portable para enteros (PRIu32)

#include "sdkconfig.h"      // Configuración generada por 'idf.py menuconfig'
#include "freertos/FreeRTOS.h"  // Sistema operativo FreeRTOS
#include "freertos/task.h"      // Funciones para tareas (vTaskDelay, etc.)
#include "esp_chip_info.h"      // Información del chip
#include "esp_flash.h"          // Acceso a la memoria flash
#include "esp_system.h"         // Funciones generales del sistema (reinicio, heap, etc.)

// Función principal de la aplicación
void app_main(void)
{
    // --- Mensaje de bienvenida ---
    printf("Hello world!\n");
    fflush(stdout); // fuerza a enviar el mensaje por UART

    // --- Variables para información del chip y flash ---
    esp_chip_info_t chip_info;
    uint32_t flash_size;

    // --- Obtener información del chip ---
    esp_chip_info(&chip_info);

    // --- Imprimir información general del chip ---
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
        CONFIG_IDF_TARGET,
        chip_info.cores,
        (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
        (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
        (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
        (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : ""
    );

    unsigned major_rev = chip_info.revision / 100; // parte mayor de la revisión
    unsigned minor_rev = chip_info.revision % 100; // parte menor
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);

    // --- Obtener tamaño de la memoria flash ---
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed\n");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n",
        flash_size / (uint32_t)(1024 * 1024),
        (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external"
    );

    // --- Mostrar memoria mínima disponible ---
    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
    fflush(stdout);

    // --- Cuenta regresiva antes del reinicio ---
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        fflush(stdout);
        vTaskDelay(1000 / portTICK_PERIOD_MS); // espera 1 segundo
    }

    printf("Restarting now.\n");
    fflush(stdout);

    // --- Reinicio del chip ---
    esp_restart();

    // --- Bucle infinito para mantener la salida visible en el monitor ---
    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
