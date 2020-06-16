#include <fstream>
#include <KTKLDecoder.h>
#include "Utility.h"
#include "Simulation.h"
#include "catch.hpp"
const double EPSILON = 0.0000001;

TEST_CASE("KTKL: Can decode zero word") {
  std::ifstream filename("../tests/test_matrix");
  std::string name;
  std::getline(filename, name);
  unsigned int n, k;
  filename >> n >> k;
  int **g;
  g = readMatrix(filename, n, k);
  int **h = readMatrix(filename, n, n-k);
  KTKLDecoder dec(n, k, g, h, 1, 3);
  double *x = new double[n];
  int *u = new int[n];
  x[0] = -1.5;
  x[1] = -0.5;
  x[2] = -1.7;
  x[3] = -1.9;
  x[4] = -2.0;
  x[5] = -1.0;

  dec.decode(x, u);

  for (unsigned int i = 0; i < n; ++i) {
    REQUIRE(u[i] == 0);
  }

  delete[] x;
  delete[] u;
  for (unsigned int i = 0; i < k; ++i) {
    delete[] g[i];
  }
  delete[] g;
  for (unsigned int i = 0; i < n-k; ++i) {
    delete[] h[i];
  }
  delete[] h;
}

TEST_CASE("KTKL: Can decode non-zero word") {
  // notice - with this data it should fail
  std::ifstream filename("../tests/test_matrix");
  std::string name;
  std::getline(filename, name);
  int **g;
  unsigned int n, k;
  filename >> n >> k;
  g = readMatrix(filename, n, k);
  int **h = readMatrix(filename, n, n-k);
  KTKLDecoder dec(n, k, g, h, 1, 3);
  double *x = new double[n];
  int *y = new int[n];
  int *u = new int[n];
  u[0] = 0;
  u[1] = 1;
  u[2] = 1;
  u[3] = 0;
  u[4] = 0;
  u[5] = 1;
  x[0] = 0.503536;
  x[1] = 0.423184;
  x[2] = 1.87715;
  x[3] = -0.107348;
  x[4] = -1.7087;
  x[5] = -1.10187;
  // True weight: 1.6053

  std::string codeword = "000000";

  dec.decode(x, y);

  for (unsigned int j = 0; j < n; ++j) {
    codeword[j] = y[j] ? '1' : '0';
  }
  INFO("Decoded word: " << codeword);
  for (unsigned int i = 0; i < n; ++i) {
    CHECK(y[i] == u[i]);
  }

  for (unsigned int i = 0; i < k; ++i) {
    delete[] g[i];
  }
  delete[] g;
  for (unsigned int i = 0; i < n-k; ++i) {
    delete[] h[i];
  }
  delete[] h;
  delete[] x;
  delete[] u;
  delete[] y;
}

void inline test_KTKL_decoder(int tests, double stn, int w, int buf_size, const std::string &filename) {
  std::ifstream in(filename);
  Simulation sim(in, 0, tests);
  sim.add_ktkl_decoder(w, buf_size);
  sim.setSTN(stn);
  sim.test_run();
}

TEST_CASE("KTKL: Can decode series of random words with minimal noise") {
  test_KTKL_decoder(1000, 100, 1, 3, "../tests/test_matrix");
}

TEST_CASE("KTKL: Can decode series of random words with some noise") {
  test_KTKL_decoder(1000, 1, 1, 3, "../tests/test_matrix");
}

TEST_CASE("KTKL: Can decode BCH(7, 4, 3)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-7-4");
}

TEST_CASE("KTKL: Can decode BCH(7, 1, 5)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-7-1");
}

TEST_CASE("KTKL: Can decode EBCH(8, 4, 4)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-8-4");
}

TEST_CASE("KTKL: Can decode BCH(31, 16, 7)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-31-16-7");
}

TEST_CASE("KTKL: Can decode BCH(31, 21, 5)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-31-21-5");
}

TEST_CASE("KTKL: Can decode BCH(63, 7, 57)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-63-7-57");
}

TEST_CASE("KTKL: Can decode BCH(63, 16, 23)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-63-16-23");
}

/*TEST_CASE("KTKL: Can decode BCH(63, 30, 13)")
{
    test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-63-30-13");
}

TEST_CASE("KTKL: Can decode BCH(63, 39, 9)")
{
    test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-63-39-9");
}*/

TEST_CASE("KTKL: Can decode RM(16, 5)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/reed-muller-16-1");
}

TEST_CASE("KTKL: Can decode RM(32, 6)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/reed-muller-32-1");
}

TEST_CASE("KTKL: Can decode EBCH(32, 16)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-32-16");
}

TEST_CASE("KTKL: Can decode EBCH(32, 21)") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-32-21");
}

TEST_CASE("KTKL: Can decode EBCH(32, 16) in SBO") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-32-16-bit-order");
}

TEST_CASE("KTKL: Can decode EBCH(32, 21) in SBO") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-32-21-bit-order");
}

TEST_CASE("KTKL: Can decode EBCH(64, 57) in SBO") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-64-57-bit-order");
}

TEST_CASE("KTKL: Can decode EBCH(64, 51) in SBO") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-64-51-bit-order");
}

TEST_CASE("KTKL: Can decode EBCH(64, 45) in SBO") {
  test_KTKL_decoder(1000, 1, 1, 3, "../data/bch-64-45-bit-order");
}
