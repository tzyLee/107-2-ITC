#include <algorithm>
#include <fstream>
#include <memory>

inline void arr_cpy(unsigned dim, double* const src, double* dest) {
    // Copy half*half from src to dest
    unsigned half = (dim + 1) / 2;
    for (unsigned i = 0; i < half; ++i)
        std::copy(src + i * dim, src + i * dim + half, dest + i * half); // copy rows from src to dest
}

inline void arr_cpy(unsigned dim, bool skipLastCol, bool skipLastRow, double* const src, double* dest) {
    // Copy rectangular area half*half from src[dim*dim] to dest[half*half]
    unsigned half = (dim + 1) / 2;
    unsigned last = skipLastCol ? half-1 : half;
    for(unsigned i=0, end=skipLastRow ? half-1 : half; i<end; ++i) {
        std::copy(src + i * dim, src + i * dim + last, dest + i * half);
        if(skipLastCol)
            dest[i*half + last] = 0;
    }
    if(skipLastRow)
        std::fill(dest + (half - 1) * half, dest + half * half, 0); // set row after h-th to zero
}

inline void arr_diff(unsigned dim, double* const a, double* const b, double* out) {
    std::transform(a, a+dim*dim, b, out, [](double a, double b){ return a-b; });
}

inline void arr_add(unsigned dim, double* const a, double* const b, double* out) {
    std::transform(a, a+dim*dim, b, out, [](double a, double b){ return a+b; });
}

void strassen(unsigned dim, double* const a, double* const b, double* out)
{
    if (dim == 2) {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                out[i * dim + j] = a[i * dim] * b[j] + a[i * dim + 1] * b[dim + j];
    }
    else {
        const unsigned half = (dim + 1) / 2;
        const unsigned len = dim%2 ? half-1 : half;
        double *A[4] = {nullptr}; // A == [A0 A1; A2 A3]
        double *B[4] = {nullptr}; // B == [B0 B1; B2 B3]
        double *M[7] = {nullptr};
        double *first = new double[half * half], *second = new double[half * half];
        for (int i = 0; i < 4; ++i) {
            A[i] = new double[half * half];
            B[i] = new double[half * half];
            M[i] = new double[half * half];
        }
        for (int i = 4; i < 7; ++i)
            M[i] = new double[half * half];
        const bool skip = dim%2;
        arr_cpy(dim, a, A[0]);
        arr_cpy(dim, skip, false, a + half, A[1]);
        arr_cpy(dim, false, skip, a + half*dim, A[2]);
        arr_cpy(dim, skip, skip, a + half*dim + half, A[3]);
        arr_cpy(dim, b, B[0]);
        arr_cpy(dim, skip, false, b + half, B[1]);
        arr_cpy(dim, false, skip, b + half*dim, B[2]);
        arr_cpy(dim, skip, skip, b + half*dim + half, B[3]);

        arr_add(half, A[0], A[3], first); // first = A11 + A22
        arr_add(half, B[0], B[3], second); // second = B11 + B22
        strassen(half, first, second, M[0]); // M[0] = first * second

        arr_add(half, A[2], A[3], first); // first = A21 + A22
        strassen(half, first, B[0], M[1]); // M[1] = first * B11

        arr_diff(half, B[1], B[3], second); // second = B12 - B22
        strassen(half, A[0], second, M[2]); // M[2] = A11 * second

        arr_diff(half, B[2], B[0], second); // second = B21 - B11
        strassen(half, A[3], second, M[3]); // M[3] = A22 * second

        arr_add(half, A[0], A[1], first); // first = A11 + A12
        strassen(half, first, B[3], M[4]); // M[4] = first * B22

        arr_diff(half, A[2], A[0], first); // first = A21 - A11
        arr_add(half, B[0], B[1], second);// second = B11 + B12
        strassen(half, first, second, M[5]);// M[5] = first * second

        arr_diff(half, A[1], A[3], first); // first = A12 - A22
        arr_add(half, B[2], B[3], second); // second = B21 + B22
        strassen(half, first, second, M[6]); // M[6] = first * second

        arr_add(half, M[0], M[3], first); // first = P1 + P4
        arr_diff(half, first, M[4], first); // first -= P5
        arr_add(half, first, M[6], first); // first += P7
        for(unsigned i=0; i<half; ++i) // Copy C11 to out
            std::copy(first + i*half, first + i*half + half, out + i*dim);

        arr_add(half, M[2], M[4], first); // first = P3 + P5
        for(unsigned i=0; i<half; ++i) // Copy C12 to out
            std::copy(first + i*half, first + i*half + len, out + i*dim + half);

        arr_add(half, M[1], M[3], first); // first = P2 + P4
        for(unsigned i=0; i<len; ++i) // Copy C21 to out
            std::copy(first + i*half, first + i*half + half, out + (i + half)*dim);

        arr_add(half, M[0], M[2], first); // first = P1 + P3
        arr_diff(half, first, M[1], first); // first -= P2
        arr_add(half, first, M[5], first); // first += P6
        for(unsigned i=0; i<len; ++i) // Copy C22 to out
            std::copy(first + i*half, first + i*half + len, out + (i+half)*dim + half);

        for (int i = 0; i < 4; ++i) {
            delete[] A[i];
            delete[] B[i];
            delete[] M[i];
        }
        for (int i = 4; i < 7; ++i)
            delete[] M[i];
        delete[] first;
        delete[] second;
    }
}

int main() {
    std::ifstream ifs("matrix_in.txt");
    unsigned dim = 0;
    ifs >> dim;
    const std::size_t size = dim*dim;
    std::unique_ptr<double[]> A(new double[size]), B(new double[size]), C(new double[size]);
    for(std::size_t i=0; i<size; ++i)
        ifs >> A[i];
    for(std::size_t i=0; i<size; ++i)
        ifs >> B[i];
    strassen(dim, A.get(), B.get(), C.get());

    std::ofstream ofs("matrix_out.txt");
    for(unsigned i=0; i<dim; ++i) {
        ofs << C[i*dim];
        for(unsigned j=1; j<dim; ++j)
            ofs << ' ' << C[i*dim + j];
        ofs.put('\n');
    }
    return 0;
}