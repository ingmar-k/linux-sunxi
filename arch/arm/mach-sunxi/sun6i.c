/*
 * Device Tree support for Allwinner A31 SoCs
 *
 * Copyright (C) 2013-2014 Maxime Ripard
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/clk-provider.h>
#include <linux/clocksource.h>
#include <linux/init.h>
#include <linux/of_platform.h>

#include <asm/mach/arch.h>

#include "common.h"
#include "restart.h"

static void __init sun6i_dt_init(void)
{
	sunxi_setup_restart();

	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

extern void __init sun6i_reset_init(void);
static void __init sun6i_timer_init(void)
{
	of_clk_init(NULL);
	sun6i_reset_init();
	clocksource_of_init();
}

static const char * const sun6i_board_dt_compat[] = {
	"allwinner,sun6i-a31",
	NULL,
};

DT_MACHINE_START(SUN6I_DT, "Allwinner sun6i (A31) Family")
	.init_machine	= sun6i_dt_init,
	.init_time	= sun6i_timer_init,
	.dt_compat	= sun6i_board_dt_compat,
	.restart	= sun6i_restart,
	.smp		= smp_ops(sun6i_smp_ops),
MACHINE_END
