#include <zephyr/shell/shell.h>

static int cmd_set_limit(const struct shell *shell, size_t argc, char **argv) {
    if (argc != 3) {
        shell_print(shell, "Uso: set_limit <sensor> <valor>");
        return -EINVAL;
    }
    shell_print(shell, "Limite ajustado para %s: %s", argv[1], argv[2]);
    return 0;
}

SHELL_CMD_REGISTER(set_limit, NULL, "Ajustar limites de sensores", cmd_set_limit);