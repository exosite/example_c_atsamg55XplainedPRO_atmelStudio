/*****************************************************************************
*
*  Copyright (C) 2017 Exosite LLC
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Exosite LLC nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef __CONF_EXOSITE_H__
#define __CONF_EXOSITE_H__

#ifndef MAIN_WLAN_SSID

/*
 * Configure the MAIN_WLAN_SSID define.
 *
 * This is the SSID to the WiFi network.
 */

#define MAIN_WLAN_SSID "<SSID>"
#endif
#ifndef MAIN_WLAN_AUTH
#define MAIN_WLAN_AUTH M2M_WIFI_SEC_WPA_PSK
#endif
#ifndef MAIN_WLAN_PSK

/*
 * Configure the MAIN_WLAN_PSK define.
 *
 * This is the password to the WiFi network.
 */

#define MAIN_WLAN_PSK "<PASSWORD>"
#endif

#ifndef EXOPAL_PRODUCT_ID

/*
 * Configure the EXOPAL_PRODUCT_ID define.
 *
 * This is the alpha-numeric string found in your
 * Murano Product Web UI or using the MuranoCLI
 * command line tool.
 */

#define EXOPAL_PRODUCT_ID "<PRODUCT_ID>"
#endif
#ifndef EXOPAL_VENDOR_TOKEN
#define EXOPAL_VENDOR_TOKEN    EXOPAL_PRODUCT_ID
#endif
#ifndef EXOPAL_MODEL_TOKEN
#define EXOPAL_MODEL_TOKEN     EXOPAL_VENDOR_TOKEN
#endif

#ifndef EXOPAL_HOSTNAME

/*
 * The EXOPAL_HOSTNAME define is configured according to
 * the EXOPAL_PRODUCT_ID configured, above. The hostname
 * is tailored to clients like the WINC1500 that are using
 * the ECC508 crypto coprocessor. Clients such as these
 * can have difficulty when the server (Murano) presents
 * an RSA cert to the client. For this reason, Exosite
 * has a hostname available to all Products that will
 * force Murano present an ECDSA cert. This hostname is
 * PRODUCT_ID.m2-ecdsa.exosite.io.
 */

#define EXOPAL_HOSTNAME     EXOPAL_PRODUCT_ID ".m2-ecdsa.exosite.io"
#endif

#endif /*__CONF_EXOSITE_H__*/
/* vim: set ai cin et sw=4 ts=4 : */
