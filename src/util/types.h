#pragma once
#include <stdint.h>
namespace astron
{
	typedef uint64_t channel_t;
	typedef uint32_t doid_t;
	typedef uint32_t zone_t;
}

#define CHANNEL_MAX ((channel_t)(0) - 1)
#define CHANNEL_SIZE_BYTES 8
#define DOID_SIZE_BYTES 4
#define ZONE_SIZE_BITS 32
#define ZONE_SIZE_BYTES (ZONE_SIZE_BITS/8)

// Type constants
#define INVALID_CHANNEL 0
#define INVALID_DO_ID 0

// Message boolean values
#define SUCCESS true
#define FAILURE false

// Reserve message-type
#define RESERVED_MSG_TYPE 0