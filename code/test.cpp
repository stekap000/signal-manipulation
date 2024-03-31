#ifndef SM_TEST_CPP
#define SM_TEST_CPP

namespace sm {
	u8 *double_frequency(u8 *data, int size) {
		u8 *new_data = new u8[size / 2];
		for(int i = 0; i < size / 4; ++i)
			*((u16 *)new_data + i) = *((u16 *)data + i * 2);
		return new_data;
	}

	void amp_scale(u8 *data, int size, float scale) {
		for(int i = 0; i < size; ++i)
			data[i] *= scale;
	}
}

#endif
