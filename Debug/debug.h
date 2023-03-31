#ifndef LANGUAGE_DEBUG_H
#define LANGUAGE_DEBUG_H

const int WRONG = 0xBAD;

#define PRINT(str)     { fprintf(__OUT__, "%s\n", #str                                                              );   }
#define PRINT_LINE     { fprintf(__OUT__, "I'm at %s at line %d in %s\n", __PRETTY_FUNCTION__, __LINE__, __FILE__   );   }
#define PRINT_(str)    { fprintf(__OUT__, "[%s:%d] %s\n",                 __PRETTY_FUNCTION__, __LINE__, #str       );   }
#define PRINT_PTR(ptr) { fprintf(__OUT__, "[%s:%d] pointer %s at %p\n",   __PRETTY_FUNCTION__, __LINE__, #ptr,  ptr );   }
#define PRINT_C(char)  { fprintf(__OUT__, "[%s:%d] %s = %c  \n",          __PRETTY_FUNCTION__, __LINE__, #char, char);   }
#define PRINT_S(str)   { fprintf(__OUT__, "[%s:%d] %s = %s  \n",          __PRETTY_FUNCTION__, __LINE__, #str,  str );   }
#define PRINT_LU(num)  { fprintf(__OUT__, "[%s:%d] %s = %lu \n",          __PRETTY_FUNCTION__, __LINE__, #num,  num );   }
#define PRINT_D(num)   { fprintf(__OUT__, "[%s:%d] %s = %d  \n",          __PRETTY_FUNCTION__, __LINE__, #num,  num );   }
#define PRINT_X(num)   { fprintf(__OUT__, "[%s:%d] %s = %x  \n",          __PRETTY_FUNCTION__, __LINE__, #num,  num );   }
#define PRINT_SM(s, n) { fprintf(__OUT__, "[%s:%d] %s = %.*s\n",          __PRETTY_FUNCTION__, __LINE__, #s,    n, s);   }

#endif //LANGUAGE_DEBUG_H
