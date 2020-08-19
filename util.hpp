#include <vector>

void fill_matrix(std::vector<float>& mat, size_t m, size_t n) {
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      // In row-major order, element i,j
      // is located at index i*ncols + j
      mat[i*n + j] = (i*n + j) / ((float) m*n);
    }
  }
}

void fill_vector(std::vector<float>& vec) {
  for (size_t i = 0; i < vec.size(); i++) {
    vec[i] = i / (float) vec.size();
  }
}

bool check_precision(const std::vector<float>& a, const std::vector<float>& b) {
  if (a.size() != b.size()) {
    return false;
  }

  float eps = 1.0e-4;

  for (size_t i = 0; i < a.size(); i++) {
    if (std::abs(a[i] - b[i]) > eps) {
      return false;
    }
  }
  return true;
}
