
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include "ConvertUTF.h"
#include <stdlib.h>
#include <string.h>

typedef ConversionResult (*conversion_func)
    (const char**, const char*, const char**, const char*, ConversionFlags);

static int
perform_conversion(conversion_func f, const char *src, int len, char **dst, int *dst_len)
{
 int size  = len;
 int ok    = 0;
 int retry = 1;
 char *dst_start = NULL;
 char *dst_end   = NULL;

 while(retry) {
     const char *src_ = src;

     size = size + (size >> 1);
     if(dst_start) free(dst_start);
     dst_start = dst_end = malloc(size);

     switch((*f)(&src_, src_+len, (const char **)&dst_end, dst_end+size, lenientConversion)) {
	 case conversionOK:
	     retry = 0;
	     ok = 1;
	     break;
	 case targetExhausted:
	     break;
	 case sourceExhausted:
	 case sourceIllegal:
	     retry = 0;
     }
 }

 if(ok)  {
     *dst     = dst_start;
     *dst_len = dst_end - dst_start;
     return 1;
 }

 if(dst_start) free(dst_start);
 return 0;
}

static value
wrap_conversion(conversion_func f, value s)
{
 CAMLparam1(s);
 CAMLlocal2(ret, rets);

 char *dst;
 int  dst_len = 0;

 ret = Val_unit;

 if(perform_conversion(f, String_val(s), string_length(s), &dst, &dst_len)) {
     rets = caml_alloc_string(dst_len);
     memcpy(String_val(rets), dst, dst_len);
     Byte_u(rets, dst_len) = '\0';
     ret  = caml_alloc_small(1, 0);
     Field(ret, 0) = rets;
     free(dst);
 }

 CAMLreturn(ret);
}

#define CONV_FUNC(name, func) \
    CAMLprim value win32unixw_convert_##name(value s) { \
	CAMLparam1(s); \
	CAMLlocal1(ret); \
	ret = wrap_conversion((conversion_func) func, s); \
	CAMLreturn(ret); \
    }

CONV_FUNC(utf8_to_utf16, ConvertUTF8toUTF16)

CONV_FUNC(utf16_to_utf8, ConvertUTF16toUTF8)
