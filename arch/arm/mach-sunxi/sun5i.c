/*
 * Device Tree support for Allwinner A20 SoCs
 *
 * Copyright (C) 2013-2014 Maxime Ripard
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/init.h>
#include <linux/of_platform.h>

#include <asm/mach/arch.h>

#include "restart.h"

static void __init sun5i_dt_init(void)
{
	sunxi_setup_restart();

	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static const char * const sun5i_board_dt_compat[] = {
	"allwinner,sun5i-a10s",
	"allwinner,sun5i-a13",
	NULL,
};

DT_MACHINE_START(SUN5I_DT, "Allwinner sun5i (A13/A10s) Family")
	.init_machine	= sun5i_dt_init,
	.dt_compat	= sun5i_board_dt_compat,
	.restart	= sun4i_restart,
MACHINE_END
