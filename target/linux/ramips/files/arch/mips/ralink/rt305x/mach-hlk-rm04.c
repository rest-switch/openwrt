/*
 *  HiLink HLK-RM04 board support
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

#include <asm/mach-ralink/machine.h>
#include <asm/mach-ralink/dev-gpio-leds.h>
#include <asm/mach-ralink/dev-gpio-buttons.h>
#include <asm/mach-ralink/rt305x.h>
#include <asm/mach-ralink/rt305x_regs.h>

#include "devices.h"

#define HLK_RM04_GPIO_LED_POWER     	0

#define HLK_RM04_GPIO_BUTTON_WPS	0	/* active low */
#define HLK_RM04_GPIO_BUTTON_RESET	10	/* active low */

#define HLK_RM04_KEYS_POLL_INTERVAL	20	/* ms */
#define HLK_RM04_KEYS_DEBOUNCE_INTERVAL	(3 * HLK_RM04_KEYS_POLL_INTERVAL)

const struct flash_platform_data hlk_rm04_flash = {
		.type			= "w25q32",
};

struct spi_board_info hlk_rm04_spi_slave_info[] __initdata = {
	{
		.modalias		= "m25p80",
		.platform_data		= &hlk_rm04_flash,
		.irq			= -1,
		.max_speed_hz		= 10000000,
		.bus_num		= 0,
		.chip_select		= 0,
	},
};

static struct gpio_led hlk_rm04_leds_gpio[] __initdata = {
	{
		.name			= "hlk-rm04:red:power",
		.gpio			= HLK_RM04_GPIO_LED_POWER,
		.active_low		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_ON,
	},
};

static struct gpio_keys_button hlk_rm04_gpio_buttons[] __initdata = {
	{
		.desc			= "reset",
		.type			= EV_KEY,
		.code			= KEY_RESTART,
		.debounce_interval 	= HLK_RM04_KEYS_DEBOUNCE_INTERVAL,
		.gpio			= HLK_RM04_GPIO_BUTTON_RESET,
		.active_low		= 1,
	}, {
		.desc			= "wps",
		.type			= EV_KEY,
		.code			= KEY_WPS_BUTTON,
		.debounce_interval 	= HLK_RM04_KEYS_DEBOUNCE_INTERVAL,
		.gpio			= HLK_RM04_GPIO_BUTTON_WPS,
		.active_low		= 1,
	},
};

static void __init hlk_rm04_init(void)
{
	rt305x_gpio_init((RT305X_GPIO_MODE_GPIO << RT305X_GPIO_MODE_UART0_SHIFT) | 
				RT305X_GPIO_MODE_JTAG);

	rt305x_register_spi(hlk_rm04_spi_slave_info, ARRAY_SIZE(hlk_rm04_spi_slave_info));

	ramips_register_gpio_leds(-1, ARRAY_SIZE(hlk_rm04_leds_gpio),
					hlk_rm04_leds_gpio);

	ramips_register_gpio_buttons(-1, HLK_RM04_KEYS_POLL_INTERVAL, 
					ARRAY_SIZE(hlk_rm04_gpio_buttons), hlk_rm04_gpio_buttons);

	rt305x_esw_data.vlan_config = RT305X_ESW_VLAN_CONFIG_NONE;
	rt305x_register_ethernet();
	rt305x_register_wifi();

	rt305x_register_wdt();
}

MIPS_MACHINE(RAMIPS_MACH_HLK_RM04, "HLK-RM04", "HiLink HLK-RM04", hlk_rm04_init);
