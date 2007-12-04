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

/******************************************************************************
 * adhoc.c
 * PSPアドホック通信制御
 ******************************************************************************/

/******************************************************************************
 * ヘッダファイルの読込み
 ******************************************************************************/
#include "common.h"

/******************************************************************************
 * マクロ等の定義
 ******************************************************************************/
#define NUM_ENTRIES 16

#define MODE_LOBBY      0
#define MODE_P2P        1

#define PSP_LISTING     1
#define PSP_SELECTED    2
#define PSP_SELECTING   3
#define PSP_WAIT_EST    4
#define PSP_ESTABLISHED 5

#define ADHOC_BUFFER_SIZE 0x400
#define PDP_BUFFER_SIZE   (ADHOC_BUFFER_SIZE * 2)
#define PDP_PORT      (0x309)

#define ADHOC_TIMEOUT   30*1000000
#define ADHOC_BLOCKSIZE 0x400

#define PRODUCT "gpSP"

/*--------------------------------------------------------
  関数パラメータの定義
--------------------------------------------------------*/
#define MATCHING_CREATE_PARAMS  \
  3,                            \
  0xa,                          \
  0x22b,                        \
  0x800,                        \
  0x2dc6c0,                     \
  0x5b8d80,                     \
  3,                            \
  0x7a120,                      \
  matching_callback

#define MATCHING_START_PARAMS   \
  matching_id,                  \
  0x10,                         \
  0x2000,                       \
  0x10,                         \
  0x2000,                       \
  strlen(matching_data) + 1,    \
  (char *)matching_data

/***************************************************************************
 ローカル変数
 ***************************************************************************/

static int mode;
static int server;
static int pdp_id;

static unsigned char mac[16];
static unsigned char mymac[6];
static unsigned char ssid[8];
static u32 unk1;
static u32 match_event;
static u32 match_opt_len;
static char match_opt_data[1000];
static char matching_data[32];
static u32 match_changed;
static int matching_id;

static struct psplist_t
  {
    char name[48];
    char mac[6];
  } psplist[NUM_ENTRIES];

static int max;
static int pos;

static int adhoc_initialized = 0;
static unsigned char adhoc_buffer[ADHOC_BUFFER_SIZE];
static unsigned char adhoc_work[ADHOC_BUFFER_SIZE];

/***************************************************************************
 ローカル関数
 ***************************************************************************/

/*--------------------------------------------------------
 プログレスバー初期化
 --------------------------------------------------------*/

static void adhoc_init_progress(u32 total, char *text)
  {
    char buf[MAX_FILE];

    // 画面の設定
//    load_background(WP_LOGO);
//    video_copy_rect(work_frame, draw_frame, &full_rect, &full_rect);

    // アイコンの表示
//    small_icon(6, 3, UI_COLOR(UI_PAL_TITLE), ICON_SYSTEM);
    sprintf(buf, "AdHoc - %s", gamepak_filename);
    // 文字の表示
//    uifont_print(32, 5, UI_COLOR(UI_PAL_TITLE), buf);

    // 画面の設定
//    video_copy_rect(draw_frame, work_frame, &full_rect, &full_rect);

    // プログレスバーの表示
    init_progress(total, text);
  }

/*--------------------------------------------------------
 リスト消去
 --------------------------------------------------------*/

static void clear_psp_list(void)
{
  max = 0;
  pos = 0;
  memset(&psplist, 0, sizeof(psplist));
}

/*--------------------------------------------------------
 リストに追加
 --------------------------------------------------------*/

static u32 add_psp(char *l_mac, char *name, u32 length)
{
  u32 i;

  if (max == NUM_ENTRIES)
    return 0;
  if (length == 1)
    return 0;

  for (i = 0; i < max; i++)
  {
    if (memcmp(psplist[i].mac, l_mac, 6) == 0)
      return 0;
  }

  memcpy(psplist[max].mac, l_mac, 6);

  if (length)
  {
    if (length < 47)
      strcpy(psplist[max].name, name);
    else
      strncpy(psplist[max].name, name, 47);
  }
  else
  psplist[max].name[0] = '\0';

  max++;

  return 1;
}

/*--------------------------------------------------------
 リストから削除
 --------------------------------------------------------*/
static u32 del_psp(char *l_mac)
{
  u32 i, j;

  for (i = 0; i < max; i++)
  {
    if (memcmp(psplist[i].mac, l_mac, 6) == 0)
    {
      if (i != max - 1)
      {
        for (j = i + 1; j < max; j++)
        {
          memcpy(psplist[j - 1].mac, psplist[j].mac, 6);
          strcpy(psplist[j - 1].name, psplist[j].name);
        }
      }

      if (pos == i) pos = 0;
      if (pos > i) pos--;
      max--;

      return 0;
    }
  }

  return -1;
}

/*--------------------------------------------------------
 リストを表示
 --------------------------------------------------------*/

static void display_psp_list(u32 top, u32 rows)
  {
    if (max == 0)
      {
        // 文字表示
//        msg_printf(TEXT(WAITING_FOR_ANOTHER_PSP_TO_JOIN));
      }
    else
      {
        u32 i;
        char temp[20];

        // 画面表示
//        video_copy_rect(show_frame, draw_frame, &full_rect, &full_rect);

        scrollbar(470, 26, 479, 270, max, rows, pos);

        for (i = 0; i < rows; i++)
          {
            if ((top + i) >= max)
              break;

            sceNetEtherNtostr((u8 *)psplist[top + i].mac, temp);

            if ((top + i) == pos)
              {
                // 文字表示
//                uifont_print(24, 40 + (i + 2) * 17, UI_COLOR(UI_PAL_SELECT), temp);
//                uifont_print(190, 40 + (i + 2) * 17, UI_COLOR(UI_PAL_SELECT), psplist[top + i].name);
              }
            else
              {
                // 文字表示
//                uifont_print(24, 40 + (i + 2) * 17, UI_COLOR(UI_PAL_NORMAL), temp);
//                uifont_print(190, 40 + (i + 2) * 17, UI_COLOR(UI_PAL_NORMAL), psplist[top + i].name);
              }
          }

        // 画面切替
        flip_screen();
      }
  }

/*--------------------------------------------------------
 選択中のPSPの情報を取得
 --------------------------------------------------------*/

static u32 GetPspEntry(char *mac, char *name)
  {
    if (max == 0)
      return -1;

    memcpy(mac, psplist[pos].mac, 6);
    strcpy(name, psplist[pos].name);

    return 1;
  }

/*--------------------------------------------------------
 Matching callback
 --------------------------------------------------------*/

static void matchingCallback(int unk1, int event, char *mac2, int optLen, char *optData)
  {
    switch (event)
      {
        case MATCHING_JOINED:
          add_psp(mac2, optData, optLen);
          break;

        case MATCHING_DISCONNECT:
          del_psp(mac2);
          break;

        default:
          unk1 = unk1;
          match_event = event;
          match_opt_len = optLen;
          strncpy(match_opt_data, optData, optLen);
          memcpy(mac, mac2, sizeof(char) * 6);
          match_changed = 1;
          break;
      }
  }

/***************************************************************************
 AdHocインタフェース関数
 ***************************************************************************/

/*--------------------------------------------------------
 モジュールのロード
 --------------------------------------------------------*/

u32 load_adhoc_modules(void)
{
  if (sceKernelDevkitVersion() >= 0x02000010)
  {
    int error;

    if ((error = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON)) < 0)
      return error;

    if ((error = sceUtilityLoadNetModule(PSP_NET_MODULE_ADHOC)) < 0)
      return error;

    return 0;
  }
  return -1;
}

/*--------------------------------------------------------
 初期化
 --------------------------------------------------------*/

u32 adhoc_init(const char *l_matching_data)
{
  struct productStruct product;
  int error = 0, state = 0;
  unsigned char mac[6];
  const char *unknown = "";
  char message[256];

  mode = MODE_LOBBY;
  server = 0;
  adhoc_initialized = 0;

  unk1 = 0;
  match_event = 0;
  match_opt_len = 0;
  match_changed = 0;
  memset(mac, 0, sizeof(mac));
  memset(mymac, 0, sizeof(mymac));

  sprintf((char *)product.product, PRODUCT "00%d%d%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
  product.unknown = 0;

  clear_psp_list();

  if (strlen(l_matching_data) == 0)
    return -1;

  strcpy(matching_data, l_matching_data);

  sprintf(message, "CONNECTING_TO_%s", "LOBBY");
  adhoc_init_progress(10, message);

  if ((error = sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000)) == 0)
  {
    update_progress();
    if ((error = sceNetAdhocInit()) == 0)
    {
      update_progress();
      if ((error = sceNetAdhocctlInit(0x2000, 0x20, &product)) == 0)
      {
        update_progress();
        if ((error = sceNetAdhocctlConnect((int *)unknown)) == 0)
        {
          update_progress();
          do
          {
            if ((error = sceNetAdhocctlGetState(&state)) != 0) break;
              sceKernelDelayThread(1000000/60);
          }while (state != 1);
          if (!error)
          {
            update_progress();
            sceWlanGetEtherAddr(mac);
            update_progress();
            if ((pdp_id = sceNetAdhocPdpCreate(mac, PDP_PORT, PDP_BUFFER_SIZE, 0)) > 0)
            {
              update_progress();
              if ((error = sceNetAdhocMatchingInit(0x20000)) == 0)
              {
                update_progress();
                if ((matching_id = sceNetAdhocMatchingCreate(MATCHING_CREATE_PARAMS)) >= 0)
                {
                  update_progress();
                  if ((error = sceNetAdhocMatchingStart(MATCHING_START_PARAMS)) == 0)
                  {
                    update_progress();
                    show_progress("CONNECTED");
                    return 0;
                  }
                  sceNetAdhocMatchingDelete(matching_id);
                }
                error = 2;
                sceNetAdhocMatchingTerm();
              }
              sceNetAdhocPdpDelete(pdp_id, 0);
            }
            error = 1;
          }
        sceNetAdhocctlDisconnect();
      }
      sceNetAdhocctlTerm();
    }
    sceNetAdhocTerm();
  }
  sceNetTerm();
}

    switch (error)
      {
        case 1: sprintf(message, "%s (PDP ID = %08x)", "failed", pdp_id); break;
        case 2: sprintf(message, "%s (Matching ID = %08x)", "failed", matching_id); break;
        default: sprintf(message, "%s (Error Code = %08x)", "failed", error); break;
      }

    show_progress(message);
    error_msg("");
    return -1;
  }

/*--------------------------------------------------------
 切断
 --------------------------------------------------------*/

u32 adhoc_term(void)
{
  if (adhoc_initialized > 0)
  {
    char message[256];

    adhoc_init_progress(5, "DISCONNECTING %s", server ? "Client" : "Server");

    sceNetAdhocctlDisconnect();
    update_progress();

    sceNetAdhocPdpDelete(pdp_id, 0);
    update_progress();

    sceNetAdhocctlTerm();
    update_progress();

    sceNetAdhocTerm();
    update_progress();

    sceNetTerm();
    update_progress();

    show_progress("DISCONNECTED");

    adhoc_initialized = 0;
  }

  return 0;
}

/*--------------------------------------------------------
 ロビーから切断
 --------------------------------------------------------*/

static void adhoc_disconnect(void)
{
  char message[256];

  sprintf(message, "DISCONNECTING_FROM_%s", "LOBBY");
  adhoc_init_progress(8, message);

  sceNetAdhocMatchingStop(matching_id);
  update_progress();

  sceNetAdhocMatchingDelete(matching_id);
  update_progress();

  sceNetAdhocMatchingTerm();
  update_progress();

  sceNetAdhocctlDisconnect();
  update_progress();

  sceNetAdhocPdpDelete(pdp_id, 0);
  update_progress();

  sceNetAdhocctlTerm();
  update_progress();

  sceNetAdhocTerm();
  update_progress();

  sceNetTerm();
  update_progress();

  show_progress("DISCONNECTED");

  adhoc_initialized = 0;
}

/*--------------------------------------------------------
  ロビーから切断し、P2P開始
--------------------------------------------------------*/

static int adhoc_start_p2p(void)
{
  int error = 0, state = 1;
  unsigned char mac[6];
  char message[256];

  sprintf(message, "DISCONNECTING_FROM_%s", "LOBBY");
  adhoc_init_progress(6, message);

  sceNetAdhocMatchingStop(matching_id);
  update_progress();

  sceNetAdhocMatchingDelete(matching_id);
  update_progress();

  sceNetAdhocMatchingTerm();
  update_progress();

  sceNetAdhocPdpDelete(pdp_id, 0);
  update_progress();

  sceNetAdhocctlDisconnect();
  update_progress();

  do
  {
    if ((error = sceNetAdhocctlGetState(&state)) != 0) break;
      sceKernelDelayThread(1000000/60);
  } while (state == 1);

  update_progress();
  show_progress("DISCONNECTED");

  mode = MODE_P2P;
  sprintf(message, "CONNECTING_TO_%s", server ? "CLIENT" : "SERVER");
  adhoc_init_progress(4, message);

  if ((error = sceNetAdhocctlConnect((int *)ssid)) == 0)
  {
    update_progress();
    do
    {
      if ((error = sceNetAdhocctlGetState(&state)) != 0) break;
      sceKernelDelayThread(1000000/60);
    } while (state != 1);

    if (!error)
    {
      update_progress();

      sceWlanGetEtherAddr(mac);
      memcpy(mymac, mac, 6);
      update_progress();

      if ((pdp_id = sceNetAdhocPdpCreate(mac, PDP_PORT, PDP_BUFFER_SIZE, 0)) > 0)
      {
        update_progress();
        adhoc_initialized = 2;

        show_progress("WAITING_FOR_SYNCHRONIZATION");
        if ((error = adhoc_sync()) == 0)
          return server;
      }
      else
      {
        error = 1;
      }
    }
    sceNetAdhocctlDisconnect();

    if (state == 1)
    {
      do
      {
        if ((error = sceNetAdhocctlGetState(&state)) != 0) break;
        sceKernelDelayThread(1000000/60);
      } while (state == 1);
    }
  }

  sceNetAdhocctlTerm();
  sceNetAdhocTerm();
  sceNetTerm();

  adhoc_initialized = 0;

  switch (error)
  {
  case 1:  sprintf(message, "%s (PDP ID = %08x)", "FAILED", pdp_id); break;
  default: sprintf(message, "%s (Error Code = %08x)", "FAILED", error); break;
  }

  show_progress(message);

  err_msg("");

  return -1;
}

/*--------------------------------------------------------
  接続先の選択
--------------------------------------------------------*/

int adhocSelect(void)
{
  int top = 0;
  int rows = 11;
  int currentState = PSP_LISTING;
  int prev_max = 0;
  int update = 1;
  unsigned char mac[6];
  char name[64];
  char temp[64];
  char title[32];

  sprintf(title, "AdHoc - %s", game_name);
//  msg_screen_init(WP_LOGO, ICON_SYSTEM, title);

  while (1)
  {
    pad_update();

//    msg_set_text_color(0xffff);

    switch (currentState)
    {
    case PSP_LISTING:
      server = 0;
      if (update)
      {
        msg_screen_init(WP_LOGO, ICON_SYSTEM, title);
        msg_printf(TEXT(SELECT_A_SERVER_TO_CONNECT_TO));
        msg_printf("\n");
        display_psp_list(top, rows);
        update = 0;
      }
      if (pad_pressed(PSP_CTRL_UP))
      {
        if (pos > 0) pos--;
        update = 1;
      }
      else if (pad_pressed(PSP_CTRL_DOWN))
      {
        if (pos < max - 1) pos++;
        update = 1;
      }
      else if (pad_pressed(PSP_CTRL_CIRCLE))
      {
        if (GetPspEntry(mac, name) > 0)
        {
          if (strcmp(name, g_matchingData) == 0)
          {
            currentState = PSP_SELECTING;
            sceNetAdhocMatchingSelectTarget(matchingId, mac, 0, NULL);
            update = 1;
          }
        }
      }
      else if (pad_pressed(PSP_CTRL_TRIANGLE))
      {
        msg_set_text_color(0xffffffff);
        adhocDisconnect();
        pad_wait_clear();
        return -1;
      }
      if (matchChanged)
      {
        if (g_matchEvent == MATCHING_SELECTED)
        {
          memcpy(mac, g_mac, 6);
          strcpy(name, g_matchOptData);
          currentState = PSP_SELECTED;
        }
        update = 1;
      }
      break;

    case PSP_SELECTING:
      if (update)
      {
        msg_screen_init(WP_LOGO, ICON_SYSTEM, title);
        sceNetEtherNtostr(mac, temp);
        msg_printf(TEXT(WAITING_FOR_x_TO_ACCEPT_THE_CONNECTION), temp);
        msg_printf(TEXT(TO_CANCEL_PRESS_CROSS));
        update = 0;
      }
      if (pad_pressed(PSP_CTRL_CROSS))
      {
        sceNetAdhocMatchingCancelTarget(matchingId, mac);
        currentState = PSP_LISTING;
        update = 1;
      }
      if (matchChanged)
      {
        switch (g_matchEvent)
        {
        case MATCHING_SELECTED:
          sceNetAdhocMatchingCancelTarget(matchingId, mac);
          break;

        case MATCHING_ESTABLISHED:
          currentState = PSP_ESTABLISHED;
          break;

        case MATCHING_REJECTED:
          currentState = PSP_LISTING;
          break;
        }
        update = 1;
      }
      break;

    case PSP_SELECTED:
      Server = 1;
      if (update)
      {
        msg_screen_init(WP_LOGO, ICON_SYSTEM, title);
        sceNetEtherNtostr(mac, temp);
        msg_printf(TEXT(x_HAS_REQUESTED_A_CONNECTION), temp);
        msg_printf(TEXT(TO_ACCEPT_THE_CONNECTION_PRESS_CIRCLE_TO_CANCEL_PRESS_CIRCLE));
        update = 0;
      }
      if (pad_pressed(PSP_CTRL_CROSS))
      {
        sceNetAdhocMatchingCancelTarget(matchingId, mac);
        currentState = PSP_LISTING;
        update = 1;
      }
      else if (pad_pressed(PSP_CTRL_CIRCLE))
      {
        sceNetAdhocMatchingSelectTarget(matchingId, mac, 0, NULL);
        currentState = PSP_WAIT_EST;
        update = 1;
      }
      if (matchChanged)
      {
        if (g_matchEvent == MATCHING_CANCELED)
        {
          currentState = PSP_LISTING;
        }
        update = 1;
      }
      break;

    case PSP_WAIT_EST:
      if (matchChanged)
      {
        if (g_matchEvent == MATCHING_ESTABLISHED)
        {
          currentState = PSP_ESTABLISHED;
        }
        update = 1;
      }
      break;
    }

    matchChanged = 0;
    if (currentState == PSP_ESTABLISHED)
      break;

    if (top > max - rows) top = max - rows;
    if (top < 0) top = 0;
    if (pos >= top + rows) top = pos - rows + 1;
    if (pos < top) top = pos;

    if (max != prev_max)
    {
      prev_max = max;
      update = 1;
    }

    sceDisplayWaitVblankStart();
  }

  msg_set_text_color(0xffffffff);

  if (Server) sceWlanGetEtherAddr(mac);

  sceNetEtherNtostr(mac, temp);

  g_ssid[0] = temp[ 9];
  g_ssid[1] = temp[10];
  g_ssid[2] = temp[12];
  g_ssid[3] = temp[13];
  g_ssid[4] = temp[15];
  g_ssid[5] = temp[16];
  g_ssid[6] = '\0';

  return adhocStartP2P();
}


/*--------------------------------------------------------
  データを送信
--------------------------------------------------------*/

int adhocSend(void *buffer, int length, int type)
{
  int error;

  memset(adhoc_buffer, 0, ADHOC_BUFFER_SIZE);

  adhoc_buffer[0] = type;
  memcpy(&adhoc_buffer[1], buffer, length);

  if ((error = sceNetAdhocPdpSend(pdpId, g_mac, PDP_PORT, adhoc_buffer, length + 1, 0, 1)) < 0)
    return error;

  return length;
}


/*--------------------------------------------------------
  データを受信
--------------------------------------------------------*/

int adhocRecv(void *buffer, int timeout, int type)
{
  int error;
  int length = ADHOC_BUFFER_SIZE;
  unsigned short port = 0;
  unsigned char mac[6];

  memset(adhoc_buffer, 0, ADHOC_BUFFER_SIZE);

  if ((error = sceNetAdhocPdpRecv(pdpId, mac, &port, adhoc_buffer, &length, timeout, 0)) < 0)
    return error;

  if (adhoc_buffer[0] & type)
  {
    memcpy(buffer, &adhoc_buffer[1], length - 1);
    return length - 1;
  }

  return -1;
}


/*--------------------------------------------------------
  データを送信し、ackを受信するまで待つ
--------------------------------------------------------*/

int adhocSendRecvAck(void *buffer, int length, int timeout, int type)
{
  int temp_length = length;
  int sent_length = 0;
  int error = 0;
  unsigned char *buf = (unsigned char *)buffer;

  do
  {
    if (temp_length > ADHOC_BUFFER_SIZE - 1)
      temp_length = ADHOC_BUFFER_SIZE - 1;

    adhocSend(buf, temp_length, type);

    if ((error = adhocRecv(adhoc_work, timeout, ADHOC_DATATYPE_ACK)) != 4)
      return error;

    if (*(int *)adhoc_work != sent_length + temp_length)
      return -1;

    buf += temp_length;
    sent_length += temp_length;
    temp_length = length - sent_length;

  } while (sent_length < length);

  return sent_length;
}


/*--------------------------------------------------------
  データの受信を待ち、ackを送信する
--------------------------------------------------------*/

int adhocRecvSendAck(void *buffer, int length, int timeout, int type)
{
  int temp_length = length;
  int rcvd_length = 0;
  int error = 0;
  unsigned char *buf = (unsigned char *)buffer;

  do
  {
    if (temp_length > ADHOC_BUFFER_SIZE - 1)
      temp_length = ADHOC_BUFFER_SIZE - 1;

    if ((error = adhocRecv(buf, timeout, type)) != temp_length)
      return error;

    *(int *)adhoc_work = rcvd_length + temp_length;
    adhocSend(adhoc_work, 4, ADHOC_DATATYPE_ACK);

    buf += temp_length;
    rcvd_length += temp_length;
    temp_length = length - rcvd_length;

  } while (rcvd_length < length);

  return rcvd_length;
}


/*--------------------------------------------------------
  相手との同期を待つ
--------------------------------------------------------*/

int adhocSync(void)
{
  int size = 0;
  int retry = 60;

  if (Server)
  {
    while (retry--)
    {
      adhocSend(adhoc_work, 1, ADHOC_DATATYPE_SYNC);

      if (adhocRecv(adhoc_work, 1000000, ADHOC_DATATYPE_SYNC) == 1)
        goto check_packet;
    }
  }
  else
  {
    while (retry--)
    {
      if (adhocRecv(adhoc_work, 1000000, ADHOC_DATATYPE_SYNC) == 1)
      {
        adhocSend(adhoc_work, 1, ADHOC_DATATYPE_SYNC);
        goto check_packet;
      }
    }
  }

  return -1;

check_packet:
  while (1)
  {
    pdpStatStruct pdpStat;

    size = sizeof(pdpStat);

    if (sceNetAdhocGetPdpStat(&size, &pdpStat) >= 0)
    {
      // 余分なパケットを破棄
      if (pdpStat.rcvdData == ADHOC_DATASIZE_SYNC)
        adhocRecv(adhoc_work, 0, ADHOC_DATATYPE_SYNC);
      else
        break;
    }

    if (Loop != LOOP_EXEC) return 0;

    sceKernelDelayThread(100);
  }

  return 0;
}


/*--------------------------------------------------------
  指定サイズのデータを受信するか、バッファが空に
  なるまで待つ
--------------------------------------------------------*/

void adhocWait(int data_size)
{
  pdpStatStruct pdpStat;
  int size = sizeof(pdpStat);

  if (data_size > ADHOC_BUFFER_SIZE)
    data_size = ADHOC_BUFFER_SIZE;

  while (1)
  {
    if (sceNetAdhocGetPdpStat(&size, &pdpStat) >= 0)
    {
      if (pdpStat.rcvdData == 0 || (int)pdpStat.rcvdData == data_size)
        break;
      else
        adhocRecv(adhoc_work, 0, ADHOC_DATATYPE_ANY);
    }

    if (Loop != LOOP_EXEC) break;

    sceKernelDelayThread(100);
  }
}

