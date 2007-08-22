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
#define NUM_ENTRIES 32

#define PSP_LISTING     1
#define PSP_SELECTED    2
#define PSP_SELECTING   3
#define PSP_WAIT_EST    4
#define PSP_ESTABLISHED 5

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
  matchingCallback

#define MATCHING_START_PARAMS   \
  matchingId,                   \
  0x10,                         \
  0x2000,                       \
  0x10,                         \
  0x2000,                       \
  strlen(matchingData) + 1,     \
  (char *)matchingData

/***************************************************************************
 ローカル変数
 ***************************************************************************/

static int Server;
static int pdpId;

static char g_mac[16];
static char g_mymac[16];
static int g_unk1;
static int g_matchEvent;
static int g_matchOptLen;
static char g_matchOptData[1000];
static char g_matchingData[32];
static int matchChanged;
static int matchingId;

static struct psplist_t
  {
    char name[48];
    char mac[6];
  } psplist[NUM_ENTRIES];

static int adhoc_max;
static int ahdoc_pos;

/***************************************************************************
 ローカル関数
 ***************************************************************************/

/*--------------------------------------------------------
 プログレスバー初期化
 --------------------------------------------------------*/

static void adhoc_init_progress(int total, const char *text)
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

static void ClearPspList(void)
  {
    adhoc_max = 0;
    ahdoc_pos = 0;
    memset(&psplist, 0, sizeof(psplist));
  }

/*--------------------------------------------------------
 リストに追加
 --------------------------------------------------------*/

static int AddPsp(char *mac, char *name, int length)
  {
    int i;

    if (adhoc_max == NUM_ENTRIES)
      return 0;
    if (length == 1)
      return 0;

    for (i = 0; i < adhoc_max; i++)
      {
        if (memcmp(psplist[i].mac, mac, 6) == 0)
          return 0;
      }

    memcpy(psplist[adhoc_max].mac, mac, 6);

    if (length)
      {
        if (length < 47)
          strcpy(psplist[adhoc_max].name, name);
        else
          strncpy(psplist[adhoc_max].name, name, 47);
      }
    else
      psplist[adhoc_max].name[0] = '\0';

    adhoc_max++;

    return 1;
  }

/*--------------------------------------------------------
 リストから削除
 --------------------------------------------------------*/

static int DelPsp(char *mac)
  {
    int i, j;

    for (i = 0; i < adhoc_max; i++)
      {
        if (memcmp(psplist[i].mac, mac, 6) == 0)
          {
            if (i != adhoc_max - 1)
              {
                for (j = i + 1; j < adhoc_max; j++)
                  {
                    memcpy(psplist[j - 1].mac, psplist[j].mac, 6);
                    strcpy(psplist[j - 1].name, psplist[j].name);
                  }
              }

            if (ahdoc_pos == i)
              ahdoc_pos = 0;
            if (ahdoc_pos > i)
              ahdoc_pos--;
            adhoc_max--;

            return 0;
          }
      }

    return -1;
  }

/*--------------------------------------------------------
 リストを表示
 --------------------------------------------------------*/

static void DisplayPspList(int top, int rows)
  {
    if (adhoc_max == 0)
      {
        // 文字表示
//        msg_printf(TEXT(WAITING_FOR_ANOTHER_PSP_TO_JOIN));
      }
    else
      {
        int i;
        char temp[20];

        // 画面表示
//        video_copy_rect(show_frame, draw_frame, &full_rect, &full_rect);

//        draw_scrollbar(470, 26, 479, 270, rows, max, pos);

        for (i = 0; i < rows; i++)
          {
            if ((top + i) >= adhoc_max)
              break;

            sceNetEtherNtostr((u8 *)psplist[top + i].mac, temp);

            if ((top + i) == ahdoc_pos)
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
//        video_flip_screen(1);
      }
  }

/*--------------------------------------------------------
 選択中のPSPの情報を取得
 --------------------------------------------------------*/

static int GetPspEntry(char *mac, char *name)
  {
    if (adhoc_max == 0)
      return -1;

    memcpy(mac, psplist[ahdoc_pos].mac, 6);
    strcpy(name, psplist[ahdoc_pos].name);

    return 1;
  }

/*--------------------------------------------------------
 Matching callback
 --------------------------------------------------------*/

static void matchingCallback(int unk1, int event, char *mac2, int optLen,
    char *optData)
  {
    switch (event)
      {
        case MATCHING_JOINED:
          AddPsp(mac2, optData, optLen);
          break;

        case MATCHING_DISCONNECT:
          DelPsp(mac2);
          break;

        default:
          g_unk1 = unk1;
          g_matchEvent = event;
          g_matchOptLen = optLen;
          strncpy(g_matchOptData, optData, optLen);
          memcpy(g_mac, mac2, sizeof(char) * 6);
          matchChanged = 1;
          break;
      }
  }

/***************************************************************************
 AdHocインタフェース関数
 ***************************************************************************/

/*--------------------------------------------------------
 モジュールのロード
 --------------------------------------------------------*/

int pspSdkLoadAdhocModules(void)
  {
    int modID;

    modID = pspSdkLoadStartModule("flash0:/kd/ifhandle.prx", PSP_MEMORY_PARTITION_KERNEL);
    if (modID < 0)
      return modID;

    modID = pspSdkLoadStartModule("flash0:/kd/memab.prx", PSP_MEMORY_PARTITION_KERNEL);
    if (modID < 0)
      return modID;

    modID = pspSdkLoadStartModule("flash0:/kd/pspnet_adhoc_auth.prx", PSP_MEMORY_PARTITION_KERNEL);
    if (modID < 0)
      return modID;

    modID = pspSdkLoadStartModule("flash0:/kd/pspnet.prx", PSP_MEMORY_PARTITION_USER);
    if (modID < 0)
      return modID;
    else
      pspSdkFixupImports(modID);

    modID = pspSdkLoadStartModule("flash0:/kd/pspnet_adhoc.prx", PSP_MEMORY_PARTITION_USER);
    if (modID < 0)
      return modID;
    else
      pspSdkFixupImports(modID);

    modID = pspSdkLoadStartModule("flash0:/kd/pspnet_adhocctl.prx", PSP_MEMORY_PARTITION_USER);
    if (modID < 0)
      return modID;
    else
      pspSdkFixupImports(modID);

    modID = pspSdkLoadStartModule("flash0:/kd/pspnet_adhoc_matching.prx", PSP_MEMORY_PARTITION_USER);
    if (modID < 0)
      return modID;
    else
      pspSdkFixupImports(modID);

    sceKernelDcacheWritebackAll();
    sceKernelIcacheInvalidateAll();

    return 0;
  }

/*--------------------------------------------------------
 初期化
 --------------------------------------------------------*/

int adhocInit(const char *matchingData)
  {
    struct productStruct product;
    int error = 0, state = 0;
    char mac[20], buf[256];

//    video_set_mode(32);

    Server = 0;

    g_unk1 = 0;
    g_matchEvent = 0;
    g_matchOptLen = 0;
    matchChanged = 0;
    memset(g_mac, 0, sizeof(g_mac));
    memset(g_mymac, 0, sizeof(g_mymac));

    sprintf(product.product, PRODUCT "00%d%d%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
    product.unknown = 0;

    ClearPspList();

    if (strlen(matchingData) == 0)
    return -1;

    strcpy(g_matchingData, matchingData);

    adhoc_init_progress(10, "CONNECTING");

    if ((error = sceNetInit(0x20000, 0x20, 0x1000, 0x20, 0x1000)) == 0)
      {
        update_progress();
        if ((error = sceNetAdhocInit()) == 0)
          {
            update_progress();
            if ((error = sceNetAdhocctlInit(0x2000, 0x20, &product)) == 0)
              {
                update_progress();
                if ((error = sceNetAdhocctlConnect((int *)"")) == 0)
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

                        sceWlanGetEtherAddr((u8 *)mac);
                        update_progress();

                        if ((pdpId = sceNetAdhocPdpCreate((unsigned char *)mac, 0x309, 0x400, 0)) > 0)
                          {
                            update_progress();
                            if ((error = sceNetAdhocMatchingInit(0x20000)) == 0)
                              {
                                update_progress();
                                if ((matchingId = sceNetAdhocMatchingCreate(MATCHING_CREATE_PARAMS)) >= 0)
                                  {
                                    update_progress();
                                    if ((error = sceNetAdhocMatchingStart(MATCHING_START_PARAMS)) == 0)
                                      {
                                        update_progress();
                                        show_progress("CONNECTED");
                                        return 0;
                                      }
                                    sceNetAdhocMatchingDelete(matchingId);
                                  }
                                error = 2;
                                sceNetAdhocMatchingTerm();
                              }
                            sceNetAdhocPdpDelete(pdpId, 0);
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
        case 1: sprintf(buf, "%s (PDP ID = %08x)", "failed", pdpId); DBGOUT("%s\n",buf); break;
        case 2: sprintf(buf, "%s (Matching ID = %08x)", "failed", matchingId); DBGOUT("%s\n",buf);break;
        default: sprintf(buf, "%s (Error Code = %08x)", "failed", error); DBGOUT("%s\n",buf);break;
      }

    show_progress(buf);

//    pad_wait_clear();
//    pad_wait_press(PAD_WAIT_INFINITY);
error_msg("");
    return -1;
  }

/*--------------------------------------------------------
 切断
 --------------------------------------------------------*/

int adhocTerm(void)
  {
    adhoc_init_progress(5, "DISCONNECTING");

    sceNetAdhocctlDisconnect();
    update_progress();

    sceNetAdhocPdpDelete(pdpId, 0);
    update_progress();

    sceNetAdhocctlTerm();
    update_progress();

    sceNetAdhocTerm();
    update_progress();

    sceNetTerm();
    update_progress();

    show_progress("DISCONNECTED");

    return 0;
  }

/*--------------------------------------------------------
 切断
 --------------------------------------------------------*/

static void adhocDisconnect(void)
  {
    adhoc_init_progress(8, "DISCONNECTING");

    sceNetAdhocMatchingStop(matchingId);
    update_progress();

    sceNetAdhocMatchingDelete(matchingId);
    update_progress();

    sceNetAdhocMatchingTerm();
    update_progress();

    sceNetAdhocctlDisconnect();
    update_progress();

    sceNetAdhocPdpDelete(pdpId, 0);
    update_progress();

    sceNetAdhocctlTerm();
    update_progress();

    sceNetAdhocTerm();
    update_progress();

    sceNetTerm();
    update_progress();

    show_progress("DISCONNECTED");
  }

/*--------------------------------------------------------
 SSIDを指定して再接続
 --------------------------------------------------------*/

int adhocReconnect(char *ssid)
  {
    int error = 0, state = 1;
    char mac[20], buf[256];

    adhoc_init_progress(6, "DISCONNECTING");

    sceNetAdhocMatchingStop(matchingId);
    update_progress();

    sceNetAdhocMatchingDelete(matchingId);
    update_progress();

    sceNetAdhocMatchingTerm();
    update_progress();

    sceNetAdhocPdpDelete(pdpId, 0);
    update_progress();

    sceNetAdhocctlDisconnect();
    update_progress();

    do
      {
        if ((error = sceNetAdhocctlGetState(&state)) != 0)
          break;
        sceKernelDelayThread(1000000/60);
      } while (state == 1);

    update_progress();
    show_progress("DISCONNECTED");

    adhoc_init_progress(4, "CONNECTING");

    if ((error = sceNetAdhocctlConnect((int *)ssid)) == 0)
      {
        update_progress();
        do
          {
            if ((error = sceNetAdhocctlGetState(&state)) != 0)
              break;
            sceKernelDelayThread(1000000/60);
          } while (state != 1);

        if (!error)
          {
            update_progress();

            sceWlanGetEtherAddr((u8 *)mac);
            memcpy(g_mymac, mac, 6);
            update_progress();

            if ((pdpId = sceNetAdhocPdpCreate((u8 *)mac, 0x309, 0x800, 0)) > 0)
              {
                update_progress();
                show_progress("CONNECTED");
                if (Server)
                  {
                    // ほぼ同時に送受信が発生するとフリーズするため、
                    // サーバ側を少し遅らせてタイミングをずらす
                    sceKernelDelayThread(1*1000000);
                  }
                return 0;
              }
            error = 1;
          }
        sceNetAdhocctlDisconnect();

        if (state == 1)
          {
            do
              {
                if ((error = sceNetAdhocctlGetState(&state)) != 0)
                  break;
                sceKernelDelayThread(1000000/60);
              } while (state == 1);
          }
      }

    sceNetAdhocctlTerm();
    sceNetAdhocTerm();
    sceNetTerm();

    switch (error)
      {
        case 1:
          sprintf(buf, "%s (PDP ID = %08x)", "FAILED", pdpId);
          break;
        default:
          sprintf(buf, "%s (Error Code = %08x)", "FAILED", error);
          break;
      }

    show_progress(buf);

//    pad_wait_clear();
//    pad_wait_press(PAD_WAIT_INFINITY);

    return -1;
  }

/*--------------------------------------------------------
 接続先の選択
 --------------------------------------------------------*/

int adhocSelect(void)
  {
    int top = 0;
    int rows = 11;
    int currentState= PSP_LISTING;
    int prev_max = 0;
    int update = 1;
    char mac[16];
    char name[64];
    char temp[64];
    char ssid[10];
    char title[32];

//    sprintf(title, "AdHoc - %s", game_name);
//    msg_screen_init(WP_LOGO, ICON_SYSTEM, title);

    while (1)
      {
//        pad_update();

//        msg_set_text_color(0xffff);

        switch (currentState)
          {
            case PSP_LISTING:
              Server = 0;
              if (update)
                {
//                  msg_screen_init(WP_LOGO, ICON_SYSTEM, title);
//                  msg_printf(TEXT(SELECT_A_SERVER_TO_CONNECT_TO));
//                  msg_printf("\n");
                  DisplayPspList(top, rows);
                  update = 0;
                }
              if (/*pad_pressed(PSP_CTRL_UP)*/0)
                {
                  if (ahdoc_pos > 0)
                    ahdoc_pos--;
                  update = 1;
                }
              else if (/*pad_pressed(PSP_CTRL_DOWN)*/0)
                {
                  if (ahdoc_pos < adhoc_max - 1)
                    ahdoc_pos++;
                  update = 1;
                }
              else if (/*pad_pressed(PSP_CTRL_CIRCLE)*/0)
                {
                  if (GetPspEntry(mac, name) > 0)
                    {
                      if (strcmp(name, g_matchingData) == 0)
                        {
                          currentState = PSP_SELECTING;
                          sceNetAdhocMatchingSelectTarget(matchingId, mac, 0, 0);
                          update = 1;
                        }
                    }
                }
              else if (/*pad_pressed(PSP_CTRL_TRIANGLE)*/0)
                {
//                  msg_set_text_color(0xffffffff);
                  adhocDisconnect();
//                  pad_wait_clear();
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
//                  msg_screen_init(WP_LOGO, ICON_SYSTEM, title);
                  sceNetEtherNtostr((u8 *)mac, temp);
//                  msg_printf(TEXT(WAITING_FOR_x_TO_ACCEPT_THE_CONNECTION), temp);
//                  msg_printf(TEXT(TO_CANCEL_PRESS_CROSS));
                  update = 0;
                }
              if (/*pad_pressed(PSP_CTRL_CROSS)*/0)
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
//                  msg_screen_init(WP_LOGO, ICON_SYSTEM, title);
                  sceNetEtherNtostr((u8 *)mac, temp);
//                  msg_printf(TEXT(x_HAS_REQUESTED_A_CONNECTION), temp);
//                  msg_printf(TEXT(TO_ACCEPT_THE_CONNECTION_PRESS_CIRCLE_TO_CANCEL_PRESS_CIRCLE));
                  update = 0;
                }
              if (/*pad_pressed(PSP_CTRL_CROSS)*/0)
                {
                  sceNetAdhocMatchingCancelTarget(matchingId, mac);
                  currentState = PSP_LISTING;
                  update = 1;
                }
              else if (/*pad_pressed(PSP_CTRL_CIRCLE)*/0)
                {
                  sceNetAdhocMatchingSelectTarget(matchingId, mac, 0, 0);
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

        if (top > adhoc_max - rows)
          top = adhoc_max - rows;
        if (top < 0)
          top = 0;
        if (ahdoc_pos >= top + rows)
          top = ahdoc_pos - rows + 1;
        if (ahdoc_pos < top)
          top = ahdoc_pos;

        if (adhoc_max != prev_max)
          {
            prev_max = adhoc_max;
            update = 1;
          }

        sceDisplayWaitVblankStart();
      }

//    msg_set_text_color(0xffffffff);

    if (Server)
      sceWlanGetEtherAddr((u8 *)mac);

    sceNetEtherNtostr((u8 *)mac, temp);

    ssid[0] = temp[ 9];
    ssid[1] = temp[10];
    ssid[2] = temp[12];
    ssid[3] = temp[13];
    ssid[4] = temp[15];
    ssid[5] = temp[16];
    ssid[6] = '\0';

    if (adhocReconnect(ssid) < 0)
      return -1;

    return (Server ? 1 : 0);
  }

/*--------------------------------------------------------
 データ送信
 --------------------------------------------------------*/

int adhocSend(void *buffer, int length)
  {
    if (sceNetAdhocPdpSend(pdpId, g_mac, 0x309, buffer, length, 0, 1) < 0)
      return 0;
    return length;
  }

/*--------------------------------------------------------
 データ受信
 --------------------------------------------------------*/

int adhocRecv(void *buffer, int length)
  {
    int port = 0;
    char mac[6];

    if (sceNetAdhocPdpRecv(pdpId, mac, &port, buffer, &length, 0, 1) < 0)
      return 0;
    return length;
  }

/*--------------------------------------------------------
 データ送信を待つ
 --------------------------------------------------------*/

int adhocSendBlocking(void *buffer, int length)
  {
    if (sceNetAdhocPdpSend(pdpId, g_mac, 0x309, buffer, length, ADHOC_TIMEOUT, 0) < 0)
      return 0;
    return length;
  }

/*--------------------------------------------------------
 データ受信を待つ
 --------------------------------------------------------*/

int adhocRecvBlocking(void *buffer, int length)
  {
    int port = 0;
    char mac[6];

    if (sceNetAdhocPdpRecv(pdpId, mac, &port, buffer, &length, ADHOC_TIMEOUT, 0)
        < 0)
      return 0;
    return length;
  }

/*--------------------------------------------------------
 データ受信を待つ(タイムアウト指定)
 --------------------------------------------------------*/

int adhocRecvBlockingTimeout(void *buffer, int length, int timeout)
  {
    int port = 0;
    char mac[6];

    if (sceNetAdhocPdpRecv(pdpId, mac, &port, buffer, &length, timeout, 0) < 0)
      return 0;
    return length;
  }

/*--------------------------------------------------------
 データを送信し、ackを受信するまで待つ
 --------------------------------------------------------*/

int adhocSendRecvAck(void *buffer, int length)
  {
    int ack_data = 0;
    int tempLen = length;
    int sentCount = 0;
    u8 *buf = (u8 *)buffer;

    do
      {
        if (tempLen > ADHOC_BLOCKSIZE)
          tempLen = ADHOC_BLOCKSIZE;

        adhocSendBlocking(buf, tempLen);

        if (adhocRecvBlocking(&ack_data, sizeof(int)) == 0)
          return 0;

        if (ack_data != tempLen)
          return 0;

        buf += ADHOC_BLOCKSIZE;
        sentCount += ADHOC_BLOCKSIZE;
        tempLen = length - sentCount;
      } while (sentCount < length);

    return length;
  }

/*--------------------------------------------------------
 データの受信を待ち、ackを送信する
 --------------------------------------------------------*/

int adhocRecvSendAck(void *buffer, int length)
  {
    int tempLen = length;
    int rcvdCount = 0;
    u8 *buf = (u8 *)buffer;

    do
      {
        if (tempLen > ADHOC_BLOCKSIZE)
          tempLen = ADHOC_BLOCKSIZE;

        if (adhocRecvBlocking(buf, tempLen) == 0)
          return 0;

        adhocSendBlocking(&tempLen, sizeof(int));

        buf += ADHOC_BLOCKSIZE;
        rcvdCount += ADHOC_BLOCKSIZE;
        tempLen = length - rcvdCount;
      } while (rcvdCount < length);

    return length;
  }
