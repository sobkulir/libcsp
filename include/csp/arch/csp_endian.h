// Taken from here: https://github.com/zephyrproject-rtos/zephyr/pull/38939

/*
 * Copyright (c) 2021 Space Cubics, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#if (CSP_ZEPHYR)

#include <stdint.h>
#include <zephyr/sys/byteorder.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline uint16_t htobe16(uint16_t val) { return sys_cpu_to_be16(val); }
static inline uint16_t htole16(uint16_t val) { return sys_cpu_to_le16(val); }
static inline uint16_t be16toh(uint16_t val) { return sys_be16_to_cpu(val); }
static inline uint16_t le16toh(uint16_t val) { return sys_le16_to_cpu(val); }

static inline uint32_t htobe32(uint32_t val) { return sys_cpu_to_be32(val); }
static inline uint32_t htole32(uint32_t val) { return sys_cpu_to_le32(val); }
static inline uint32_t be32toh(uint32_t val) { return sys_be32_to_cpu(val); }
static inline uint32_t le32toh(uint32_t val) { return sys_le32_to_cpu(val); }

static inline uint64_t htobe64(uint64_t val) { return sys_cpu_to_be64(val); }
static inline uint64_t htole64(uint64_t val) { return sys_cpu_to_le64(val); }
static inline uint64_t be64toh(uint64_t val) { return sys_be64_to_cpu(val); }
static inline uint64_t le64toh(uint64_t val) { return sys_le64_to_cpu(val); }

#ifdef __cplusplus
}
#endif

#elif (CSP_FREERTOS) || (CSP_POSIX)
#include <endian.h>
#endif
