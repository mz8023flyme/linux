/*
 *   dirve\net\Cs89x0.c
 *
 */

#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>


static struct net_device * vnet;

static void my_rx_packet(struct sk_buff *skb,struct net_device *dev)
{
    /*�Ե�ԭĿ��ip      ����*/
}

static int virt_net_sent_packet (struct sk_buff *skb,struct net_device *dev)
{
    static int cnt = 0;
    printk("cnt = %d\n",++cnt);
    //dev_kfree_skb (skb);
    /*������ʵ����������skb�������ͨ���������ͳ�ȥ*/

    netif_stop_queue(dev);//ֹͣ�������Ķ���

    my_rx_packet(skb,dev);    //��skb������д������

    dev_kfree_skb(skb);   // �ͷ�skb 
    netif_wake_queue(dev);  // ����ȫ�����ͳ�ȥ������������
    dev->stats.tx_packets++;
    dev->stats.tx_bytes+=skb->len;

    
    return 0;
}


static int virt_net_init(void)
{
    /*1.����һ��net_drivce�ṹ��*/
    vnet= alloc_netdev(0,"vent%d",ether_setup);

    /*2.����*/
    vnet->hard_start_xmit = virt_net_sent_packet;
    /*����MAC��ַ*/
    vnet->dev_addr[0] = 0x01;
    vnet->dev_addr[1] = 0x81;
    vnet->dev_addr[2] = 0x41;
    vnet->dev_addr[3] = 0x21;
    vnet->dev_addr[4] = 0x51;
    vnet->dev_addr[5] = 0x61;
    
    vnet->flags|=IFF_NOARP;
    vnet->features|=NETIF_F_NO_CSUM;
    /*3.ע��*/
    register_netdev(vnet);
    return 0;
}

static void virt_net_exit(void)
{
    free_netdev(vnet);
    unregister_netdev(vnet);

}




module_init(virt_net_init);
module_exit(virt_net_exit);
MODULE_LICENSE("GPL");
 








