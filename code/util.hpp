#ifndef SM_UTIL_H
#define SM_UTIL_H

#include <string>
#include <cstdint>
#include <fstream>

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;

namespace sm {
	struct Wav_File {
		u32 chunk_id; // 'RIFF' (big-endian)
		u32 chunk_size;
		u32 format; // 'WAVE' (big-endian)

		u32 fmt_chunk_id; // 'fmt ' (big-endian)
		u32 fmt_chunk_size;
		u16 audio_format;
		u16 num_channels;
		u32 sample_rate;
		u32 byte_rate;
		u16 block_align; // 'data' (big-endian)
		u16 bits_per_sample;
		//u16 extra_param_size;
		//extra_params

		u32 data_chunk_id; // B
		u32 data_chunk_size;

		u8 *data;

		std::ifstream in;
		
		Wav_File &operator>>(u16 &data);
		Wav_File &operator>>(u32 &data);

		void read(const std::string &file_name);
		void invalidate();
		bool valid();

		~Wav_File();
	};
}

#endif // SM_UTIL_H
