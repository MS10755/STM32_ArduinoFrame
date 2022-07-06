#ifndef __DEBUG_WRAPPER_H__
#define __DEBUG_WRAPPER_H__

void Debug_printf(int const debug_level,char * fmt,...);
void Debug_fprintf(int const debug_level,void *f,char * fmt,...);

void Debug_printfCPP(int const debug_level,char * fmt,...);
void Debug_fprintfCPP(int const debug_level,void *f,char * fmt,...);

#endif
