/* 
   ipmi-udm.h: IPMI Unified Driver Model (API interface for all IPMI drivers)

   Copyright (C) 2005 FreeIPMI Core Team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  

*/

#ifndef _IPMI_UDM_H
#define _IPMI_UDM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/socket.h>
#include <freeipmi/ipmi-error.h>
#include <freeipmi/ipmi-kcs-api.h>
#include <freeipmi/ipmi-locate.h>
#include <freeipmi/ipmi-messaging-support-cmds.h>
#include <freeipmi/ipmi-ssif-api.h>

enum ipmi_errnum
  {
    IPMI_ERR_SUCCESS = 0,
    IPMI_ERR_NULL_DEVICE = 1,
    IPMI_ERR_DEVICE_MAGIC = 2,
    IPMI_ERR_PERMISSION = 3,
    IPMI_ERR_USERNAME = 4,
    IPMI_ERR_PASSWORD = 5,
    IPMI_ERR_K_G = 6,
    IPMI_ERR_PRIVILEGE = 7,
    IPMI_ERR_AUTHENTICATION_TYPE = 8,
    IPMI_ERR_CIPHER_SUITE_UNAVAILABLE = 9,
    IPMI_ERR_PASSWORD_VERIFICATION_TIMEOUT = 10,
    IPMI_ERR_IPMI_2_0_UNAVAILABLE = 11,
    IPMI_ERR_SESSION_TIMEOUT = 12,
    IPMI_ERR_DEVICE_ALREADY_OPEN = 13,
    IPMI_ERR_DEVICE_NOT_OPEN = 14,
    IPMI_ERR_DEVICE_NOT_SUPPORTED = 15,
    IPMI_ERR_DEVICE_NOT_FOUND = 16,
    IPMI_ERR_BAD_COMPLETION_CODE_NODE_BUSY = 17,
    IPMI_ERR_BAD_COMPLETION_CODE_INVALID_COMMAND = 18,
    IPMI_ERR_BAD_COMPLETION_CODE_REQUEST_DATA_INVALID = 19,
    IPMI_ERR_BAD_COMPLETION_CODE_INSUFFICIENT_PRIVILEGE = 20,
    IPMI_ERR_BAD_COMPLETION_CODE = 21,
    IPMI_ERR_BAD_RMCPPLUS_STATUS_CODE = 22,
    IPMI_ERR_BMC_BUSY = 23,
    IPMI_ERR_OUT_OF_MEMORY = 24,
    IPMI_ERR_INVALID_HOSTNAME = 25,
    IPMI_ERR_INVALID_PARAMETERS = 26,
    IPMI_ERR_DRIVER_PATH_REQUIRED = 27,
    IPMI_ERR_INTERNAL_IPMI_ERROR = 28,
    IPMI_ERR_INTERNAL_SYSTEM_ERROR = 29,
    IPMI_ERR_INTERNAL_LIBRARY_ERROR = 30,
    IPMI_ERR_INTERNAL_ERROR = 31,
    IPMI_ERR_OUTOFRANGE = 32,
  };
typedef enum ipmi_errnum ipmi_errnum_type_t;

enum ipmi_driver_type
  {
    IPMI_DEVICE_UNKNOWN = 0,
    IPMI_DEVICE_LAN = 1,
    IPMI_DEVICE_LAN_2_0 = 2,
    IPMI_DEVICE_KCS = 3,
    IPMI_DEVICE_SMIC = 4,
    IPMI_DEVICE_BT = 5,
    IPMI_DEVICE_SSIF = 6,
    IPMI_DEVICE_OPENIPMI = 7,
  };
typedef enum ipmi_driver_type ipmi_driver_type_t;

#define IPMI_FLAGS_DEFAULT        0x00000000
#define IPMI_FLAGS_NONBLOCKING    0x00000001
#define IPMI_FLAGS_DEBUG_DUMP     0x00000010

#define IPMI_OUTOFBAND_WORKAROUND_FLAGS_DEFAULT                     0x00000000
#define IPMI_OUTOFBAND_WORKAROUND_FLAGS_ACCEPT_SESSION_ID_ZERO      0x00000001
#define IPMI_OUTOFBAND_WORKAROUND_FLAGS_FORCE_PERMSG_AUTHENTICATION 0x00000002
#define IPMI_OUTOFBAND_WORKAROUND_FLAGS_CHECK_UNEXPECTED_AUTHCODE   0x00000004
#define IPMI_OUTOFBAND_WORKAROUND_FLAGS_BIG_ENDIAN_SEQUENCE_NUMBER  0x00000008

#define IPMI_OUTOFBAND_2_0_WORKAROUND_FLAGS_DEFAULT                 0x00000000

#define IPMI_INBAND_WORKAROUND_FLAGS_DEFAULT                        0x00000000

typedef struct ipmi_device *ipmi_device_t;
 
ipmi_device_t ipmi_device_create(void);

char *ipmi_device_strerror(int errnum);

int ipmi_device_errnum(ipmi_device_t dev);

int ipmi_open_outofband (ipmi_device_t dev,
			 ipmi_driver_type_t driver_type, 
			 const char *hostname,
			 const char *username, 
			 const char *password, 
			 uint8_t authentication_type, 
			 uint8_t privilege_level,
			 unsigned int session_timeout,
			 unsigned int retry_timeout, 
                         uint32_t workaround_flags,
			 uint32_t flags);

int ipmi_open_outofband_2_0 (ipmi_device_t dev,
                             ipmi_driver_type_t driver_type, 
                             const char *hostname,
                             const char *username, 
                             const char *password, 
                             const char *k_g,
                             unsigned int k_g_len,
                             uint8_t privilege_level,
                             uint8_t cipher_suite_id,
                             unsigned int session_timeout,
                             unsigned int retry_timeout, 
                             uint32_t workaround_flags,
                             uint32_t flags);

int ipmi_open_inband (ipmi_device_t dev,
		      ipmi_driver_type_t driver_type, 
		      int disable_auto_probe, 
		      uint16_t driver_address, 
		      uint8_t reg_space,
		      char *driver_device, 
                      uint32_t workaround_flags,
		      uint32_t flags);

int ipmi_cmd (ipmi_device_t dev, 
	      uint8_t lun, 
	      uint8_t net_fn, 
	      fiid_obj_t obj_cmd_rq, 
	      fiid_obj_t obj_cmd_rs);

int ipmi_cmd_raw (ipmi_device_t dev, 
                  uint8_t lun, 
                  uint8_t net_fn, 
		  uint8_t *in, 
		  size_t in_len, 
		  uint8_t *out, 
		  size_t out_len);

int ipmi_close_device (ipmi_device_t dev);

void ipmi_device_destroy (ipmi_device_t dev);

#ifdef __cplusplus
}
#endif

#endif /* _IPMI_UDM_H */
