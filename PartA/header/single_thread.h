void singleThread(int N, int *matrix_A, int *matrix_B, int *matrix_output) {
	for (int j = 0; j < N; j += 2) {
		int col_1[N];
		int col_2[N];

		for (int k = 0; k < N; k++) {
			col_1[k] = matrix_B[(k*N) + j];
			col_2[k] = matrix_B[(k*N) + j+1];
		} 

		for (int i = 0; i < N; i += 2) {
			int sum = 0;

			for (int k = 0; k < N; k++) {
				sum += matrix_A[(i*N)+k] * col_1[k];
				sum += matrix_A[(i*N)+k] * col_2[k];
				sum += matrix_A[(i+1)*(N)+k] * col_1[k];
				sum += matrix_A[(i+1)*(N)+k] * col_2[k];
			}

			int r = i >> 1;
			int offset = j >> 1;
			
			matrix_output[(r*(N>>1)) + offset] = sum;
		}
	}
}
