#ifndef SM_UTIL_H
#define SM_UTIL_H

#include <string>
#include <cstdint>

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;

namespace sm {
	
	struct Wav_File {
		u32 chunk_id; // big
		u32 chunk_size;
		u32 format; // big

		u32 fmt_chunk_id; // big
		u32 fmt_chunk_size;
		u16 audio_format;
		u16 num_channels;
		u32 sample_rate;
		u32 byte_rate;
		u16 block_align;
		u16 bits_per_sample;
		//u16 extra_param_size;
		//extra_params

		u32 data_chunk_id; // big
		u32 data_chunk_size;

		u8 *data;
	};

	Wav_File read_wav_file(const std::string &file_name);
}

#endif // SM_UTIL_H
