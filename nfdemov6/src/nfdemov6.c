/*
 *  Netfilter IPv6 Demo, NAT
 *  Autor : Mason
 *  Date  : 20180731
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/socket.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <net/dsfield.h>
#include <linux/skbuff.h>
#include <linux/inet.h>
#include <linux/netdevice.h>

#include <net/route.h>
#include "nfdemov6.h"





static unsigned int nfdemo_input_hook_v6(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{   
    /* do something */
    
    return NF_ACCEPT;                          
}


static unsigned int nfdemo_output_hook_v6(unsigned int hooknum,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{          
    struct ipv6hdr *iph6;
    struct udphdr *udph;    
    unsigned int udp_len;
    
    iph6 = ipv6_hdr(skb);
    if (!iph6)
        return NF_ACCEPT;

    /* 过滤 ::/128 类型地址 */
    if (ipv6_addr_any(&iph6->saddr) || ipv6_addr_any(&iph6->daddr))
        return NF_ACCEPT;

    /* 过滤源地址和目的地址相等的报文 */
    if (ipv6_addr_equal(&iph6->saddr, &iph6->daddr))
        return NF_ACCEPT;

    /* 过滤环回地址报文 ::1/128 */
    if (ipv6_addr_loopback(&iph6->saddr) || ipv6_addr_loopback(&iph6->daddr))        
        return NF_ACCEPT;

    /* 只处理UDP报文 */    
    if (iph6->nexthdr != NEXTHDR_UDP)
        return NF_ACCEPT;

    /* 设置传输层首部 */
    skb_set_transport_header(skb, sizeof(struct ipv6hdr));

    /* 获取UDP首部 */
    udph = udp_hdr(skb);
    if (!udph )
        return NF_ACCEPT;

    /* 更改端口 */        
    udph->source = htons(6666);    

    /* 重新计算校验和 */        
    udph->check = 0; 
    udp_len = ntohs(udph->len);                               
    skb->csum = csum_partial(skb_transport_header(skb), udp_len, 0);

    /*
     * IPv6校验和计算使用 csum_ipv6_magic() 接口
     * IPv4校验和计算使用 csum_tcpudp_magic() 接口
     */
    udph->check = csum_ipv6_magic(&iph6->saddr, &iph6->daddr, udp_len, IPPROTO_UDP, skb->csum);        
    skb->ip_summed = CHECKSUM_NONE;

    /* 如果udp首部校验和为0，替换成CSUM_MANGLED_0 */
    if (0 == udph->check)
        udph->check = CSUM_MANGLED_0;

    /* NAT 更改完成 */
    return NF_ACCEPT;               
}


struct nf_hook_ops nfdemo_hook_ops[] ={
    {
        .hook = nfdemo_input_hook_v6,       /* 钩子处理函数 */
        .pf = NFPROTO_IPV6,                 /* 协议类型IPv6 */
        .hooknum = NF_INET_PRE_ROUTING,     /* Pre_Routing链 */
        .priority = NF_IP_PRI_FIRST + 20,   /* 优先级 */
    },
    {
        .hook = nfdemo_output_hook_v6,        /* 钩子处理函数 */
        .pf = NFPROTO_IPV6,                   /* 协议类型IPv6 */
        .hooknum = NF_INET_PRE_ROUTING,       /* Post_Routing链 */
        .priority = NF_IP_PRI_FIRST + 20,     /* 优先级 */
    },
    {}
};

static int __init nfdemov6_init(void)
{
    printk("nfv6demo init \r\n");

    /* 注册 Netfilter 钩子函数 */
    nf_register_hooks(nfdemo_hook_ops,ARRAY_SIZE(nfdemo_hook_ops));    
    return 0;
}
static void __exit nfdemov6_exit(void)
{
    printk("nfv6demo exit \r\n");

    /* 注销 Netfilter 钩子函数 */
    nf_unregister_hooks(nfdemo_hook_ops,ARRAY_SIZE(nfdemo_hook_ops));    
    return ;
}


module_init(nfdemov6_init)
module_exit(nfdemov6_exit)
MODULE_LICENSE("GPL");



