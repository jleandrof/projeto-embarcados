#include <zephyr/shell/shell.h>

int sensor_pulling = 0;

static int cmd_set_limit(const struct shell *shell, size_t argc, char **argv) {
    if (argc != 3) {
        shell_print(shell, "Uso: set_limit <sensor> <valor>");
        return -EINVAL;
    }
    shell_print(shell, "Limite ajustado para %s: %s", argv[1], argv[2]);
    return 0;
}

static int cmd_show_sensor_log(const struct shell *shell, size_t argc, char **argv) {
    if (argc != 1) {
        shell_print(shell, "Uso: set_limit <sensor> <valor>");
        return -EINVAL;
    }

    sensor_pulling = 1;

    shell_print(shell, "Showing sensor log");
    return 0;
}

static int cmd_hide_sensor_log(const struct shell *shell, size_t argc, char **argv) {
    if (argc != 1) {
        shell_print(shell, "Uso: set_limit <sensor> <valor>");
        return -EINVAL;
    }

    sensor_pulling = 0;

    shell_print(shell, "Sensor log hidden");
    return 0;
}

// SHELL_CMD_REGISTER(set_limit, NULL, "Ajustar limites de sensores", cmd_set_limit);
SHELL_CMD_REGISTER(show_sensor_log, NULL, "Exibe logs do sensor de ambiente", cmd_show_sensor_log);
SHELL_CMD_REGISTER(hide_sensor_log, NULL, "Esconde logs do sensor de ambiente", cmd_hide_sensor_log);