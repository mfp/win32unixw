
static CAMLprim value
copy_utf16_string(LPCWSTR s)
{
 CAMLparam0();
 CAMLlocal1(ret);
 unsigned long len = 0;
 LPCWSTR p = s;

 while(*p++ != L'\000') len++;
 ret = caml_alloc_string(sizeof(WCHAR) * len);
 memcpy(String_val(ret), (char *)s, sizeof(WCHAR) * len);

 CAMLreturn(ret);
}
