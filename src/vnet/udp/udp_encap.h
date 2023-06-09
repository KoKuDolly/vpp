/*
 * Copyright (c) 2016-2019 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __UDP_ENCAP_H__
#define __UDP_ENCAP_H__

#include <vnet/ip/ip.h>
#include <vnet/udp/udp_packet.h>
#include <vnet/fib/fib_node.h>

/**
 * UDP encapsulation.
 * A representation of the encapsulation of packets in UDP-over-IP.
 * This is encapsulation only, there is no tunnel interface, hence
 * it is uni-directional. For decap register a handler with the UDP port
 * dispatcher.
 */

/**
 * Fixup behaviour. Actions performed on the encap in the data-plane
 */
typedef enum udp_encap_fixup_flags_t_
{
  UDP_ENCAP_FIXUP_NONE = 0,
  /**
   * UDP source port contains an entropy/hash value for load-balancing by downstream peers.
   */
  UDP_ENCAP_FIXUP_UDP_SRC_PORT_ENTROPY = (1 << 0),
} udp_encap_fixup_flags_t;

/**
 * The UDP encap representation
 */
typedef struct udp_encap_t_
{
  /**
   * The first cacheline contains the data used in the data-plane
   */
  CLIB_CACHE_LINE_ALIGN_MARK (cacheline0);

  /**
   * The headers to paint, in packet painting order
   */
  union
  {
    struct
    {
      ip4_header_t ue_ip4;
      udp_header_t ue_udp;
    } __attribute__ ((packed)) ip4;
    struct
    {
      ip6_header_t ue_ip6;
      udp_header_t ue_udp;
    } __attribute__ ((packed)) ip6;
  } __attribute__ ((packed)) ue_hdrs;

  /**
   * The DPO used to forward to the next node in the VLIB graph
   */
  dpo_id_t ue_dpo;

  /**
   * Flags controlling fixup behaviour
   */
  udp_encap_fixup_flags_t ue_flags;

  /**
   * the protocol of the IP header imposed
   */
  fib_protocol_t ue_ip_proto;

  /**
   * The second cacheline contains control-plane data
   */
  CLIB_CACHE_LINE_ALIGN_MARK (cacheline1);

  /**
   * linkage into the FIB graph
   */
  fib_node_t ue_fib_node;

  /**
   * Tracking information for the IP destination
   */
  fib_node_index_t ue_fib_entry_index;
  u32 ue_fib_sibling;

  /**
   * The FIB index in which the encap destination resides
   */
  index_t ue_fib_index;
} udp_encap_t;

extern index_t udp_encap_add_and_lock (fib_protocol_t proto,
				       index_t fib_index,
				       const ip46_address_t * src_ip,
				       const ip46_address_t * dst_ip,
				       u16 src_port,
				       u16 dst_port,
				       udp_encap_fixup_flags_t flags);

extern void udp_encap_lock (index_t uei);
extern void udp_encap_unlock (index_t uei);
extern u8 *format_udp_encap (u8 * s, va_list * args);
extern u8 *format_udp_encap_fixup_flags (u8 *s, va_list *args);
extern void udp_encap_contribute_forwarding (index_t uei,
					     dpo_proto_t proto,
					     dpo_id_t * dpo);

extern void udp_encap_get_stats (index_t uei, u64 * packets, u64 * bytes);

/**
 * Callback function invoked when walking all encap objects.
 * Return non-zero to continue the walk.
 */
typedef walk_rc_t (*udp_encap_walk_cb_t) (index_t uei, void *ctx);

/**
 * Walk each of the encap objects
 */
extern void udp_encap_walk (udp_encap_walk_cb_t cb, void *ctx);

/**
 * Pool of encaps
 */
extern udp_encap_t *udp_encap_pool;

static inline udp_encap_t *
udp_encap_get (index_t uei)
{
  return (pool_elt_at_index (udp_encap_pool, uei));
}

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */

#endif
