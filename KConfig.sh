#!/bin/bash

# 获取脚本所在绝对路径
workDir=$(dirname "$(readlink -f "$0")")



# 初始化命令
cmdline="last"

rm "./Include/auto.h" 

# 读取配置文件（添加安全检测）
config_file="$workDir/.config"
if [ ! -f "$config_file" ]; then
    echo "ERROR! 配置文件 $config_file 不存在" >&2
    exit 1
elif [ ! -r "$config_file" ]; then
    echo "ERROR! 无法读取配置文件 $config_file" >&2
    exit 1
fi
source "$config_file" || exit 1


echo "// Auto Create" > "./Include/auto.h" 

echo "#define Memory_SIZE " $CONFIG_SELECT_CONFIG_MEMORY_SIZE >>  "./Include/auto.h" 

echo "#define Memory_BASE " $CONFIG_SELECT_CONFIG_MBASE >>  "./Include/auto.h" 

echo "#define SRAM_SIZE " $CONFIG_SELECT_CONFIG_SRAM_SIZE >>  "./Include/auto.h" 

echo "#define SRAM_BASE " $CONFIG_SELECT_CONFIG_SRAM_BASE >>  "./Include/auto.h" 

echo "#define FLASH_SIZE " $CONFIG_SELECT_CONFIG_FLASH_SIZE >>  "./Include/auto.h" 

echo "#define FLASH_BASE " $CONFIG_SELECT_CONFIG_FLASH_BASE >>  "./Include/auto.h" 

echo "#define MROM_SIZE " $CONFIG_SELECT_CONFIG_MROM_SIZE >>  "./Include/auto.h" 

echo "#define MROM_BASE " $CONFIG_SELECT_CONFIG_MROM_BASE >>  "./Include/auto.h" 

echo "#define UART_SIZE " $CONFIG_SELECT_CONFIG_UART_SIZE >>  "./Include/auto.h" 

echo "#define UART_BASE " $CONFIG_SELECT_CONFIG_UART_BASE >>  "./Include/auto.h" 

echo "#define CLINT_SIZE " $CONFIG_SELECT_CONFIG_CLINT_SIZE >>  "./Include/auto.h" 

echo "#define CLINT_BASE " $CONFIG_SELECT_CONFIG_CLINT_BASE >>  "./Include/auto.h" 

echo "#define RESET_VECTOR " $CONFIG_SELECT_CONFIG_PC_INIT >>  "./Include/auto.h" 

echo "#define DEVICE_TREE_ADDR " $CONFIG_SELECT_CONFIG_DEVICETREEADDR >>  "./Include/auto.h" 

echo "#define HART_ID " $CONFIG_SELECT_CONFIG_HART_ID >>  "./Include/auto.h" 

if [ "${CONFIG_SELECT_START_MODE_M:-n}" = "y" ]; then
    echo "#define START_MODE_M" >>  "./Include/auto.h" 
fi

if [ "${CONFIG_SELECT_START_MODE_S:-n}" = "y" ]; then
    echo "#define START_MODE_S" >>  "./Include/auto.h" 
fi

if [ "${CONFIG_SELECT_START_MODE_U:-n}" = "y" ]; then
    echo "#define START_MODE_M" >>  "./Include/auto.h" 
fi

if [ "${CONFIG_DIFFTEST_ON:-n}" = "y" ]; then
    echo "#define DIFFTEST_ON" >>  "./Include/auto.h" 
fi
