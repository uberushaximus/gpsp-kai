/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 NJ
 * Copyright (C) 2007 takka <takka@tfact.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef ADHOC_H
#define ADHOC_H

/******************************************************************************
 * グローバル関数の宣言
 ******************************************************************************/
u32 pspSdkLoadAdhocModules(void);

u32 adhocInit(const char *matchingData);
u32 adhocTerm(void);
u32 adhocSelect(void);
u32 adhocReconnect(char *ssid);

u32 adhocSend(void *buffer, u32 length);
u32 adhocRecv(void *buffer, u32 length);

u32 adhocSendBlocking(void *buffer, u32 length);
u32 adhocRecvBlocking(void *buffer, u32 length);
u32 adhocRecvBlockingTimeout(void *buffer, u32 length, u32 timeout);

u32 adhocSendRecvAck(void *buffer, u32 length);
u32 adhocRecvSendAck(void *buffer, u32 length);

#endif
