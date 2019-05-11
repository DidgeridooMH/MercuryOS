#ifndef STDARG_H
#define STDARG_H

typedef char* va_list;

#define va_start(ap, la) \
    ((ap) = ((va_list)(&(la)) + sizeof(la)))

#define va_arg(ap, type) \
    (*((type*)(((ap) = ((ap) + sizeof(type))) - sizeof(type))))

#define va_end(ap) \
    ((ap) = (void(0)))

#endif
