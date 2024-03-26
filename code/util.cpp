#include "util.hpp"

#include <fstream>

namespace sm {
	Wav_File read_wav_file(const std::string &file_name) {
		std::ifstream input_stream;
		input_stream.open(file_name);

		Wav_File wf = {0};

		if(input_stream.is_open()) {
			input_stream
				>> wf.chunk_id
				>> wf.chunk_size
				>> wf.format;
		}
		else {
			wf.chunk_id = -1;
		}

		return wf;
	}
}
