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

#include "shell.c"

LOG_MODULE_REGISTER(env_analyzer, LOG_LEVEL_INF);

// Definições para os sensores
#define I2C_NODE DT_NODELABEL(i2c0)
#define ADC_NODE DT_NODELABEL(adc0)

// Buffers para dados
static float temperature = 0.0;
static float humidity = 0.0;
static uint16_t sound_level = 0;
static uint16_t light_intensity = 0;

// Device pointers
const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);
// const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);

// Threads
void sensor_thread(void *arg1, void *arg2, void *arg3);
void ble_thread(void *arg1, void *arg2, void *arg3);

// Bluetooth GATT Service
static struct bt_uuid_128 env_svc_uuid = BT_UUID_INIT_128(
    0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0);
static struct bt_gatt_attr attrs[] = {
    BT_GATT_PRIMARY_SERVICE(&env_svc_uuid),
    BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE,
                           BT_GATT_CHRC_READ, BT_GATT_PERM_READ,
                           NULL, NULL, &temperature),
    BT_GATT_CHARACTERISTIC(BT_UUID_HUMIDITY,
                           BT_GATT_CHRC_READ, BT_GATT_PERM_READ,
                           NULL, NULL, &humidity),
};
static struct bt_gatt_service env_svc = BT_GATT_SERVICE(attrs);

// Função de inicialização do BLE
void ble_init() {
    int err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Erro ao inicializar BLE: %d", err);
        return;
    }
    bt_gatt_service_register(&env_svc);
    LOG_INF("BLE inicializado com sucesso");
}

// Função de inicialização dos sensores
void sensor_init() {
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C não está pronto");
        return;
    }

    // if (!device_is_ready(adc_dev)) {
    //     LOG_ERR("ADC não está pronto");
    //     return;
    // }
    LOG_INF("Sensores inicializados");
}

// Thread para leitura dos sensores
void sensor_thread(void *arg1, void *arg2, void *arg3) {
    while (1) {
        // Leitura dos sensores (exemplo para temperatura)
        // Aqui você pode usar funções específicas para o sensor que está utilizando.
        temperature += 0.5; // Simulação
        humidity += 0.2;    // Simulação
        sound_level = 300;  // Simulação
        light_intensity = 150; // Simulação

        LOG_INF("Temp: %.2f, Umid: %.2f, Som: %d, Luz: %d",
                temperature, humidity, sound_level, light_intensity);

        k_sleep(K_SECONDS(5));
    }
}

// Thread para envio BLE
void ble_thread(void *arg1, void *arg2, void *arg3) {
    while (1) {
        // Atualizar valores no GATT
        LOG_INF("Enviando dados BLE...");
        k_sleep(K_SECONDS(10));
    }
}

K_THREAD_DEFINE(sensor_tid, 2048, sensor_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(ble_tid, 2048, ble_thread, NULL, NULL, NULL, 5, 0, 0);

void main(void) {
    LOG_INF("Inicializando sistema...");
    // ble_init();
    sensor_init();
    LOG_INF("Sistema inicializado");
}