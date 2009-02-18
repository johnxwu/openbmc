/* 
   Copyright (C) 2003-2009 FreeIPMI Core Team

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
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.  

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#ifdef STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_ALLOCA_H
#include <alloca.h>
#endif /* HAVE_ALLOCA_H */
#include <assert.h>
#include <errno.h>

#include "freeipmi/util/ipmi-lan-util.h"
#include "freeipmi/cmds/ipmi-messaging-support-cmds.h"
#include "freeipmi/interface/ipmi-lan-interface.h"
#include "freeipmi/interface/rmcp-interface.h"
#include "freeipmi/spec/ipmi-authentication-type-spec.h"
#include "freeipmi/spec/ipmi-netfn-spec.h"
#include "freeipmi/util/ipmi-util.h"

#include "libcommon/ipmi-fiid-util.h"
#include "libcommon/ipmi-trace.h"
#include "libcommon/ipmi-fiid-wrappers.h"
#include "libcommon/ipmi-md2.h"
#include "libcommon/ipmi-md5.h"

#include "freeipmi-portability.h"
#include "secure.h"

int8_t 
ipmi_lan_check_session_sequence_number (fiid_obj_t obj_lan_session_hdr, uint32_t session_sequence_number)
{
  uint64_t session_sequence_number_recv;
  int32_t len;

  if (!fiid_obj_valid(obj_lan_session_hdr))
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_field_lookup (obj_lan_session_hdr, "session_sequence_number") < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((len = fiid_obj_field_len (obj_lan_session_hdr, "session_sequence_number")) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_session_hdr);
      return (-1);
    }
  if (!len)
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_get(obj_lan_session_hdr, "session_sequence_number", &session_sequence_number_recv) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  return ((((uint32_t)session_sequence_number_recv) == session_sequence_number) ? 1 : 0);
}

int8_t 
ipmi_lan_check_session_id (fiid_obj_t obj_lan_session_hdr, uint32_t session_id)
{
  uint64_t session_id_recv;
  int32_t len;

  if (fiid_obj_valid(obj_lan_session_hdr))
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_field_lookup (obj_lan_session_hdr, "session_id") < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((len = fiid_obj_field_len (obj_lan_session_hdr, "session_id")) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_session_hdr);
      return (-1);
    }
  if (!len)
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_get(obj_lan_session_hdr, "session_id", &session_id_recv) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  return ((((uint32_t)session_id_recv) == session_id) ? 1 : 0);
}

int8_t 
ipmi_lan_check_session_authentication_code (fiid_obj_t obj_lan_session_hdr_rs,
                                            fiid_obj_t obj_lan_msg_hdr_rs,
                                            fiid_obj_t obj_cmd,
                                            fiid_obj_t obj_lan_msg_trlr_rs,
                                            uint8_t authentication_type, 
                                            uint8_t *authentication_code_data, 
                                            uint32_t authentication_code_data_len)
{
  uint8_t authentication_code_recv[IPMI_1_5_MAX_PASSWORD_LENGTH];
  uint8_t authentication_code_calc[IPMI_1_5_MAX_PASSWORD_LENGTH];
  int32_t authentication_code_recv_len;
  uint8_t pwbuf[IPMI_1_5_MAX_PASSWORD_LENGTH];
  uint64_t val;
  int rv = -1;

  if (!fiid_obj_valid(obj_lan_session_hdr_rs)
      || !fiid_obj_valid(obj_lan_msg_hdr_rs)
      || !fiid_obj_valid(obj_cmd)
      || !fiid_obj_valid(obj_lan_msg_trlr_rs)
      || (authentication_type != IPMI_AUTHENTICATION_TYPE_NONE
          && authentication_type != IPMI_AUTHENTICATION_TYPE_MD2
          && authentication_type != IPMI_AUTHENTICATION_TYPE_MD5
          && authentication_type != IPMI_AUTHENTICATION_TYPE_STRAIGHT_PASSWORD_KEY)
      || (authentication_code_data && authentication_code_data_len > IPMI_1_5_MAX_PASSWORD_LENGTH))
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_template_compare(obj_lan_session_hdr_rs, tmpl_lan_session_hdr) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  if (Fiid_obj_template_compare(obj_lan_msg_hdr_rs, tmpl_lan_msg_hdr_rs) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  if (Fiid_obj_template_compare(obj_lan_msg_trlr_rs, tmpl_lan_msg_trlr) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if (Fiid_obj_packet_valid(obj_lan_session_hdr_rs) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  if (Fiid_obj_packet_valid(obj_lan_msg_hdr_rs) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  if (Fiid_obj_packet_valid(obj_lan_msg_trlr_rs) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if (Fiid_obj_get(obj_lan_session_hdr_rs, "authentication_type", &val) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if (authentication_type != val)
    return (0);

  memset(authentication_code_recv, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  if ((authentication_code_recv_len = fiid_obj_get_data(obj_lan_session_hdr_rs,
                                                        "authentication_code",
                                                        authentication_code_recv,
                                                        IPMI_1_5_MAX_PASSWORD_LENGTH)) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_session_hdr_rs);
      return (-1);
    }
  
  if (authentication_type == IPMI_AUTHENTICATION_TYPE_NONE)
    {
      if (authentication_code_recv_len)
	rv = 0;
      else
	rv = 1;
      goto cleanup;
    }

  memset(authentication_code_calc, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  if (authentication_type == IPMI_AUTHENTICATION_TYPE_STRAIGHT_PASSWORD_KEY)
    memcpy(authentication_code_calc, authentication_code_data, authentication_code_data_len);
  else if (authentication_type == IPMI_AUTHENTICATION_TYPE_MD2
           || authentication_type == IPMI_AUTHENTICATION_TYPE_MD5)
    {
      int32_t obj_lan_msg_hdr_len, obj_cmd_len, obj_lan_msg_trlr_len, obj_len, len;
      uint8_t session_id_buf[1024];
      uint8_t session_sequence_number_buf[1024];
      int32_t session_id_len, session_sequence_number_len;
      uint8_t *buf;
      uint32_t buflen;

      if ((session_id_len = fiid_obj_get_data (obj_lan_session_hdr_rs,
                                               "session_id",
                                               session_id_buf,
                                               1024)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_session_hdr_rs);
          goto cleanup;
        }
      
      if ((session_sequence_number_len = fiid_obj_get_data (obj_lan_session_hdr_rs,
                                                            "session_sequence_number",
                                                            session_sequence_number_buf,
                                                            1024)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_session_hdr_rs);
          goto cleanup;
        }
      
      /* Must zero extend password.  No null termination is required.
       * Also, must memcpy instead of strcpy, password need not be
       * 1 word
       */
      memset(pwbuf, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
      memcpy(pwbuf, authentication_code_data, authentication_code_data_len);

      if ((obj_lan_msg_hdr_len = fiid_obj_len_bytes (obj_lan_msg_hdr_rs)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr_rs);
          goto cleanup;
        }
      if ((obj_cmd_len = fiid_obj_len_bytes (obj_cmd)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_cmd);
          goto cleanup;
        }
      if ((obj_lan_msg_trlr_len = fiid_obj_len_bytes (obj_lan_msg_trlr_rs)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_trlr_rs);
          goto cleanup;
        }
      
      buflen = obj_lan_msg_hdr_len + obj_cmd_len + obj_lan_msg_trlr_len;
      if (!(buf = (uint8_t *)alloca(buflen)))
        {
          ERRNO_TRACE(errno);
          return (-1);
        }
      
      len = 0;
      if ((obj_len = fiid_obj_get_all(obj_lan_msg_hdr_rs, buf + len, buflen - len)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr_rs);
          goto cleanup;
        }
      len += obj_len;
      
      if ((obj_len = fiid_obj_get_all(obj_cmd, buf + len, buflen - len)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_cmd);
          goto cleanup;
        }
      len += obj_len;
      
      if ((obj_len = fiid_obj_get_all(obj_lan_msg_trlr_rs, buf + len, buflen - len)) < 0)
        {
          FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_trlr_rs);
          goto cleanup;
        }
      len += obj_len;

      if (authentication_type == IPMI_AUTHENTICATION_TYPE_MD2)
        {
          md2_t ctx;
          
          assert(IPMI_1_5_MAX_PASSWORD_LENGTH == MD2_DIGEST_LENGTH);
          
          md2_init(&ctx);
          md2_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
          md2_update_data(&ctx, session_id_buf, session_id_len);
          md2_update_data(&ctx, buf, len);
          md2_update_data(&ctx, session_sequence_number_buf, session_sequence_number_len);
          md2_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
          md2_finish(&ctx, authentication_code_calc, IPMI_1_5_MAX_PASSWORD_LENGTH);
          md2_init(&ctx);
        }
      else if (authentication_type == IPMI_AUTHENTICATION_TYPE_MD5)
        {
          md5_t ctx;

          assert(IPMI_1_5_MAX_PASSWORD_LENGTH == MD5_DIGEST_LENGTH);

          md5_init(&ctx);
          md5_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
          md5_update_data(&ctx, session_id_buf, session_id_len);
          md5_update_data(&ctx, buf, len);
          md5_update_data(&ctx, session_sequence_number_buf, session_sequence_number_len);
          md5_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
          md5_finish(&ctx, authentication_code_calc, IPMI_1_5_MAX_PASSWORD_LENGTH);
          md5_init(&ctx);
        }
    }

  /* Must memcmp instead of strcmp, password need not be 1 word */
  if (memcmp(authentication_code_recv, authentication_code_calc, IPMI_1_5_MAX_PASSWORD_LENGTH) == 0)
    rv = 1;
  else
    rv = 0;

 cleanup:
  secure_memset(authentication_code_recv, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  secure_memset(authentication_code_calc, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  secure_memset(pwbuf, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  return rv;
}

int8_t 
ipmi_lan_check_packet_session_authentication_code (uint8_t *pkt, uint64_t pkt_len, uint8_t authentication_type, uint8_t *authentication_code_data, uint32_t authentication_code_data_len)
{
  uint8_t authentication_type_recv;
  int32_t rmcp_hdr_len, authentication_type_index, authentication_code_index;
  uint32_t authentication_type_offset, authentication_code_offset;
  uint8_t authentication_code_buf[IPMI_1_5_MAX_PASSWORD_LENGTH];
  uint8_t pwbuf[IPMI_1_5_MAX_PASSWORD_LENGTH];
  int rv = -1;

  if (!pkt 
      || (authentication_type != IPMI_AUTHENTICATION_TYPE_NONE
          && authentication_type != IPMI_AUTHENTICATION_TYPE_MD2
          && authentication_type != IPMI_AUTHENTICATION_TYPE_MD5
          && authentication_type != IPMI_AUTHENTICATION_TYPE_STRAIGHT_PASSWORD_KEY)
      || (authentication_code_data && authentication_code_data_len > IPMI_1_5_MAX_PASSWORD_LENGTH))
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }
  
  if ((rmcp_hdr_len = fiid_template_len_bytes(tmpl_rmcp_hdr)) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((authentication_type_index = fiid_template_field_start_bytes(tmpl_lan_session_hdr,
                                                                   "authentication_type")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  authentication_type_offset = rmcp_hdr_len + authentication_type_index;

  if (pkt_len < authentication_type_offset)
    return 0;

  authentication_type_recv = pkt[authentication_type_offset];

  /* authentication code check fails if authentication types do not match */
  if (authentication_type != authentication_type_recv)
    return 0;

  /* Automatically return 1 if auth type is none */
  if (authentication_type_recv == IPMI_AUTHENTICATION_TYPE_NONE)
    return 1;

  if ((authentication_code_index = fiid_template_field_start_bytes(tmpl_lan_session_hdr,
                                                                   "authentication_code")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  authentication_code_offset = rmcp_hdr_len + authentication_code_index;

  if (pkt_len < (authentication_code_offset + IPMI_1_5_MAX_PASSWORD_LENGTH))
    return 0;

  memset(authentication_code_buf, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  if (authentication_type == IPMI_AUTHENTICATION_TYPE_MD2
      || authentication_type == IPMI_AUTHENTICATION_TYPE_MD5)
    {
      int32_t session_id_index, session_sequence_number_index, data_index;
      uint32_t session_id_offset, session_sequence_number_offset, data_offset;
      int32_t session_id_len, session_sequence_number_len;
      
      if ((session_id_index = fiid_template_field_start_bytes(tmpl_lan_session_hdr,
                                                              "session_id")) < 0)
        {
          ERRNO_TRACE(errno);
          goto cleanup;
        }

      if ((session_sequence_number_index = fiid_template_field_start_bytes (tmpl_lan_session_hdr, 
                                                                            "session_sequence_number")) < 0)
        {
          ERRNO_TRACE(errno);
          goto cleanup;
        }

      if ((session_id_len = fiid_template_field_len_bytes (tmpl_lan_session_hdr,
                                                           "session_id")) < 0)
        {
          ERRNO_TRACE(errno);
          goto cleanup;
        }
      
      if ((session_sequence_number_len = fiid_template_field_len_bytes (tmpl_lan_session_hdr, 
                                                                        "session_sequence_number")) < 0)
        {
          ERRNO_TRACE(errno);
          goto cleanup;
        }

      if ((data_index = fiid_template_len_bytes(tmpl_lan_session_hdr)) < 0)
        {
          ERRNO_TRACE(errno);
          goto cleanup;
        }
      
      session_id_offset = rmcp_hdr_len + session_id_index;
      session_sequence_number_offset = rmcp_hdr_len + session_sequence_number_index;
      data_offset = rmcp_hdr_len + data_index;
      
      if (pkt_len < (session_id_offset + session_id_len)
	  || pkt_len < (session_sequence_number_offset + session_sequence_number_len)
	  || pkt_len < data_offset)
	{
	  rv = 0;
	  goto cleanup;
	}
      
      /* Must zero extend password.  No null termination is required.
       * Also, must memcpy instead of strcpy, password need not be
       * 1 word
       */
      memset(pwbuf, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
      memcpy(pwbuf, authentication_code_data, authentication_code_data_len);
      if (authentication_type == IPMI_AUTHENTICATION_TYPE_MD2)
	{
	  md2_t ctx;
	  
	  assert(IPMI_1_5_MAX_PASSWORD_LENGTH == MD2_DIGEST_LENGTH);
	  
	  md2_init(&ctx);
	  md2_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
	  md2_update_data(&ctx,
			  pkt + session_id_offset,
			  session_id_len);
	  md2_update_data(&ctx,
			  pkt + data_offset,
			  pkt_len - data_offset);
	  md2_update_data(&ctx,
			  pkt + session_sequence_number_offset,
			  session_sequence_number_len);
	  md2_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
	  md2_finish(&ctx, authentication_code_buf, IPMI_1_5_MAX_PASSWORD_LENGTH);
	  md2_init(&ctx);
	}
      else if (authentication_type == IPMI_AUTHENTICATION_TYPE_MD5)
	{
	  md5_t ctx;
	  
	  assert(IPMI_1_5_MAX_PASSWORD_LENGTH == MD5_DIGEST_LENGTH);
	  
	  md5_init(&ctx);
	  md5_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
	  md5_update_data(&ctx,
			  pkt + session_id_offset,
			  session_id_len);
	  md5_update_data(&ctx,
			  pkt + data_offset,
			  pkt_len - data_offset);
	  md5_update_data(&ctx,
			  pkt + session_sequence_number_offset,
			  session_sequence_number_len);
	  md5_update_data(&ctx, pwbuf, IPMI_1_5_MAX_PASSWORD_LENGTH);
	  md5_finish(&ctx, authentication_code_buf, IPMI_1_5_MAX_PASSWORD_LENGTH);
	  md5_init(&ctx);
	}
    }
  else /* authentication_type == IPMI_AUTHENTICATION_TYPE_STRAIGHT_PASSWORD_KEY
	  || authentication_type == IPMI_AUTHENTICATION_TYPE_OEM_PROP */
    {
      if (authentication_code_data)
	memcpy(authentication_code_buf, authentication_code_data, authentication_code_data_len);
    }

  /* Must memcmp instead of strcmp, password need not be 1 word */
  if (memcmp(authentication_code_buf, pkt + authentication_code_offset, IPMI_1_5_MAX_PASSWORD_LENGTH) == 0)
    rv = 1;
  else
    rv = 0;

 cleanup:
  secure_memset(authentication_code_buf, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  secure_memset(pwbuf, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH);
  return rv;
}

int8_t 
ipmi_lan_check_net_fn (fiid_obj_t obj_lan_msg_hdr, uint8_t net_fn)
{
  uint64_t net_fn_recv;
  int32_t len;

  if (!fiid_obj_valid(obj_lan_msg_hdr)
      || !IPMI_NET_FN_VALID(net_fn))
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_field_lookup (obj_lan_msg_hdr, "net_fn") < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((len = fiid_obj_field_len (obj_lan_msg_hdr, "net_fn")) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr);
      return (-1);
    }
  if (!len)
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_get(obj_lan_msg_hdr, "net_fn", &net_fn_recv) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  return ((((uint8_t)net_fn_recv) == net_fn) ? 1 : 0);
}

int8_t 
ipmi_lan_check_rq_seq (fiid_obj_t obj_lan_msg_hdr, uint8_t rq_seq)
{
  uint64_t rq_seq_recv;
  int32_t len;

  if (!fiid_obj_valid(obj_lan_msg_hdr))
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_field_lookup (obj_lan_msg_hdr, "rq_seq") < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((len = fiid_obj_field_len (obj_lan_msg_hdr, "rq_seq")) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr);
      return (-1);
    }
  if (!len)
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if (Fiid_obj_get(obj_lan_msg_hdr, "rq_seq", &rq_seq_recv) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  return ((((uint8_t)rq_seq_recv) == rq_seq) ? 1 : 0);
}

int8_t
ipmi_lan_check_checksum (fiid_obj_t obj_lan_msg_hdr,
                         fiid_obj_t obj_cmd,
                         fiid_obj_t obj_lan_msg_trlr)
{
  int32_t obj_lan_msg_hdr_len, obj_cmd_len, obj_len, len, req_len;
  uint8_t checksum1_recv, checksum1_calc, checksum2_recv, checksum2_calc;
  uint8_t *buf = NULL;
  uint32_t buflen;
  uint64_t val;
  
  if (!fiid_obj_valid(obj_lan_msg_hdr)
      || !fiid_obj_valid(obj_cmd)
      || !fiid_obj_valid(obj_lan_msg_trlr))
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }
  
  if (Fiid_obj_template_compare(obj_lan_msg_hdr, tmpl_lan_msg_hdr_rs) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  if (Fiid_obj_template_compare(obj_lan_msg_trlr, tmpl_lan_msg_trlr) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((len = fiid_obj_field_len (obj_lan_msg_hdr, "checksum1")) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr);
      return (-1);
    }
  if ((req_len = fiid_template_field_len(tmpl_lan_msg_hdr_rs, "checksum1")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  if (len != req_len)
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if ((len = fiid_obj_field_len (obj_lan_msg_trlr, "checksum2")) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_trlr);
      return (-1);
    }
  if ((req_len = fiid_template_field_len(tmpl_lan_msg_trlr, "checksum2")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  if (len != req_len)
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }

  if ((obj_lan_msg_hdr_len = fiid_obj_len_bytes (obj_lan_msg_hdr)) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr);
      return (-1);
    }
  if ((obj_cmd_len = fiid_obj_len_bytes (obj_cmd)) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_cmd);
      return (-1);
    }

  if (Fiid_obj_get (obj_lan_msg_hdr, "checksum1", &val) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  checksum1_recv = val;

  if (!(buf = (uint8_t *)alloca(obj_lan_msg_hdr_len)))
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((len = fiid_obj_get_block(obj_lan_msg_hdr, 
                                "rq_addr",
                                "net_fn",
                                buf, 
                                obj_lan_msg_hdr_len)) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr);
      return (-1);
    }
  checksum1_calc = ipmi_checksum(buf, len);

  if (checksum1_recv != checksum1_calc)
    return (0);

  if (Fiid_obj_get (obj_lan_msg_trlr, "checksum2", &val) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  checksum2_recv = val;

  buflen = obj_lan_msg_hdr_len + obj_cmd_len;
  if (!(buf = (uint8_t *)alloca(buflen)))
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  len = 0;
  if ((obj_len = fiid_obj_get_block(obj_lan_msg_hdr, 
                                    "rs_addr", 
                                    "rq_seq", 
                                    buf, 
                                    buflen - len)) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_lan_msg_hdr);
      return (-1);
    }
  len += obj_len;

  if ((obj_len = fiid_obj_get_all(obj_cmd, buf + len, buflen - len)) < 0)
    {
      FIID_OBJECT_ERROR_TO_ERRNO(obj_cmd);
      return (-1);
    }
  len += obj_len;

  checksum2_calc = ipmi_checksum(buf, len);
  if (checksum2_recv != checksum2_calc)
    return (0);

  return (1);
}

int8_t 
ipmi_lan_check_packet_checksum (uint8_t *pkt, uint64_t pkt_len)
{
  uint8_t authentication_type;
  uint32_t authentication_type_offset;
  int32_t rmcp_hdr_len, msg_hdr_len1, msg_hdr_len2, authentication_code_len;
  int32_t authentication_type_start_bytes;
  int32_t checksum1_block_index, checksum1_block_len, 
    checksum2_block_index, checksum2_block_len;
  uint8_t checksum1_recv, checksum1_calc, checksum2_recv, checksum2_calc;

  if (!pkt 
      || !pkt_len)
    {
      SET_ERRNO(EINVAL);
      return (-1);
    }
  
  if ((rmcp_hdr_len = fiid_template_len_bytes(tmpl_rmcp_hdr)) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if ((authentication_type_start_bytes = fiid_template_field_start_bytes(tmpl_lan_session_hdr,
                                                                         "authentication_type")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }
  
  authentication_type_offset = rmcp_hdr_len + authentication_type_start_bytes;
  authentication_type = pkt[authentication_type_offset];

  if ((msg_hdr_len1 = fiid_template_block_len_bytes(tmpl_lan_session_hdr,
                                                    "authentication_type",
                                                    "session_id")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if (authentication_type != IPMI_AUTHENTICATION_TYPE_NONE)
    authentication_code_len = IPMI_1_5_MAX_PASSWORD_LENGTH;
  else
    authentication_code_len = 0;

  if ((msg_hdr_len2 = fiid_template_field_len_bytes(tmpl_lan_session_hdr, "ipmi_msg_len")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  checksum1_block_index = rmcp_hdr_len + msg_hdr_len1 + authentication_code_len + msg_hdr_len2;

  if ((checksum1_block_len = fiid_template_block_len_bytes(tmpl_lan_msg_hdr_rs,
                                                           "rq_addr",
                                                           "net_fn")) < 0)
    {
      ERRNO_TRACE(errno);
      return (-1);
    }

  if (pkt_len < (checksum1_block_index + checksum1_block_len + 1))
    return (0);

  checksum1_calc = ipmi_checksum(pkt + checksum1_block_index, checksum1_block_len);
  checksum1_recv = pkt[checksum1_block_index + checksum1_block_len];

  if (checksum1_calc != checksum1_recv)
    return (0);

  checksum2_block_index = checksum1_block_index + checksum1_block_len + 1;

  if (pkt_len <= (checksum2_block_index + 1))
    return (0);

  checksum2_block_len = pkt_len - checksum2_block_index - 1;
  
  checksum2_calc = ipmi_checksum(pkt + checksum2_block_index, checksum2_block_len);
  checksum2_recv = pkt[checksum2_block_index + checksum2_block_len];

  if (checksum2_calc != checksum2_recv)
    return (0);

  return (1);
}
