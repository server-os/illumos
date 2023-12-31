/*
 * Copyright 1993 by OpenVision Technologies, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appears in all copies and
 * that both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of OpenVision not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission. OpenVision makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * OPENVISION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OPENVISION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * $Id: util_buffer.c 11001 1998-10-30 02:56:35Z marc $
 */

#include "gssapiP_generic.h"
#include <string.h>

/* return nonzero on success, 0 on failure
   make sure that buffer is consistent (release'able) when this
   function exits, no matter what the exit value */

int g_make_string_buffer(str, buffer)
     const char *str;
     gss_buffer_t buffer;
{
   buffer->length = strlen(str);

   if ((buffer->value = (void *) xmalloc(buffer->length + 1)) == NULL) {
      buffer->length = 0;
      return(0);
   }

   strcpy(buffer->value, str);

   return(1);
}
