#ifndef SM_UTIL_HPP
#define SM_UTIL_HPP

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8  = std::uint8_t;

namespace sm {
	constexpr int WAV_FILE_HEADER_SIZE = 44;
	// TODO: Handle additional chunks that can come after data chunk.
	// TODO: Make better interface for partial data reads.
	struct Wav_File {
		u32 chunk_id;		// 'RIFF' (big-endian)
		u32 chunk_size;
		u32 format;			// 'WAVE' (big-endian)

		u32 fmt_chunk_id;	// 'fmt ' (big-endian)
		u32 fmt_chunk_size;
		u16 audio_format;
		u16 num_channels;
		u32 sample_rate;
		u32 byte_rate;
		u16 block_align;	
		u16 bits_per_sample;
		//u16 extra_param_size;
		//extra_params

		u32 data_chunk_id;  // 'data' (big-endian)
		u32 data_chunk_size;

		u8 *data;

		std::ifstream in;
		std::ofstream out;
		
		Wav_File &operator>>(u16 &data);
		Wav_File &operator>>(u32 &data);
		
		void read_meta(const std::string &file_name);
		bool read_data(u8 *buffer, u32 size);
		void read_all(const std::string &file_name);

		void write_meta(const std::string &file_name);
		bool write_data(u8 *buffer, u32 size);
		void write_all(const std::string &file_name);
		
		void invalidate();
		bool valid();

		void print_meta();

		~Wav_File();
	};

	void reverse(u8 *s, int n);
}

#endif // SM_UTIL_HPP
