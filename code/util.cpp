#include "util.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>

#define WAV_READ_ERROR_RETURN(wf) do {(wf)->invalidate(); return;} while(0)

namespace sm {
	Wav_File &Wav_File::operator>>(u16 &data) {
		this->in.read((char *)&data, sizeof(data));
		return *this;
	}

	Wav_File &Wav_File::operator>>(u32 &data) {
		this->in.read((char *)&data, sizeof(data));
		return *this;
	}
	
	void Wav_File::read_meta(const std::string &file_name) {
		in.open(file_name);

		printf("FILE:\n");
		if(in) {
			*this >> this->chunk_id;
			printf("chunk_id: %x\n", this->chunk_id);
			// RIFF (big endian)
			if(this->chunk_id != 0x46464952) WAV_READ_ERROR_RETURN(this);

			*this >> this->chunk_size;
			printf("chunk_size: %x\n", this->chunk_size);

			*this >> this->format;
			printf("format: %x\n", this->format);
			// WAVE (big endian)
			if(this->format != 0x45564157) WAV_READ_ERROR_RETURN(this);
			
			*this >> this->fmt_chunk_id;
			printf("fmt_chunk_id: %x\n", this->fmt_chunk_id);
			if(this->fmt_chunk_id != 0x20746d66) WAV_READ_ERROR_RETURN(this);

			*this >> this->fmt_chunk_size;
			printf("fmt_chunk_size: %x\n", this->fmt_chunk_size);
			*this >> this->audio_format;
			printf("audio_format: %d\n", this->audio_format);
			// If format is PCM then fmt chunk size must be 16?
			if(this->audio_format == 1 && this->fmt_chunk_size != 16) WAV_READ_ERROR_RETURN(this);
			*this >> this->num_channels;
			printf("num_channels: %d\n", this->num_channels);

			*this >> this->sample_rate;
			printf("sample_rate: %d\n", this->sample_rate);

			*this >> this->byte_rate;
			printf("byte_rate: %d\n", this->byte_rate);
			*this >> this->block_align;
			printf("block_align: %d\n", this->block_align);
			*this >> this->bits_per_sample;
			printf("bits_per_sample: %d\n", this->bits_per_sample);
			if(this->byte_rate != this->sample_rate * this->num_channels * this->bits_per_sample / 8) WAV_READ_ERROR_RETURN(this);
			if(this->block_align != this->num_channels * this->bits_per_sample / 8) WAV_READ_ERROR_RETURN(this);
			
			*this >> this->data_chunk_id;
			printf("data_chunk_id: %x\n", this->data_chunk_id);
			if(this->data_chunk_id != 0x61746164) WAV_READ_ERROR_RETURN(this);
			
			*this >> this->data_chunk_size;
			printf("data_chunk_size: %x\n", this->data_chunk_size);
		}
	}

	bool Wav_File::read_data(u8 *buffer, u32 size) {
		if(in) {
			in.read((char *)buffer, size);
			in.flush();
			if(in.eof()) printf("EOF ERROR\n");
			if(in.fail()) printf("FAIL ERROR\n");
			if(in.bad()) printf("BAD ERROR\n");
			if(in.eof() || in.fail() || in.bad()) return false;
			return true;
		}

		return false;
	}

	void Wav_File::write_meta(const std::string &file_name) {
		out.open(file_name, std::ios_base::app);

		out.write((char *)&this->chunk_id,			sizeof(this->chunk_id));
		out.write((char *)&this->chunk_size,		sizeof(this->chunk_id));
		out.write((char *)&this->format,			sizeof(this->format));
		out.write((char *)&this->fmt_chunk_id,		sizeof(this->fmt_chunk_id));
		out.write((char *)&this->fmt_chunk_size,	sizeof(this->fmt_chunk_size));
		out.write((char *)&this->audio_format,		sizeof(this->audio_format));
		out.write((char *)&this->num_channels,		sizeof(this->num_channels));
		out.write((char *)&this->sample_rate,		sizeof(this->sample_rate));
		out.write((char *)&this->byte_rate,			sizeof(this->byte_rate));
		out.write((char *)&this->block_align,		sizeof(this->block_align));
		out.write((char *)&this->bits_per_sample,	sizeof(this->bits_per_sample));
		out.write((char *)&this->data_chunk_id,		sizeof(this->data_chunk_id));
		out.write((char *)&this->data_chunk_size,	sizeof(this->data_chunk_size));
	}
	
	bool Wav_File::write_data(u8 *buffer, u32 size) {
		if(out) {
			out.write((char *)buffer, size);
			return true;
		}

		return false;
	}
	
	void Wav_File::invalidate() {
		this->chunk_id = 0;
	}
	
	bool Wav_File::valid() {
		return this->chunk_id != 0;
	}

	Wav_File::~Wav_File() {
		delete[] this->data;
	}
}
