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

#ifndef _SUNXI_RESTART_H_
#define _SUNXI_RESTART_H_

void sun4i_restart(enum reboot_mode mode, const char *cmd);
void sun6i_restart(enum reboot_mode mode, const char *cmd);
void sunxi_setup_restart(void);

#endif
