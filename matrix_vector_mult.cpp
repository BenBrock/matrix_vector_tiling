#include <cstdlib>
#include <cstdio>
#include <vector>
#include <chrono>

#include "util.hpp"

std::vector<float> simple_mvm(const std::vector<float>& a,
                              const std::vector<float>& b,
                              size_t m, size_t n)
{
  std::vector<float> result(n, 0.0);

  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      result[j] += a[i*n + j] * b[j];
    }
  }
  return result;
}

std::vector<float> blocked_mvm(const std::vector<float>& a,
                               const std::vector<float>& b,
                               size_t m, size_t n)
{
  std::vector<float> result(n, 0.0);

  size_t a_bs = A_BLOCKSIZE;
  size_t b_bs = B_BLOCKSIZE;

  for (size_t i = 0; i < m; i += a_bs) {
    for (size_t j = 0; j < n; j += b_bs) {
      for (size_t ii = i; ii < std::min(i+a_bs, m); ii++) {
        for (size_t jj = j; jj < std::min(j+b_bs, n); jj++) {
          result[jj] += a[ii*n + jj] * b[jj];
        }
      }
    }
  }
  return result;
}

int main(int argc, char** argv) {
  size_t m = 512*1024;
  size_t n = 1024;

  std::vector<float> a(m*n);
  std::vector<float> b(n);

  fill_matrix(a, m, n);
  fill_vector(b);

  printf("BLOCKSIZE %d,%d\n", A_BLOCKSIZE, B_BLOCKSIZE);

  auto begin = std::chrono::high_resolution_clock::now();
  std::vector<float> c = simple_mvm(a, b, m, n);
  auto end = std::chrono::high_resolution_clock::now();
  double duration = std::chrono::duration<double>(end - begin).count();

  printf("Simple MVM Runtime: %lf seconds.\n", duration);

  fill_matrix(a, m, n);
  fill_vector(b);

  begin = std::chrono::high_resolution_clock::now();
  std::vector<float> blocked_c = blocked_mvm(a, b, m, n);
  end = std::chrono::high_resolution_clock::now();
  double blocked_duration = std::chrono::duration<double>(end - begin).count();

  bool correct = check_precision(c, blocked_c);
  if (correct) {
    printf("Matrices match.\n");
  } else {
    printf("Matrices incorrect.\n");
  }

  printf("Blocked MVM Runtime: %lf seconds. (%s)\n", blocked_duration,
         (blocked_duration < duration) ? "faster" : "slower");

  return 0;
}
