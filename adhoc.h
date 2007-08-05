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
int pspSdkLoadAdhocModules(void);

int adhocInit(const char *matchingData);
int adhocTerm(void);
int adhocSelect(void);
int adhocReconnect(char *ssid);

int adhocSend(void *buffer, int length);
int adhocRecv(void *buffer, int length);

int adhocSendBlocking(void *buffer, int length);
int adhocRecvBlocking(void *buffer, int length);
int adhocRecvBlockingTimeout(void *buffer, int length, int timeout);

int adhocSendRecvAck(void *buffer, int length);
int adhocRecvSendAck(void *buffer, int length);

#endif
