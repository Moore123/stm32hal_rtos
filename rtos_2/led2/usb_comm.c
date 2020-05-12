/*

#include "usb_comm.h"
#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <stdlib.h>
#include <stdio.h>

static const struct usb_device_descriptor dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = USB_CLASS_CDC,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0x0483,
	.idProduct = 0x5740,
	.bcdDevice = 0x0200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

static const struct usb_endpoint_descriptor comm_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x83,
	.bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
	.wMaxPacketSize = 16,
	.bInterval = 255,
}};

static const struct usb_endpoint_descriptor data_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x01,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 1,
}, {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x82,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 1,
}};

static const struct {
	struct usb_cdc_header_descriptor header;
	struct usb_cdc_call_management_descriptor call_mgmt;
	struct usb_cdc_acm_descriptor acm;
	struct usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) cdcacm_functional_descriptors = {
	.header = {
		.bFunctionLength = sizeof(struct usb_cdc_header_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_HEADER,
		.bcdCDC = 0x0110,
	},
	.call_mgmt = {
		.bFunctionLength =
			sizeof(struct usb_cdc_call_management_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,
		.bmCapabilities = 0,
		.bDataInterface = 1,
	},
	.acm = {
		.bFunctionLength = sizeof(struct usb_cdc_acm_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_ACM,
		.bmCapabilities = 0,
	},
	.cdc_union = {
		.bFunctionLength = sizeof(struct usb_cdc_union_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_UNION,
		.bControlInterface = 0,
		.bSubordinateInterface0 = 1,
	 },
};

static const struct usb_interface_descriptor comm_iface[] = {{
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_CDC,
	.bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
	.bInterfaceProtocol = USB_CDC_PROTOCOL_AT,
	.iInterface = 0,

	.endpoint = comm_endp,

	.extra = &cdcacm_functional_descriptors,
	.extralen = sizeof(cdcacm_functional_descriptors),
}};

static const struct usb_interface_descriptor data_iface[] = {{
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 1,
	.bAlternateSetting = 0,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_DATA,
	.bInterfaceSubClass = 0,
	.bInterfaceProtocol = 0,
	.iInterface = 0,

	.endpoint = data_endp,
}};

static const struct usb_interface ifaces[] = {{
	.num_altsetting = 1,
	.altsetting = comm_iface,
}, {
	.num_altsetting = 1,
	.altsetting = data_iface,
}};

static const struct usb_config_descriptor config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 2,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0x80,
	.bMaxPower = 0x32,

	.interface = ifaces,
};

static const char *usb_strings[] = {
	"Black Sphere Technologies",
	"CDC-ACM Demo",
	"DEMO",
};

#define USB_COMM_RX_BUFFER_LEN 40
uint8_t usbd_control_buffer[128];
volatile uint8_t usbd_rx_buffer[USB_COMM_RX_BUFFER_LEN];
volatile uint8_t usbd_rx_wr = 0, usbd_rx_rd = 0;

static enum usbd_request_return_codes cdcacm_control_request(usbd_device *usbd_dev, struct usb_setup_data *req, uint8_t **buf,
		uint16_t *len, void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req))
{
	(void)complete;
	(void)buf;
	(void)usbd_dev;

	switch (req->bRequest) {
	case USB_CDC_REQ_SET_CONTROL_LINE_STATE: {

		char local_buf[10];
		struct usb_cdc_notification *notif = (void *)local_buf;

		notif->bmRequestType = 0xA1;
		notif->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
		notif->wValue = 0;
		notif->wIndex = 0;
		notif->wLength = 2;
		local_buf[8] = req->wValue & 3;
		local_buf[9] = 0;
		// usbd_ep_write_packet(0x83, buf, 10);
		return USBD_REQ_HANDLED;
		}
	case USB_CDC_REQ_SET_LINE_CODING:
		if (*len < sizeof(struct usb_cdc_line_coding))
			return USBD_REQ_NOTSUPP;
		return USBD_REQ_HANDLED;
	}
	return USBD_REQ_NOTSUPP;
}

static void cdcacm_data_rx_cb(usbd_device *usbd_dev, uint8_t ep)
{
	(void)ep;
	(void)usbd_dev;

	char buf[64];
	int len = usbd_ep_read_packet(usbd_dev, 0x01, buf, 64);
	int availableLen = (usbd_rx_wr < usbd_rx_rd) ? usbd_rx_rd - usbd_rx_wr : (USB_COMM_RX_BUFFER_LEN - usbd_rx_wr) + usbd_rx_rd;
	if(availableLen < len){
		printf("error: usb rx buffer overflow");
		return;
	}
	for(int i = 0;i < len;i++)
		usbd_rx_buffer[(usbd_rx_wr+i) % USB_COMM_RX_BUFFER_LEN] = buf[i];
	usbd_rx_wr = (usbd_rx_wr+len) % USB_COMM_RX_BUFFER_LEN;

	printf("[");
	for(int i = 0;i < 4;i++)
		printf("0x%02x 0x%02x 0x%02x 0x%02x ",
				(int)usbd_rx_buffer[4 * i],
				(int)usbd_rx_buffer[4 * i + 1],
				(int)usbd_rx_buffer[4 * i + 2],
				(int)usbd_rx_buffer[4 * i + 3]
		);
	printf("]\r\n");


}
static void cdcacm_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
	(void)wValue;
	(void)usbd_dev;

	usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, 64, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, 0x82, USB_ENDPOINT_ATTR_BULK, 64, NULL);
	usbd_ep_setup(usbd_dev, 0x83, USB_ENDPOINT_ATTR_INTERRUPT, 16, NULL);

	usbd_register_control_callback(
				usbd_dev,
				USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
				USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
				cdcacm_control_request);
}

static usbd_device *usbd_dev;
volatile int txn = 0;

void tim2_isr(void){
	if(timer_get_flag(TIM2, TIM_SR_UIF)){
		timer_clear_flag(TIM2, TIM_SR_UIF);
		if(!txn)
			usbd_poll(usbd_dev);
	}
}

void usb_comm_init(){
	usbd_dev = usbd_init(&st_usbfs_v1_usb_driver, &dev, &config, usb_strings, 3, usbd_control_buffer, sizeof(usbd_control_buffer));
	usbd_register_set_config_callback(usbd_dev, cdcacm_set_config);

	for(int i = 0;i < 0xfffff;i++) usbd_poll(usbd_dev);

	nvic_enable_irq(NVIC_TIM2_IRQ);
	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_reset_pulse(RST_TIM2);

	timer_set_prescaler(TIM2, 4800);
	timer_continuous_mode(TIM2);
	timer_set_period(TIM2, 10);

	timer_enable_irq(TIM2, TIM_DIER_UIE);

	timer_enable_counter(TIM2);
	usbd_poll(usbd_dev);


}
int usb_comm_write(const char* buffer, int len){
	txn = 1;
	int r = usbd_ep_write_packet(usbd_dev, 0x82, buffer, len);
	txn = 0;
	return r;
}
int usb_comm_read(char* buffer, int len){
	int read_len = usb_comm_read_available();
	read_len = len < read_len ? len : read_len;

	for(int i = 0;i < read_len;i++)
		buffer[i] = usbd_rx_buffer[(usbd_rx_rd+i) % USB_COMM_RX_BUFFER_LEN];

	usbd_rx_rd = (usbd_rx_rd + read_len) % USB_COMM_RX_BUFFER_LEN;
	return read_len;
}

uint32_t usb_comm_read_int(){
	uint32_t tmp = 0;
	usbd_rx_rd = usbd_rx_wr;
	while(1){
		while(usbd_rx_rd == usbd_rx_wr);
		int l = usb_comm_read_available();
		for(int i = 0;i < l;i++){
			char c = usbd_rx_buffer[(usbd_rx_rd+i) % USB_COMM_RX_BUFFER_LEN];
			if(c >= '0' && c <= '9')
				tmp = 10*tmp + c - '0';
			else {
				usbd_rx_rd = (usbd_rx_rd + i) % USB_COMM_RX_BUFFER_LEN;
				return tmp;
			}
		}

	}


}

uint32_t usb_comm_read_available(){
	return (usbd_rx_wr > usbd_rx_rd) ? usbd_rx_wr - usbd_rx_rd : usbd_rx_wr + USB_COMM_RX_BUFFER_LEN - usbd_rx_rd;

}
*/
