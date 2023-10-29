int** MakeSpiral(int n) {
  int** spiral_matrix = new int*[n];
  for (int i = 0; i < n; ++i) {
    spiral_matrix[i] = new int[n];
  }
  int elem = 1;
  for (int i = 0; i <= n / 2; ++i) {
    for (int j = i; j < n - i; ++j) {
      spiral_matrix[i][j] = elem;
      ++elem;
    }
    for (int j = i + 1; j < n - i; ++j) {
      spiral_matrix[j][n - i - 1] = elem;
      ++elem;
    }
    for (int j = n - i - 2; j >= i; --j) {
      spiral_matrix[n - i - 1][j] = elem;
      ++elem;
    }
    for (int j = n - i - 2; j > i; --j) {
      spiral_matrix[j][i] = elem;
      ++elem;
    }
  }
  return spiral_matrix;
}
