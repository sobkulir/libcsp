target_sources(libcsp PRIVATE
  csp_bridge.c
  csp_buffer.c
  csp_conn.c
  csp_crc32.c
  csp_debug.c
  csp_dedup.c
  csp_hex_dump.c
  csp_id.c
  csp_iflist.c
  csp_init.c
  csp_io.c
  csp_port.c
  csp_promisc.c
  csp_qfifo.c
  csp_route.c
  csp_rtable_cidr.c
  csp_service_handler.c
  csp_services.c
  csp_rdp.c
  csp_rdp_queue.c
  )

# This is a hack to not include SFP module in Zephyr build because
# it uses malloc which doesn't work in Zephyr out of box.
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Zephyr")
  target_sources(libcsp PRIVATE csp_sfp.c)
endif()

if (CSP_HAVE_STDIO)
  target_sources(libcsp PRIVATE csp_rtable_stdio.c)
endif()

add_subdirectory(arch)
add_subdirectory(crypto)
add_subdirectory(drivers)
add_subdirectory(interfaces)
