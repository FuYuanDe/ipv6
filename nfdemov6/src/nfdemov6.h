#ifndef __NFDEMO_H__
#define __NFDEMO_H__
#define log_info(fmt, arg...) printk(KERN_EMERG"[nfdemo] %s:%d "fmt,__FUNCTION__,__LINE__,##arg)
#define MAC(addr) addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]

#ifndef NMACQUAD
#define NMACQUAD(mac)  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
#endif

#ifndef NIPQUAD
#define NIPQUAD(addr) \
    ((unsigned char *)&addr)[0], \
    ((unsigned char *)&addr)[1], \
    ((unsigned char *)&addr)[2], \
    ((unsigned char *)&addr)[3]
#endif

#ifndef NIP1
#define NIP1(addr)  ((unsigned char *)&addr)[0]
#define NIP2(addr)  ((unsigned char *)&addr)[1]
#define NIP3(addr)  ((unsigned char *)&addr)[2]
#define NIP4(addr)  ((unsigned char *)&addr)[3]
#endif

#ifndef NIPQUAD6
#define NIPQUAD6(addr) \
    ((unsigned char *)&addr)[0], \
    ((unsigned char *)&addr)[1], \
    ((unsigned char *)&addr)[2], \
    ((unsigned char *)&addr)[3], \
    ((unsigned char *)&addr)[4], \
    ((unsigned char *)&addr)[5], \
    ((unsigned char *)&addr)[6], \
    ((unsigned char *)&addr)[7], \
    ((unsigned char *)&addr)[8], \
    ((unsigned char *)&addr)[9], \
    ((unsigned char *)&addr)[10], \
    ((unsigned char *)&addr)[11], \
    ((unsigned char *)&addr)[12], \
    ((unsigned char *)&addr)[13], \
    ((unsigned char *)&addr)[14], \
    ((unsigned char *)&addr)[15]        
#endif

#endif