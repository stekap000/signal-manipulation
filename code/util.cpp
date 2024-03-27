#include "util.hpp"

#include <iostream>
#include <cstdio>

#define WAV_READ_ERROR_RETURN(wf) do {(wf).invalidate(); return wf;} while(0)

namespace sm {
	void Wav_File::invalidate() {
		this->chunk_id = 0;
	}
	
	bool Wav_File::valid() {
		return this->chunk_id != 0;
	}

	Wav_File::~Wav_File() {
		delete[] this->data;
	}
	
	Wav_Reader::Wav_Reader(const std::string &file_name) {
		this->in.open(file_name);
	}

	Wav_Reader::~Wav_Reader() { in.close(); }

	Wav_Reader &Wav_Reader::operator>>(u16 &data) {
		this->in.read((char *)&data, sizeof(data));
		return *this;
	}

	Wav_Reader &Wav_Reader::operator>>(u32 &data) {
		this->in.read((char *)&data, sizeof(data));
		return *this;
	}
	
	Wav_File &Wav_Reader::read(Wav_File &wf) {
		printf("FILE:\n");
		
		if(in) {
			*this >> wf.chunk_id;
			printf("chunk_id: %x\n", wf.chunk_id);
			// RIFF (big endian)
			if(wf.chunk_id != 0x46464952) WAV_READ_ERROR_RETURN(wf);

			*this >> wf.chunk_size;
			printf("chunk_size: %x\n", wf.chunk_size);

			*this >> wf.format;
			printf("format: %x\n", wf.format);
			// WAVE (big endian)
			if(wf.format != 0x45564157) WAV_READ_ERROR_RETURN(wf);
			
			*this >> wf.fmt_chunk_id;
			printf("fmt_chunk_id: %x\n", wf.fmt_chunk_id);
			if(wf.fmt_chunk_id != 0x20746d66) WAV_READ_ERROR_RETURN(wf);

			*this >> wf.fmt_chunk_size;
			printf("fmt_chunk_size: %x\n", wf.fmt_chunk_size);
			*this >> wf.audio_format;
			printf("audio_format: %d\n", wf.audio_format);
			// If format is PCM then fmt chunk size must be 16?
			if(wf.audio_format == 1 && wf.fmt_chunk_size != 16) WAV_READ_ERROR_RETURN(wf);
			*this >> wf.num_channels;
			printf("num_channels: %d\n", wf.num_channels);

			*this >> wf.sample_rate;
			printf("sample_rate: %d\n", wf.sample_rate);

			*this >> wf.byte_rate;
			printf("byte_rate: %d\n", wf.byte_rate);
			*this >> wf.block_align;
			printf("block_align: %d\n", wf.block_align);
			*this >> wf.bits_per_sample;
			printf("bits_per_sample: %d\n", wf.bits_per_sample);
			if(wf.byte_rate != wf.sample_rate * wf.num_channels * wf.bits_per_sample / 8) WAV_READ_ERROR_RETURN(wf);
			if(wf.block_align != wf.num_channels * wf.bits_per_sample / 8) WAV_READ_ERROR_RETURN(wf);
			
			*this >> wf.data_chunk_id;
			printf("data_chunk_id: %x\n", wf.data_chunk_id);
			if(wf.data_chunk_id != 0x61746164) WAV_READ_ERROR_RETURN(wf);
			
			*this >> wf.data_chunk_size;
			printf("data_chunk_size: %x\n", wf.data_chunk_size);

			wf.data = new u8[wf.data_chunk_size];
			this->in.read((char *)wf.data, sizeof(wf.data_chunk_size));
		}

		return wf;
	}
}
