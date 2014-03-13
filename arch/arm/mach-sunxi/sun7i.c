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

#include <linux/clk.h>
#include <linux/init.h>
#include <linux/of_platform.h>

#include <asm/mach/arch.h>

#include "restart.h"

static void __init sun7i_dt_init(void)
{
	struct clk *clk;

	sunxi_setup_restart();

	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);

	/* Make sure the clocks we absolutely need are enabled */
	/* Memory bus clock */
	clk = clk_get(NULL, "mbus");
	if (!IS_ERR(clk))
		clk_prepare_enable(clk);

	/* DDR clock */
	clk = clk_get(NULL, "pll5_ddr");
	if (!IS_ERR(clk))
		clk_prepare_enable(clk);
}

static const char * const sun7i_board_dt_compat[] = {
	"allwinner,sun7i-a20",
	NULL,
};

DT_MACHINE_START(SUN7I_DT, "Allwinner sun7i (A20) Family")
	.init_machine	= sun7i_dt_init,
	.dt_compat	= sun7i_board_dt_compat,
	.restart	= sun4i_restart,
MACHINE_END
