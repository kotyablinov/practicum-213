// Абстрактный класс: умножение двух матриц, умножение матрицы на число.

#include <iostream>
using namespace std;

template <typename T>
class Matrix {
   private:
    int rows;
    int columns;
    T **matrix;
    static int numberOfMatrixes;

   public:
    Matrix(int inputRows, int inputColumns) {
        numberOfMatrixes++;
        rows = inputRows;
        columns = inputColumns;
        matrix = (T **)new T *[rows];

        for (int currentRow = 0; currentRow < rows; currentRow++)
            matrix[currentRow] = (T *)new T[columns];
        for (int currentRow = 0; currentRow < rows; currentRow++)
            for (int currentColumn = 0; currentColumn < columns; currentColumn++)
                matrix[currentRow][currentColumn] = 0;
    }

   private:
    void set(const Matrix<T> &sourceMatrix) {
        rows = sourceMatrix.rows;
        columns = sourceMatrix.columns;
        matrix = (T **)new T *[rows];

        for (int currentRow = 0; currentRow < rows; currentRow++)
            matrix[currentRow] = (T *)new T[columns];
        for (int currentRow = 0; currentRow < rows; currentRow++)
            for (int currentColumn = 0; currentColumn < columns; currentColumn++)
                matrix[currentRow][currentColumn] = sourceMatrix.matrix[currentRow][currentColumn];
    }

   public:
    Matrix(const Matrix<T> &sourceMatrix) {
        numberOfMatrixes++;
        set(sourceMatrix);
    }

   private:
    void clean() {
        for (int currentRow = 0; currentRow < rows; currentRow++)
            delete[] matrix[currentRow];
        delete[] matrix;
    }

   public:
    ~Matrix() {
        numberOfMatrixes--;
        clean();
    }

    int getColumnsNumber() {
        return columns;
    }

    int getRowsNumber() {
        return rows;
    }

    void fill(int currentRow, int currentColumn, T value) {
        matrix[currentRow][currentColumn] = value;
    }

    T &operator()(int currentRow, int currentColumn) {
        return matrix[currentRow][currentColumn];
    }

    Matrix<T> &operator=(const Matrix<T> &copyMatrix) {
        clean();
        set(copyMatrix);
        return *this;
    }

    friend ostream &operator<<(ostream &currentStream, const Matrix<T> &currentMatrix) {
        for (int currentRow = 0; currentRow < currentMatrix.rows; currentRow++) {
            for (int currentColumn = 0; currentColumn < currentMatrix.columns; currentColumn++)
                currentStream << currentMatrix.matrix[currentRow][currentColumn] << ' ';
            currentStream << endl;
        }
        return currentStream;
    }

    static int getNumberOfMatrixes() {
        return numberOfMatrixes;
    }

    Matrix<T> &operator*(const int value) {
        Matrix<T> *newMatrix = new Matrix(*this);
        for (int currentRow = 0; currentRow < rows; currentRow++)
            for (int currentColumn = 0; currentColumn < columns; currentColumn++)
                newMatrix->fill(currentRow, currentColumn, (*newMatrix)(currentRow, currentColumn) * value);
        return *newMatrix;
    }

    Matrix<T> &operator*(const Matrix<T> anotherMatrix) {
        Matrix<T> *newMatrix = new Matrix(rows, anotherMatrix.columns);

        for (int currentRow = 0; currentRow < rows; currentRow++) {
            for (int anotherColumn = 0; anotherColumn < anotherMatrix.columns; anotherColumn++) {
                for (int currentColumn = 0; currentColumn < columns; currentColumn++)
                    newMatrix->matrix[currentRow][anotherColumn] += matrix[currentRow][currentColumn] * anotherMatrix.matrix[currentColumn][anotherColumn];
            }
        }
        return *newMatrix;
    }
};

template <typename T>
int Matrix<T>::numberOfMatrixes = 0;

int main() {
    int rows = 2;
    int columns = 3;
    Matrix<int> matrix(rows, columns);
    cout << "Созданная матрица:";
    cout << "\n";
    cout << matrix;

    for (int currentRow = 0; currentRow < matrix.getRowsNumber(); currentRow++)
        for (int currentColumn = 0; currentColumn < matrix.getColumnsNumber(); currentColumn++)
            matrix.fill(currentRow, currentColumn, (currentRow + 1) * (currentColumn + 1));
    cout << "\n----\n\n"
         << "Заполненная матрица:"
         << "\n"
         << matrix;

    Matrix<int> newMatix1(matrix);

    cout << "\n----\n\n"
         << "Скопированная матрица:"
         << "\n"
         << newMatix1;

    Matrix<int> newMatix2(matrix);

    cout << "\n----\n\n"
         << "Присвоенная матрица:"
         << "\n"
         << newMatix2;

    cout << "\n----\n\n"
         << "Количество созданных матриц: "
         << Matrix<int>::getNumberOfMatrixes();

    newMatix2(0, 0) = 666;
    cout << "\n----\n\n"
         << "Результат вызова перегруженной операции`()`: "
         << "\n"
         << newMatix2;

    Matrix<int> newMatix3 = newMatix2 * 4;
    cout << "\n----\n\n"
         << "Умножение матрицы на число: "
         << "\n"
         << newMatix3;

    Matrix<int> matrixA(2, 2);
    Matrix<int> matrixB(2, 1);
    matrixA.fill(0, 0, 3);
    matrixA.fill(0, 1, -1);
    matrixA.fill(1, 0, 4);
    matrixA.fill(1, 1, 2);

    matrixB.fill(0, 0, 8);
    matrixB.fill(1, 0, 7);

    Matrix<int> matrixC = matrixA * matrixB;
    cout << "\n----\n\n"
         << "Умножение двух матриц: "
         << "\n"
         << matrixC;

    return 0;
}