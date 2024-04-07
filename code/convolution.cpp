#include "convolution.hpp"

#include <cstdio>

namespace sm {
	u8 *convolve_slow(u8 *s1, int n1, u8 *s2, int n2) {
		int n = n1 + n2 - 1;
		u8 *new_data = new u8[n];

		printf("Size: %d ", n);

		reverse(s2, n2);
		for(int i = 0; i < n; ++i) {
			if(i % 100000 == 0) printf("%d ", i);
			int j = 0;
			int k = 0;
		
			if(i < n2) {
				k = n2 - i - 1;
			}
			else {
				j = i - n2;
			}

			for(; j < n1 && k < n2; ++j, ++k) {
				new_data[i] += s1[j] * s2[k];
			}
		}

		return new_data;
	}
}
