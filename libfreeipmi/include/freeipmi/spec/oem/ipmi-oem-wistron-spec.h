/*
 * Copyright (C) 2003-2015 FreeIPMI Core Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef IPMI_OEM_WISTRON_SPEC_H
#define IPMI_OEM_WISTRON_SPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Wistron / Dell Poweredge C6220
 */

/* achu: all named from doc except 'lan' configuration id, which I assumed names */

/* w/ IPMI_CMD_OEM_WISTRON_GET_EXTENDED_CONFIGURATION / IPMI_OEM_WISTRON_SET_EXTENDED_CONFIGURATION */

#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_NIC                      0x02
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_SOL                      0x03
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_SECURITY                 0x04
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_ACCOUNT_STATUS           0x05
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_DNS                      0x06
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_WEB_SERVER_CONFIGURATION 0x0C
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_FIRMWARE_LOG             0x0E
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_FIRMWARE_INFORMATION     0x0F
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_FIRMWARE_UPDATE          0x10
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_POWER_MANAGEMENT         0x11
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_IPV6_SETTING             0xC0
#define IPMI_OEM_WISTRON_EXTENDED_CONFIGURATION_ID_IPV6_TRAP_SETTING        0xC1

/* nic selection - 1 byte, 0 = shared, 1 = dedicated
 *
 * shared nic selection - 1 byte, 0 = reerved, 1 = nic1, ..., 4 = nic4
 * - for when nic selection is shared
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SELECTION            0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SHARED_NIC_SELECTION 0x02

/* sol idle timeout - 2 bytes, ls byte first, 0h = no timeout, default = 01h
 *
 * telnet redirect function selection - 1 byte, 0 = disable, 1 = sol enabled , 2 = smash enabled
 *
 * ssh redirect function selection - 1 byte, 0 = disable, 1 = sol enabled , 2 = smash enabled
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_SOL_IDLE_TIMEOUT                   0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_TELNET_REDIRECT_FUNCTION_SELECTION 0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_SSH_REDIRECT_FUNCTION_SELECTION    0x03

/* service disabled - 1 byte, bitmask
 *                  - 0x01 = all service except IPMI are disabled
 *                           (takes precedence over other bits)
 *                  - 0x02 = KVM/Virtual Storage
 *                  - 0x04 = HTTP/HTTPS
 *                  - 0x08 = SSH
 *                  - 0x10 = SNMP v2c agent
 *                  - 0x20 = telnet
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SECURITY_SERVICE_DISABLED            0x01

/* number of user - 1 byte, read only
 *
 * number of enabled user - 1 byte, read only
 *
 * user name - 1-17 bytes, read only
 *           - Wistron 5441/Xanadu II - reports stored as p-string, does not return p-string
 *           - Wistron 5442/Xanadu III - returns as p-string
 *
 * account status - 1 byte
 *                - 0x00 - status unspecified
 *                - 0x01 - enabled via set user password
 *                - 0x02 - disabled via set user password
 *                - 0x03 - user id is lockout
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_NUMBER_OF_USER         0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_NUMBER_OF_ENABLED_USER 0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_USER_NAME              0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_ACCOUNT_STATUS         0x04

/* DNS DHCP enable - 1 byte, 0 - false, 1 - true
 * - DNS server IP addresses should be assigned from DHCP server
 *
 * DNS Server1 - 4 bytes
 * - IP address for DNS server 1, read only if DNS DHCP Enable and DHCP are enabled
 *
 * DNS Server2 - 4 bytes
 * - IP address for DNS server 2, read only if DNS DHCP Enable and DHCP are enabled
 *
 * DNS Register BMC - 0 - false, 1 - true
 * - Enable registering the BMC host name on the DNS server
 *
 * DNS BMC Host Name - 1-64 bytes
 * - Specifies the DNS BMC host name, read only if DNS Register BMC is TRUE.
 * - Stored as P-string
 *
 * DNS Domain Name DHCP Enable - 1 byte
 * - DNS domain name should be assigned from DHCP
 *
 * DNS Domain Name - 1-256 bytes
 * - DNS domain name string, read only if DNS Domain Name DHCP Enable is TRUE
 * - Stored as P-string
 *
 * DNS Registration Delay - 1 byte (range 1-255)
 * - Delay BMC should take after the IPMI process is up and running.
   - Default 5 seconds.
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_DHCP_ENABLE             0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_SERVER1                 0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_SERVER2                 0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_REGISTER_BMC            0x04
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_BMC_HOST_NAME           0x05
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_DOMAIN_NAME_DHCP_ENABLE 0x06
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_DOMAIN_NAME             0x07
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_REGISTRATION_DELAY      0x08

/* web server enabled - 1 byte, 0 = false, 1 = true (default)
 *
 * max web sessions - 1 byte, read only
 *
 * active web sessions - 1 byte, read only
 *
 * web server timeout - 4 bytes, in seconds, 0 = disable, range 60-1920, default 300
 *
 * http port num - 2 bytes, default 80
 *
 * https port num - 2 bytes, default 443
 *
 * kvm port num - 2 bytes, default 7578
 *
 * telnet port num - 2 bytes, default 23
 *
 * ssh port num - 2 bytes, default 22
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_WEB_SERVER_ENABLED  0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_MAX_WEB_SESSIONS    0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_ACTIVE_WEB_SESSIONS 0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_WEB_SERVER_TIMEOUT  0x04
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_HTTP_PORT_NUM       0x05
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_HTTPS_PORT_NUM      0x06
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_KVM_PORT_NUM        0x07
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_TELNET_PORT_NUM     0x08
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_WEB_SERVER_CONFIGURATION_SSH_PORT_NUM        0x09

/* entity - 1 byte (read only)
 *
 * firmware version - 1-16 bytes (read only)
 *
 * branch - 1-16 bytes (read only)
 *
 * build information - 1-16 bytes (read only)
 *
 * update date / time - 3 bytes, from 0:00 1/1/08, lsbyte first
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_ENTITY            0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_FIRMWARE_VERSION  0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_BRANCH            0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_BUILD_INFORMATION 0x04
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_LOG_UPDATE_DATE_TIME  0x05

/* name - 1-16 bytes (read only)
 *
 * description - 1-256 bytes (read only)
 *
 * entity - 1 byte (read only)
 *
 * product info - 1-64 bytes (read only)
 *
 * firmware version - 1-16 bytes (read only)
 *
 * branch - 1-16 bytes (read only)
 *
 * build information - 1-16 bytes (read only)
 *
 * user default setting - 1 byte (write only)
 *
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_NAME                 0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_DESCRIPTION          0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_ENTITY               0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_PRODUCT_INFO         0x04
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_FIRMWARE_VERSION     0x05
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BRANCH               0x06
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BUILD_INFORMATION    0x07
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_USER_DEFAULT_SETTING 0x08

#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_NAME_LEN              16
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_DESCRIPTION_LEN       256
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_PRODUCT_INFO_LEN      64
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_FIRMWARE_VERSION_LEN  16
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BRANCH_LEN            16
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_BUILD_INFORMATION_LEN 16

/* remote update enable - 1 byte, ??
 *
 * protocol - 1 byte, bitmask, 7:3 reserved, 2 : http, 1: ftp, 0: tftp (read only)
 *
 * uri - 1-256 bytes
 * note - first char should be a file or unit separator
 *
 * connection retry - 1 byte, 0 = no retries
 *
 * retry interval - 1 byte, in 5 second increments, defalut 01h
 *
 * delay time - 1 byte, in seconds, 0h = immediate, ffh = random between 5 and 10
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_REMOTE_UPDATE_ENABLE 0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_PROTOCOL             0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_URI                  0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_CONNECTION_RETRY     0x04
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_RETRY_INTERVAL       0x05
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_DELAY_TIME           0x06

#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_RETRY_INTERVAL_INCREMENTS 5

#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_URI_LEN 256

/* power staggery ac recovery - 1 byte, 0x00 = immediate power on
 * (default), 0x01 = auto, random between min and max below, 0x02 =
 * user defined, must be between min and max below
 *
 * power on delay - 2 bytes
 *
 * minimum power on delay - 2 bytes (read only)
 *
 * maximum power on delay - 2 bytes
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_POWER_STAGGERING_AC_RECOVERY 0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_POWER_ON_DELAY               0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_MINIMUM_POWER_ON_DELAY       0x04
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_MAXIMUM_POWER_ON_DELAY       0x05

/* ipv6 enable - 1 byte, 0 = false, 1 = true
 *
 * ipv6 address - 1-40 bytes (read only if autoconfig is true)
 *
 * ipv6 gateway ip - 1-40 bytes
 *
 * ipv6 prefix length - 1 byte
 *
 * ipv6 autoconfig - 1 byte, 0 = false, 1 = true
 *
 * ipv6 link local address - 1-40 bytes (read only)
 *
 * ipv6 auto dns - 1 byte, 0 = false, 1 = true (can't set to 1 if autoconfig is false)
 *
 * ipv6 dns server 1 - 1-40 bytes (can't set if auto dns is true)
 *
 * ipv6 dns server 2 - 1-40 bytes (can't set if auto dns is true)
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_ENABLE             0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_ADDRESS            0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_GATEWAY_IP_ADDRESS 0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_PREFIX_LENGTH      0x04
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTOCONFIG         0x05
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_LINK_LOCAL_ADDRESS 0x06
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTO_DNS           0x07
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_DNS_SERVER1        0x08
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_DNS_SERVER2        0x09

#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_ADDRESS_LEN            40
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_GATEWAY_IP_ADDRESS_LEN 40
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_LINK_LOCAL_ADDRESS_LEN 40
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_DNS_SERVER1_LEN        40
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_DNS_SERVER2_LEN        40

/* ipv6 snmp trap destination setting - 3 bytes
 * byte 1 - destination type
 * byte 2 - alert ack timeout
 * byte 3 - retries
 *
 * ipv6 snmp trap destination address - 1-40 bytes
 *
 * number of ipv6 snmp trap desitnations - 1 byte (read only)
 */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SNMP_TRAP_DESTINATION_SETTING   0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SNMP_TRAP_DESTINATION_ADDRESS   0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NUMBER_OF_IPV6_SNMP_TRAP_DESTINATION 0x03

#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SNMP_TRAP_DESTINATION_ADDRESS_LEN 40

#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_READ_ALL_BYTES     0xFF

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SELECTION */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_NIC_MODE_SHARED    0x00
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_NIC_MODE_DEDICATED 0x01

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SHARED_NIC_SELECTION */
/* 0h - recommended when nic selection is dedicated */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SHARED_NIC_SELECTION_RESERVED 0x00
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SHARED_NIC_SELECTION_NIC_1    0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SHARED_NIC_SELECTION_NIC_2    0x02
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SHARED_NIC_SELECTION_NIC_3    0x03
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_NIC_SHARED_NIC_SELECTION_NIC_4    0x04

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_TELNET_REDIRECT_FUNCTION_SELECTION */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_TELNET_REDIRECT_FUNCTION_SELECTION_DISABLE       0x00
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_TELNET_REDIRECT_FUNCTION_SELECTION_SOL_ENABLED   0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_TELNET_REDIRECT_FUNCTION_SELECTION_SMASH_ENABLED 0x02

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_SSH_REDIRECT_FUNCTION_SELECTION */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_SSH_REDIRECT_FUNCTION_SELECTION_DISABLE       0x00
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_SSH_REDIRECT_FUNCTION_SELECTION_SOL_ENABLED   0x01
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SOL_SSH_REDIRECT_FUNCTION_SELECTION_SMASH_ENABLED 0x02

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_SECURITY_SERVICE_DISABLED */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_ENABLE_ALL                     0x00
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_ALL                    0x01
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_KVM_VIRTUAL_STORAGE    0x02
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_HTTP_HTTPS             0x04
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_SSH                    0x08
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_SNMP_V2C_AGENT         0x10
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_SECURITY_SERVICES_DISABLED_BITMASK_TELNET                 0x20

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_ACCOUNT_STATUS_ACCOUNT_STATUS */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_ACCOUNT_STATUS_UNSPECIFIED 0x00
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_ACCOUNT_STATUS_ENABLED     0x01
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_ACCOUNT_STATUS_DISABLED    0x02
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_ACCOUNT_STATUS_LOCKOUT     0x03

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_BMC_HOST_NAME */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_DNS_DNS_BMC_HOST_NAME_MAX 64

/* IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_DNS_DNS_DOMAIN_NAME */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_DNS_DNS_DOMAIN_NAME_MAX   256

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_ENTITY */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_INFORMATION_ENTITY_BMC         0
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_INFORMATION_ENTITY_SYSTEM_BIOS 1
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_INFORMATION_ENTITY_PDB         2
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_INFORMATION_ENTITY_FCB         3

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_USER_DEFAULT_SETTING */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_USER_DEFAULT_SETTING_SET_DEFAULT     0x0
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_INFORMATION_USER_DEFAULT_SETTING_RESTORE_DEFAULT 0x1

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_REMOTE_UPDATE_ENABLE */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_REMOTE_UPDATE_DISABLE 0x00
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_REMOTE_UPDATE_ENABLE  0x01

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_PROTOCOL */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_UPDATE_PROTOCOL_BITMASK_TFTP  0x01
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_UPDATE_PROTOCOL_BITMASK_FTP   0x02
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_UPDATE_PROTOCOL_BITMASK_HTTP  0x04

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_FIRMWARE_UPDATE_DELAY_TIME */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_FIRMWARE_UPDATE_DELAY_TIME_RANDOM 0xFF

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_POWER_MANAGEMENT_POWER_STAGGERING_AC_RECOVERY */
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_POWER_STAGGERING_AC_RECOVERY_IMMEDIATE    0x00
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_POWER_STAGGERING_AC_RECOVERY_AUTO         0x01
#define IPMI_OEM_WISTRON_EXTENDED_CONFIG_POWER_STAGGERING_AC_RECOVERY_USER_DEFINED 0x02

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_ENABLE */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_ENABLE_FALSE 0
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_ENABLE_TRUE  1

/* w/ #define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTOCONFIG */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTOCONFIG_ENABLE_FALSE 0
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTOCONFIG_ENABLE_TRUE  1

/* w/ #define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTO_DNS */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTO_DNS_ENABLE_FALSE 0
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SETTING_IPV6_AUTO_DNS_ENABLE_TRUE  1

/* w/ IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SNMP_TRAP_DESTINATION_SETTING */
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SNMP_TRAP_DESTINATION_SETTING_DESTINATION_TYPE_PET IPMI_DESTINATION_TYPE_PET_TRAP_DESTINATION
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SNMP_TRAP_DESTINATION_SETTING_DESTINATION_TYPE_OEM1 IPMI_DESTINATION_TYPE_OEM1
#define IPMI_OEM_WISTRON_EXTENDED_ATTRIBUTE_ID_IPV6_SNMP_TRAP_DESTINATION_SETTING_DESTINATION_TYPE_OEM2 IPMI_DESTINATION_TYPE_OEM2

/* w/ IPMI_CMD_OEM_WISTRON_RESET_TO_DEFAULTS */
#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_RESTORE_FLAG_RESTORE_PARAMETERS_NOT_INCLUDED_BELOW 0x7
#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_RESTORE_FLAG_REMAINING_PARAMETERS_STAY_WHAT_IT_IS  0x0
#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_RESTORE_FLAG_SHIFT                                 5

#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_PEF_BITMASK                  0x10
#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_SERIAL_CONFIGURATION_BITMASK 0x08
#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_SOL_CONFIGURATION_BITMASK    0x04
#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_LAN_CONFIGURATION_BITMASK    0x02
#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_USER_ACCOUNTS_BITMASK        0x01

#define IPMI_OEM_WISTRON_RESET_TO_DEFAULTS_RESTORE_ALL_SC_SETTINGS      0xFF

/* w/ IPMI_CMD_OEM_WISTRON_GET_RESTORE_STATUS */
#define IPMI_OEM_WISTRON_GET_RESTORE_STATUS_RESTORE_IN_PROGRESS 0x00
#define IPMI_OEM_WISTRON_GET_RESTORE_STATUS_RESTORE_COMPLETE    0x01

/* w/ IPMI_CMD_OEM_WISTRON_SET_CHASSIS_SERVICE_TAG and IPMI_CMD_OEM_WISTRON_GET_CHASSIS_SERVICE_TAG */
#define IPMI_OEM_WISTRON_SC_BMC_COMMUNICATION_PROTOCOL_VERSION 0x01

#define IPMI_OEM_WISTRON_CHASSIS_IDENTIFICATION_LED_CONTROLLED_BY_SC_BITMASK 0x01
#define IPMI_OEM_WISTRON_CHASSIS_IDENTIFICATION_LED_CONTROLLED_BY_SC_SHIFT   0

#define IPMI_OEM_WISTRON_CHASSIS_FAULT_LED_CONTROLLED_BY_SC_BITMASK 0x02
#define IPMI_OEM_WISTRON_CHASSIS_FAULT_LED_CONTROLLED_BY_SC_SHIFT   1

#define IPMI_OEM_WISTRON_CHASSIS_LED_OFF      0x00
#define IPMI_OEM_WISTRON_CHASSIS_LED_SOLID_ON 0x01
#define IPMI_OEM_WISTRON_CHASSIS_LED_BLINK_ON 0x02

/* w/ IPMI_CMD_OEM_WISTRON_GET_CHASSIS_POWER_READINGS */
#define IPMI_OEM_WISTRON_CHASSIS_TOTAL_POWER_CONSUMPTION_AVAILABLE_BITMASK 0x01
#define IPMI_OEM_WISTRON_CHASSIS_TOTAL_POWER_CONSUMPTION_AVAILABLE_SHIFT   0

#define IPMI_OEM_WISTRON_CHASSIS_COOLING_POWER_CONSUMPTION_AVAILABLE_BITMASK 0x02
#define IPMI_OEM_WISTRON_CHASSIS_COOLING_POWER_CONSUMPTION_AVAILABLE_SHIFT   1

/* w/ IPMI_CMD_OEM_WISTRON_SET_PASSWORD_POLICY
 */
#define IPMI_OEM_WISTRON_PASSWORD_POLICY_DISABLE 0x00
#define IPMI_OEM_WISTRON_PASSWORD_POLICY_ENABLE  0x01

/* w/ IPMI_CMD_OEM_WISTRON_WRITE_PROPRIETARY_STRING
 * and  IPMI_CMD_OEM_WISTRON_READ_PROPRIETARY_STRING
 */
#define IPMI_OEM_WISTRON_PROPRIETARY_STRING_MAX   80
#define IPMI_OEM_WISTRON_PROPRIETARY_STRING_BLOCK 16

#ifdef __cplusplus
}
#endif

#endif /* IPMI_OEM_WISTRON_SPEC_H */
