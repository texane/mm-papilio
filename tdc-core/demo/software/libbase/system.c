/*
 * Milkymist SoC (Software)
 * Copyright (C) 2007, 2008, 2009 Sebastien Bourdeauducq
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <irq.h>
#include <uart.h>
#include <hw/sysctl.h>

#include <system.h>

__attribute__((noreturn)) void reboot()
{
	uart_force_sync(1); /* flush UART buffers */
	irq_setmask(0);
	irq_enable(0);
	CSR_SYSTEM_ID = 1; /* Writing to CSR_SYSTEM_ID causes a system reset */
	while(1);
}
