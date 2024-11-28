#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#include "shell.c"

LOG_MODULE_REGISTER(env_analyzer, LOG_LEVEL_INF);

// Definições para os sensores
#define I2C_NODE DT_NODELABEL(i2c0)
#define ADC_NODE DT_NODELABEL(adc0)

// Buffers para dados de sensores
struct sensor_value temp, press, humidity;

// Device pointers
const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);
const struct device *bme680 = DEVICE_DT_GET_ANY(bosch_bme680);

// Threads
void sensor_thread(void *arg1, void *arg2, void *arg3);
// void ble_thread(void *arg1, void *arg2, void *arg3);

// Função de inicialização do BLE
void ble_init() {
    int err = bt_enable(NULL);
    if (err) {
        printk("Erro ao inicializar BLE: %d", err);
        return;
    }

    LOG_INF("BLE inicializado com sucesso");
}

// Função de inicialização dos sensores
void sensor_init() {
    if (!device_is_ready(i2c_dev)) {
        printk("I2C não está pronto");
        return;
    }

    LOG_INF("Sensores inicializados");
}

// Thread para leitura dos sensores
void sensor_thread(void *arg1, void *arg2, void *arg3) {
    while (1) {
        
        sensor_sample_fetch(bme680);
		sensor_channel_get(bme680, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(bme680, SENSOR_CHAN_PRESS, &press);
		sensor_channel_get(bme680, SENSOR_CHAN_HUMIDITY, &humidity);

        if(sensor_pulling){
            printk("\nT: %d.%06d; P: %d.%06d; H: %d.%06d;\n",
				temp.val1, temp.val2, press.val1, press.val2,
				humidity.val1, humidity.val2);
        }
		

        k_sleep(K_SECONDS(5));
    }
}

// Thread para envio BLE
void ble_thread(void *arg1, void *arg2, void *arg3) {
    while (1) {
        // Atualizar valores no GATT
        // printk("Enviando dados BLE...");
        k_sleep(K_SECONDS(10));
    }
}

K_THREAD_DEFINE(sensor_tid, 2048, sensor_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(ble_tid, 2048, ble_thread, NULL, NULL, NULL, 5, 0, 0);

void main(void) {
    printk("Inicializando sistema...");
    // ble_init();
    sensor_init();
    printk("Sistema inicializado");
}