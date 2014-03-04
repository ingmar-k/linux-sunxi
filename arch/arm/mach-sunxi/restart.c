/*
 * Restart code for Allwinner SoCs
 *
 * Copyright (C) 2012-2014 Maxime Ripard
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/delay.h>
#include <linux/io.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/reboot.h>

#include "restart.h"

#define SUN4I_WATCHDOG_CTRL_REG		0x00
#define SUN4I_WATCHDOG_CTRL_RESTART		BIT(0)
#define SUN4I_WATCHDOG_MODE_REG		0x04
#define SUN4I_WATCHDOG_MODE_ENABLE		BIT(0)
#define SUN4I_WATCHDOG_MODE_RESET_ENABLE	BIT(1)

#define SUN6I_WATCHDOG1_IRQ_REG		0x00
#define SUN6I_WATCHDOG1_CTRL_REG	0x10
#define SUN6I_WATCHDOG1_CTRL_RESTART		BIT(0)
#define SUN6I_WATCHDOG1_CONFIG_REG	0x14
#define SUN6I_WATCHDOG1_CONFIG_RESTART		BIT(0)
#define SUN6I_WATCHDOG1_CONFIG_IRQ		BIT(1)
#define SUN6I_WATCHDOG1_MODE_REG	0x18
#define SUN6I_WATCHDOG1_MODE_ENABLE		BIT(0)

static void __iomem *wdt_base;

void sun4i_restart(enum reboot_mode mode, const char *cmd)
{
	if (!wdt_base)
		return;

	/* Enable timer and set reset bit in the watchdog */
	writel(SUN4I_WATCHDOG_MODE_ENABLE | SUN4I_WATCHDOG_MODE_RESET_ENABLE,
	       wdt_base + SUN4I_WATCHDOG_MODE_REG);

	/*
	 * Restart the watchdog. The default (and lowest) interval
	 * value for the watchdog is 0.5s.
	 */
	writel(SUN4I_WATCHDOG_CTRL_RESTART, wdt_base + SUN4I_WATCHDOG_CTRL_REG);

	while (1) {
		mdelay(5);
		writel(SUN4I_WATCHDOG_MODE_ENABLE | SUN4I_WATCHDOG_MODE_RESET_ENABLE,
		       wdt_base + SUN4I_WATCHDOG_MODE_REG);
	}
}

void sun6i_restart(enum reboot_mode mode, const char *cmd)
{
	if (!wdt_base)
		return;

	/* Disable interrupts */
	writel(0, wdt_base + SUN6I_WATCHDOG1_IRQ_REG);

	/* We want to disable the IRQ and just reset the whole system */
	writel(SUN6I_WATCHDOG1_CONFIG_RESTART,
		wdt_base + SUN6I_WATCHDOG1_CONFIG_REG);

	/* Enable timer. The default and lowest interval value is 0.5s */
	writel(SUN6I_WATCHDOG1_MODE_ENABLE,
		wdt_base + SUN6I_WATCHDOG1_MODE_REG);

	/* Restart the watchdog. */
	writel(SUN6I_WATCHDOG1_CTRL_RESTART,
		wdt_base + SUN6I_WATCHDOG1_CTRL_REG);

	while (1) {
		mdelay(5);
		writel(SUN6I_WATCHDOG1_MODE_ENABLE,
			wdt_base + SUN6I_WATCHDOG1_MODE_REG);
	}
}

static struct of_device_id sunxi_restart_ids[] = {
	{ .compatible = "allwinner,sun4i-a10-wdt" },
	{ .compatible = "allwinner,sun6i-a31-wdt" },
	{ /*sentinel*/ }
};

void sunxi_setup_restart(void)
{
	struct device_node *np;

	np = of_find_matching_node(NULL, sunxi_restart_ids);
	if (WARN(!np, "unable to setup watchdog restart"))
		return;

	wdt_base = of_iomap(np, 0);
	WARN(!wdt_base, "failed to map watchdog base address");
}
