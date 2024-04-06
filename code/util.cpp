#include "util.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>

#define WAV_READ_ERROR_RETURN(wf) do {(wf)->invalidate(); return;} while(0)

namespace sm {
	Wav_File &Wav_File::operator>>(u16 &data) {
		in.read((char *)&data, sizeof(data));
		return *this;
	}

	Wav_File &Wav_File::operator>>(u32 &data) {
		in.read((char *)&data, sizeof(data));
		return *this;
	}
	
	void Wav_File::read_meta(const std::string &file_name) {
		in.open(file_name, std::ios_base::binary);

		// This is faster if we know that read bytes map precisely to structure.
		//if(in) {
		//	in.read((char *)this, WAV_FILE_HEADER_SIZE);
		//}

		// This is safer, but a bit slower.
		if(in) {
			*this >> chunk_id;
			// RIFF (big endian)
			if(chunk_id != 0x46464952) WAV_READ_ERROR_RETURN(this);
			*this >> chunk_size;
			*this >> format;
			// WAVE (big endian)
			if(format != 0x45564157) WAV_READ_ERROR_RETURN(this);
			*this >> fmt_chunk_id;
			if(fmt_chunk_id != 0x20746d66) WAV_READ_ERROR_RETURN(this);
			*this >> fmt_chunk_size;
			*this >> audio_format;
			// If format is PCM then fmt chunk size must be 16?
			if(audio_format == 1 && fmt_chunk_size != 16) WAV_READ_ERROR_RETURN(this);
			*this >> num_channels;
			*this >> sample_rate;
			*this >> byte_rate;
			*this >> block_align;
			*this >> bits_per_sample;
			if(byte_rate != sample_rate * num_channels * bits_per_sample / 8) WAV_READ_ERROR_RETURN(this);
			if(block_align != num_channels * bits_per_sample / 8) WAV_READ_ERROR_RETURN(this);
			*this >> data_chunk_id;
			if(data_chunk_id != 0x61746164) WAV_READ_ERROR_RETURN(this);
			*this >> data_chunk_size;
		}
	}

	bool Wav_File::read_data(u8 *buffer, u32 size) {
		if(in) {
			in.read((char *)buffer, size);
			if(in.eof() || in.fail()) return false;
			return true;
		}

		return false;
	}

	void Wav_File::read_all(const std::string &file_name) {
		read_meta(file_name);
		data = new u8[data_chunk_size];
		in.read((char *)data, data_chunk_size);
	}

	void Wav_File::write_meta(const std::string &file_name) {
		out.open(file_name, std::ios_base::binary | std::ios_base::app);

		if(out) {
			out.write((char *)&chunk_id,		sizeof(chunk_id));
			out.write((char *)&chunk_size,		sizeof(chunk_id));
			out.write((char *)&format,			sizeof(format));
			out.write((char *)&fmt_chunk_id,	sizeof(fmt_chunk_id));
			out.write((char *)&fmt_chunk_size,	sizeof(fmt_chunk_size));
			out.write((char *)&audio_format,	sizeof(audio_format));
			out.write((char *)&num_channels,	sizeof(num_channels));
			out.write((char *)&sample_rate,		sizeof(sample_rate));
			out.write((char *)&byte_rate,		sizeof(byte_rate));
			out.write((char *)&block_align,		sizeof(block_align));
			out.write((char *)&bits_per_sample,	sizeof(bits_per_sample));
			out.write((char *)&data_chunk_id,	sizeof(data_chunk_id));
			out.write((char *)&data_chunk_size,	sizeof(data_chunk_size));
		}
	}
	
	bool Wav_File::write_data(u8 *buffer, u32 size) {
		if(out) {
			out.write((char *)buffer, size);
			if(out.eof() || out.fail()) return false;
			return true;
		}

		return false;
	}

	void Wav_File::write_all(const std::string &file_name) {
		write_meta(file_name);
		out.write((char *)data, data_chunk_size);
	}
	
	void Wav_File::invalidate() {
		chunk_id = 0;
	}
	
	bool Wav_File::valid() {
		return chunk_id != 0;
	}

	Wav_File::~Wav_File() {
		delete[] data;
	}

	void Wav_File::print_meta() {
		std::cout << std::hex;
		std::cout << "chunk_id          =   0x" << chunk_id			<< std::endl;
		std::cout << "chunk_size        =   0x" << chunk_size		<< std::endl;
		std::cout << "format            =   0x" << format			<< std::endl;
		std::cout << "fmt_chunk_id      =   0x" << fmt_chunk_id		<< std::endl;
		std::cout << "fmt_chunk_size    =   0x" << fmt_chunk_size	<< std::endl;
		std::cout << std::dec;
		std::cout << "audio_format      =   "   << audio_format		<< std::endl;
		std::cout << "num_channels      =   "   << num_channels		<< std::endl;
		std::cout << "sample_rate       =   "   << sample_rate		<< std::endl;
		std::cout << "byte_rate         =   "   << byte_rate		<< std::endl;
		std::cout << "block_align       =   "   << block_align		<< std::endl;
		std::cout << "bits_per_sample   =   "   << bits_per_sample	<< std::endl;
		std::cout << std::hex;
		std::cout << "data_chunk_id     =   0x" << data_chunk_id	<< std::endl;
		std::cout << "data_chunk_size   =   0x" << data_chunk_size	<< std::endl;
		std::cout << std::dec;
	}

	void reverse(u8 *s, int n) {
		u8 temp = 0;
		for(int i = 0; i < n/2; ++i) {
			temp = s[i];
			s[i] = s[n - i - 1];
			s[n - i - 1] = temp;
		}
	}
}
