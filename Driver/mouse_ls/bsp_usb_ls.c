#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

static struct input_dev *uk_dev;
static char  *usb_buf;
static dma_addr_t usb_buf_phys;
static int len;
static struct urb *uk_urb;

static void  mouse_irq(struct urb *urb)
{
    static unsigned char per_val;
    #if 0
    int i ;
    static int cnt = 0 ;
    printk("cnt=%d",++cnt);
    for (i=0;i<len;i++)
    {
        printk("%02x  ",usb_buf[i]);
    }
    printk("\n");
    #endif
    /* USB ������ݵĺ���
     * data[0]:bit0-�����1���£�0�ɿ�
     * data[0]:bit1-�Ҽ���1���£�0�ɿ�
     * data[0]:bit2-�м���1���£�0�ɿ�
     *
     */

    if(per_val & (1<<0) != (usb_buf[0] & (1<<0)))
    {
        /*��������˱仯*/
        input_event(uk_dev,EV_KEY,KEY_L,(usb_buf[0] & (1<<0)? 1 : 0 ));
        input_sync(uk_dev);
    }
    if(per_val & (1<<1) != (usb_buf[0] & (1<<1)))
    {
         /*�Ҽ������˱仯*/
        input_event(uk_dev,EV_KEY,KEY_S,(usb_buf[0] & (1<<1)? 1 : 0 ));
        input_sync(uk_dev);
    }
    if(per_val & (1<<2) != (usb_buf[0] & (1<<2)))
    {
        /*�м������˱仯*/
        input_event(uk_dev,EV_KEY,KEY_ENTER,(usb_buf[0] & (1<<2)? 1 : 0 ));
        input_sync(uk_dev);
    }
    per_val = usb_buf[0];
    /*�����ύURB*/
     usb_submit_urb(uk_urb, GFP_KERNEL);
    
}



static int mouse_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    struct usb_device *dev = interface_to_usbdev(intf);
    struct usb_host_interface *interface;
    struct usb_endpoint_descriptor *endpoint;

    
    int pipe;
    interface = intf->cur_altsetting;
    endpoint = &interface->endpoint[0].desc;

    printk("mouse found\n");
    /*1�� ����һ�� input dev �ṹ��  */
    uk_dev = input_allocate_device();
    /*2������  */
    /*2.1���ܲ��������¼�  */
    set_bit(EV_KEY, uk_dev->evbit);
    set_bit(EV_REP, uk_dev->evbit);
    /*2.2���ܲ�����Щ�¼�  */
    set_bit(KEY_L,uk_dev->keybit);
    set_bit(KEY_S,uk_dev->keybit);
    set_bit(KEY_ENTER,uk_dev->keybit);
    /*3��ע�� */
    input_register_device(uk_dev);
 
    /*4��Ӳ����صĲ���*/

    /*���ݴ�����Ҫ��*/
    /*Դ��USB�豸��ĳ���˵�*/
    pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
    /*����*/
    len = endpoint->wMaxPacketSize;
    /*Ŀ�ģ�*/
    usb_buf = usb_buffer_alloc(dev, len, GFP_ATOMIC, &usb_buf_phys);
 
    /*ʹ����Ҫ��*/
    /*���� usb request block   */
    uk_urb = usb_alloc_urb(0, GFP_KERNEL);

    usb_fill_int_urb(uk_urb, dev, pipe, usb_buf,len,mouse_irq, NULL, endpoint->bInterval);
    uk_urb->transfer_dma =usb_buf_phys;
    uk_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
    /*ʹ��URB*/
    usb_submit_urb(uk_urb, GFP_KERNEL);
    return 0 ;
}



static void mouse_disconnect(struct usb_interface *intf)
{
    struct usb_device *dev = interface_to_usbdev(intf);
    printk("mouse disconnect\n");
	usb_kill_urb(uk_urb);
    usb_free_urb(uk_urb);
    usb_buffer_free(dev, len, usb_buf , usb_buf_phys) ; 
    input_unregister_device(uk_dev);
    input_free_device(uk_dev);
 
}




static struct usb_device_id mouse_id_table [] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_MOUSE) },
	{ }	/* Terminating entry */
};

/*1�������� */
static struct usb_driver mouse_driver = {
	.name		= "mouse",
	.probe		= mouse_probe,
	.disconnect	= mouse_disconnect,
	.id_table	= mouse_id_table,
};

static int  usbmouse_init(void)
{

	/*2. ע��*/
    usb_register(&mouse_driver);
	
	return 0;
}




static void usbmouse_exit(void)
{
	usb_deregister(&mouse_driver);
}


module_init(usbmouse_init);
module_exit(usbmouse_exit);
MODULE_LICENSE("GPL");

















































