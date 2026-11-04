#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ============================================================
// Matrix class: square N x N matrix of ints.
// Data is private; everything goes through public member functions.
// ============================================================
class Matrix {
private:
    int n;                          // matrix is n x n
    vector<vector<int> > data;      // hidden storage

public:
    Matrix(int size = 0) : n(size), data(size, vector<int>(size, 0)) {}

    // Practice 1: read N and two matrices from a file into A and B
    static bool loadFromFile(const string& filename, Matrix& A, Matrix& B) {
        ifstream in(filename.c_str());
        if (!in) {
            cerr << "Error: could not open " << filename << "\n";
            return false;
        }

        int size;
        if (!(in >> size) || size <= 0) {
            cerr << "Error: invalid N\n";
            return false;
        }

        A = Matrix(size);
        B = Matrix(size);

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                int x;
                if (!(in >> x)) { cerr << "Error: not enough numbers\n"; return false; }
                A.set(i, j, x);
            }

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                int x;
                if (!(in >> x)) { cerr << "Error: not enough numbers\n"; return false; }
                B.set(i, j, x);
            }

        return true;
    }

    // Accessors
    int  size() const { return n; }
    int  get(int r, int c) const { return data[r][c]; }
    void set(int r, int c, int value) { data[r][c] = value; }
    bool validIndex(int i) const { return i >= 0 && i < n; }

    // Practice 2: A + B
    Matrix operator+(const Matrix& other) const {
        Matrix result(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    // Practice 3: A * B
    Matrix operator*(const Matrix& other) const {
        Matrix result(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                int sum = 0;
                for (int k = 0; k < n; k++)
                    sum += data[i][k] * other.data[k][j];
                result.data[i][j] = sum;
            }
        return result;
    }

    // Practice 4: diagonal sums
    void diagonalSums(int& mainSum, int& secSum) const {
        mainSum = 0;
        secSum = 0;
        for (int i = 0; i < n; i++) {
            mainSum += data[i][i];
            secSum  += data[i][n - 1 - i];
        }
    }

    // Print with aligned columns (width = longest number + 2)
    void print(ostream& out) const {
        size_t longest = 1;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                longest = max(longest, to_string(data[i][j]).size());
        int w = static_cast<int>(longest) + 2;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                out << setw(w) << data[i][j];
            out << "\n";
        }
    }
};

// cout << matrix
ostream& operator<<(ostream& out, const Matrix& m) {
    m.print(out);
    return out;
}

// ============================================================
// Practices 5-7: Matrix passed BY VALUE, so the caller's original is untouched.
// On a bad index: print an error and return the copy unchanged.
// ============================================================
Matrix swapRows(Matrix m, int r1 = 0, int r2 = 1) {
    if (!m.validIndex(r1) || !m.validIndex(r2)) {
        cout << "Error: row index out of range\n";
        return m;
    }
    for (int j = 0; j < m.size(); j++) {
        int tmp = m.get(r1, j);
        m.set(r1, j, m.get(r2, j));
        m.set(r2, j, tmp);
    }
    return m;
}

Matrix swapCols(Matrix m, int c1 = 0, int c2 = 1) {
    if (!m.validIndex(c1) || !m.validIndex(c2)) {
        cout << "Error: column index out of range\n";
        return m;
    }
    for (int i = 0; i < m.size(); i++) {
        int tmp = m.get(i, c1);
        m.set(i, c1, m.get(i, c2));
        m.set(i, c2, tmp);
    }
    return m;
}

Matrix updateElement(Matrix m, int r = 0, int c = 0, int value = 100) {
    if (!m.validIndex(r) || !m.validIndex(c)) {
        cout << "Error: index out of range\n";
        return m;
    }
    m.set(r, c, value);
    return m;
}

// ============================================================
int main() {
    string filename;
    cout << "Enter input file name: ";
    cin >> filename;

    Matrix A, B;
    if (!Matrix::loadFromFile(filename, A, B)) {
        return 1;
    }

    // Practice 1
    cout << "\nMatrix A:\n" << A;
    cout << "\nMatrix B:\n" << B;

    // Practice 2
    cout << "\nA + B:\n" << A + B;

    // Practice 3
    cout << "\nA * B:\n" << A * B;

    // Practice 4
    int mainSum, secSum;
    A.diagonalSums(mainSum, secSum);
    cout << "\nMain diagonal sum of A: " << mainSum << "\n";
    cout << "Secondary diagonal sum of A: " << secSum << "\n";

    // Practice 5 (defaults 0,1 then explicit 0,2)
    cout << "\nA after swapping rows 0 and 1 (defaults):\n" << swapRows(A);
    cout << "\nA after swapping rows 0 and 2:\n" << swapRows(A, 0, 2);

    // Practice 6
    cout << "\nA after swapping columns 0 and 2:\n" << swapCols(A, 0, 2);

    // Practice 7 (defaults 0,0,100 then explicit)
    cout << "\nA after updating (0,0) to 100 (defaults):\n" << updateElement(A);
    cout << "\nA after updating (1,2) to 99:\n" << updateElement(A, 1, 2, 99);

    // Bad indices: should print an error and change nothing
    cout << "\nBad index tests:\n";
    swapRows(A, -1, 2);
    swapCols(A, 0, 4);
    updateElement(A, 4, 4, 99);

    // Proof the original was never modified (pass-by-value)
    cout << "\nOriginal A is unchanged:\n" << A;

    return 0;
}