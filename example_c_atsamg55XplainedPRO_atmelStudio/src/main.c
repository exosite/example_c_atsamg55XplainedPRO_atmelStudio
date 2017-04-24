/**
 * \file
 *
 * \brief
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <string.h>
#include "asf.h"
#include "bsp/include/nm_bsp.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
#include "conf_uart_serial.h"
#include "conf_exosite.h"
#include "exosite_async.h"

/** Exosite Library state. */
Exosite_state_t exoLib;
/** Plaform specific state for Exosite library. */
exoPal_state_t exoPal;

/** Wi-Fi status variable. */
static bool gbConnectedWifi = false;
static char writeReqBuffer[40];

/* Local prototypes */
int exolib_start_complete(Exosite_state_t *exo, int status);
int exolib_write_complete(Exosite_state_t *exo, int status);
int exolib_read_begin(Exosite_state_t *exo, int status);
int exolib_read_raw(Exosite_state_t *exo, const char *data, size_t len);
int exolib_read_complete(Exosite_state_t *exo, int status);
int exolib_timestamp_complete(Exosite_state_t *exo, uint32_t timestamp);

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
    const usart_serial_options_t uart_serial_options = {
        .baudrate =     CONF_UART_BAUDRATE,
        .charlength =   CONF_UART_CHAR_LENGTH,
        .paritytype =   CONF_UART_PARITY,
        .stopbits =     CONF_UART_STOP_BITS,
    };

    /* Configure UART console. */
    sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
    stdio_serial_init(CONF_UART, &uart_serial_options);
}

int exolib_start_complete(Exosite_state_t *exo, int status)
{
    if(status != 200) {
        printf("!! Exolib FAILED!. (%d)\r\n", status);
        return -1;
    }
    printf(":: Exolib ready. Getting timestamp from server.\r\n");

    // Got get the timestamp from the server.  Then write that to an alias.
    exosite_timestamp(exo);
    return 0;
}

int exolib_write_complete(Exosite_state_t *exo, int status)
{
    printf(":: Write returned with %d\r\n", status);
    return 0;
}

int exolib_read_begin(Exosite_state_t *exo, int status)
{
    return 0;
}

int exolib_read_raw(Exosite_state_t *exo, const char *data, size_t len)
{
    return 0;
}

int exolib_read_complete(Exosite_state_t *exo, int status)
{
    return 0;
}

int exolib_timestamp_complete(Exosite_state_t *exo, uint32_t timestamp)
{
    printf(":: Got timestamp %d.\r\n", timestamp);
    // !!! Remember, the buffer needs to exist until the write callback is called.
    // So you cannot use memory on the stack.
    snprintf(writeReqBuffer, sizeof(writeReqBuffer), "change=%d", timestamp);
    exosite_write(exo, writeReqBuffer);
    return 0;
}

/**
 * \brief Callback function of IP address.
 *
 * \param[in] hostName Domain name.
 * \param[in] hostIp Server IP.
 *
 * \return None.
 */
static void resolve_cb(uint8_t *hostName, unsigned long hostIp)
{
    exoPal.hostIP = hostIp;
    printf("resolve_cb: %s IP address is %d.%d.%d.%d\r\n\r\n", hostName,
            (int)((hostIp >> 0) & 0xff), (int)((hostIp >> 8) & 0xff),
            (int)((hostIp >> 16) & 0xff), (int)((hostIp >> 24) & 0xff));

    if(exoPal.ops.on_start_complete) {
        exoPal.ops.on_start_complete(&exoPal, 0);
    }
}

/**
 * \brief Callback function of TCP client socket.
 *
 * \param[in] sock socket handler.
 * \param[in] u8Msg Type of Socket notification
 * \param[in] pvMsg A structure contains notification informations.
 *
 * \return None.
 */
static void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg)
{
    /* Check for socket event on TCP socket. */
    if (sock == exoPal.tcp_socket) {
        switch (u8Msg) {
            case SOCKET_MSG_CONNECT:
                {
                    tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
                    if (pstrConnect && pstrConnect->s8Error >= SOCK_ERR_NO_ERROR) {
                        if(exoPal.ops.on_connected) {
                            exoPal.ops.on_connected(&exoPal, 0);
                        }

                    } else {
                        printf("socket_cb: connect error! (%d)\r\n", pstrConnect->s8Error);
                        close(exoPal.tcp_socket);
                        exoPal.tcp_socket = -1;
                        if(exoPal.ops.on_connected) {
                            exoPal.ops.on_connected(&exoPal, pstrConnect->s8Error);
                        }
                    }
                }
                break;

            case SOCKET_MSG_SEND:
                //tstrSocketSendMsg *snd = (tstrSocketSendMsg *)pvMsg;
                if(exoPal.ops.on_send_complete) {
                    // FIXME: where is the result code hiding?
                    exoPal.ops.on_send_complete(&exoPal, 0);
                }
                break;

            case SOCKET_MSG_RECV:
                {
                    tstrSocketRecvMsg *rcv = (tstrSocketRecvMsg *)pvMsg;
                    size_t len=0;
                    if (rcv && rcv->s16BufferSize > 0) {
                        len = rcv->s16BufferSize;
                    } else if(rcv && rcv->s16BufferSize > SOCK_ERR_CONN_ABORTED) {
                        /* Other end closed, exosite lib expects a 0 here */
                        len = 0;
                    } else {
                        printf("socket_cb: recv error! (%d)\r\n", rcv->s16BufferSize);
                        close(exoPal.tcp_socket);
                        exoPal.tcp_socket = -1;
                    }
                    if(exoPal.ops.on_recv) {
                        exoPal.ops.on_recv(&exoPal, (const char*)rcv->pu8Buffer, len);
                    }
                }
                break;

            default:
                break;
        }
    }
}

/**
 * \brief Callback to get the Wi-Fi status update.
 *
 * \param[in] u8MsgType Type of Wi-Fi notification.
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters.
 *
 * \return None.
 */
static void wifi_cb(uint8_t u8MsgType, void *pvMsg)
{
    switch (u8MsgType) {
    case M2M_WIFI_RESP_CON_STATE_CHANGED:
    {
        tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
        if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
            printf("wifi_cb: M2M_WIFI_CONNECTED\r\n");
            m2m_wifi_request_dhcp_client();
        } else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
            printf("wifi_cb: M2M_WIFI_DISCONNECTED\r\n");
            gbConnectedWifi = false;
        }

        break;
    }

    case M2M_WIFI_REQ_DHCP_CONF:
    {
        uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
        printf("wifi_cb: IP address is %u.%u.%u.%u\r\n",
                pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
        gbConnectedWifi = true;

        /* Start up the exosite library */
        exosite_start(&exoLib);
        break;
    }

    case M2M_WIFI_RESP_PROVISION_INFO:
    {
        tstrM2MProvisionInfo *pstrProvInfo = (tstrM2MProvisionInfo *)pvMsg;
        printf("wifi_cb: M2M_WIFI_RESP_PROVISION_INFO\r\n");

        if (pstrProvInfo->u8Status == M2M_SUCCESS) {
            m2m_wifi_connect((char *)pstrProvInfo->au8SSID, strlen((char *)pstrProvInfo->au8SSID), pstrProvInfo->u8SecType,
                    pstrProvInfo->au8Password, M2M_WIFI_CH_ALL);
        } else {
            printf("wifi_cb: provision failed!\r\n");
        }
    }
    break;

    default:
        break;
    }
}

/**
 * \brief Main application function.
 *
 * Initialize system, UART console, network then start weather client.
 *
 * \return Program return value.
 */
int main(void)
{
    tstrWifiInitParam param;
    int8_t ret;

    /* Initialize the board. */
    sysclk_init();
    board_init();

    /* Initialize the UART console. */
    configure_console();
    printf("-- WINC1500 Exosite client example --\r\n"
        "-- "BOARD_NAME " --\r\n"
        "-- Compiled: "__DATE__ " "__TIME__ " --\r\n");

    /* Initialize the BSP. */
    nm_bsp_init();

    /* Initialize Wi-Fi parameters structure. */
    memset((uint8_t *)&param, 0, sizeof(tstrWifiInitParam));

    /* Initialize Wi-Fi driver with data and status callbacks. */
    param.pfAppWifiCb = wifi_cb;
    ret = m2m_wifi_init(&param);
    if (M2M_SUCCESS != ret) {
        printf("main: m2m_wifi_init call error!(%d)\r\n", ret);
        while (1) {
        }
    }

    /* Initialize socket API. */
    socketInit();
    registerSocketCallback(socket_cb, resolve_cb);

    /* Initialize Exosite library */
    exosite_init(&exoLib);
    exoLib.exoPal = &exoPal;
    exoLib.ops.on_start_complete = exolib_start_complete;
    exoLib.ops.on_write_complete = exolib_write_complete;
    exoLib.ops.on_read_begin = exolib_read_begin;
    exoLib.ops.on_read_raw = exolib_read_raw;
    exoLib.ops.on_read_complete = exolib_read_complete;
    exoLib.ops.on_timestamp_complete = exolib_timestamp_complete;

    {
        // Print out the MAC early.  This makes it easy to get for enabling.
        uint8_t mac_addr[6];
        m2m_wifi_get_mac_address(mac_addr);
        printf("Device MAC: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3],
                mac_addr[4], mac_addr[5]);
    }

    // Hardcoded SSID & pass.
    m2m_wifi_connect((char*)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID),
            MAIN_WLAN_AUTH, (char*)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);

    while (1) {
        m2m_wifi_handle_events(NULL);
    }

    return 0;
}

/* vim: set ai cin et sw=4 ts=4 : */
