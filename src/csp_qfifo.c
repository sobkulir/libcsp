/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
Copyright (C) 2012 AAUSAT3 Project (http://aausat3.space.aau.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "csp_qfifo.h"

#include <csp/arch/csp_queue.h>
#include <csp/csp_debug.h>
#include <csp/csp_buffer.h>
#include <csp_autoconfig.h>

#define QFIFO_LEN 10

static csp_static_queue_t qfifo_queue;
static csp_queue_handle_t qfifo_queue_handle;
char qfifo_queue_buffer[sizeof(csp_qfifo_t) * QFIFO_LEN];

void csp_qfifo_init(void) {
	qfifo_queue_handle = csp_queue_create_static(QFIFO_LEN, sizeof(csp_qfifo_t), qfifo_queue_buffer, &qfifo_queue);
}

int csp_qfifo_read(csp_qfifo_t * input) {

	if (csp_queue_dequeue(qfifo_queue_handle, input, FIFO_TIMEOUT) != CSP_QUEUE_OK)
		return CSP_ERR_TIMEDOUT;

	return CSP_ERR_NONE;

}

void csp_qfifo_write(csp_packet_t * packet, csp_iface_t * iface, void * pxTaskWoken) {

	int result;

	if (packet == NULL) {
		if (pxTaskWoken == NULL) { // Only do logging in non-ISR context
			csp_log_warn("csp_new packet called with NULL packet");
		}
		return;
	}

	if (iface == NULL) {
		if (pxTaskWoken == NULL) { // Only do logging in non-ISR context
			csp_log_warn("csp_new packet called with NULL interface");
		}
		if (pxTaskWoken == NULL)
			csp_buffer_free(packet);
		else
			csp_buffer_free_isr(packet);
		return;
	}

	csp_qfifo_t queue_element;
	queue_element.iface = iface;
	queue_element.packet = packet;

	if (pxTaskWoken == NULL)
		result = csp_queue_enqueue(qfifo_queue_handle, &queue_element, 0);
	else
		result = csp_queue_enqueue_isr(qfifo_queue_handle, &queue_element, pxTaskWoken);

	if (result != CSP_QUEUE_OK) {
		if (pxTaskWoken == NULL) { // Only do logging in non-ISR context
			csp_log_warn("ERROR: Routing input FIFO is FULL. Dropping packet.");
		}
		iface->drop++;
		if (pxTaskWoken == NULL)
			csp_buffer_free(packet);
		else
			csp_buffer_free_isr(packet);
	}

}

void csp_qfifo_wake_up(void) {
	const csp_qfifo_t queue_element = {.iface = NULL, .packet = NULL};
	csp_queue_enqueue(qfifo_queue_handle, &queue_element, 0);
}
