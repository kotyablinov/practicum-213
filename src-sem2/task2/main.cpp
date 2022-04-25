// Абстрактный класс: умножение двух матриц, умножение матрицы на число.

#include <iostream>
using namespace std;

template <typename U>
class AbstractMatrix {
   protected:
    static int numberOfMatrixes;
    int rows;
    int columns;
    U **matrix;

    void set(const AbstractMatrix<U> &sourceMatrix) {
        this->rows = sourceMatrix.rows;
        this->columns = sourceMatrix.columns;
        this->matrix = (U **)new U *[this->rows];

        for (int currentRow = 0; currentRow < this->rows; currentRow++)
            this->matrix[currentRow] = (U *)new U[this->columns];
        for (int currentRow = 0; currentRow < this->rows; currentRow++)
            for (int currentColumn = 0; currentColumn < this->columns; currentColumn++)
                this->matrix[currentRow][currentColumn] = sourceMatrix.matrix[currentRow][currentColumn];
    }

    void clean() {
        for (int currentRow = 0; currentRow < this->rows; currentRow++)
            delete[] this->matrix[currentRow];
        delete[] this->matrix;
    }

   protected:
    AbstractMatrix() {
        numberOfMatrixes++;
    }

    AbstractMatrix(int inputRows, int inputColumns) : AbstractMatrix() {
        rows = inputRows;
        columns = inputColumns;
        matrix = (U **)new U *[rows];

        for (int currentRow = 0; currentRow < rows; currentRow++)
            matrix[currentRow] = (U *)new U[columns];
        for (int currentRow = 0; currentRow < rows; currentRow++)
            for (int currentColumn = 0; currentColumn < columns; currentColumn++)
                matrix[currentRow][currentColumn] = 0;
    }

    AbstractMatrix(const AbstractMatrix<U> &sourceMatrix) : AbstractMatrix() {
        set(sourceMatrix);
    }

   public:
    virtual ~AbstractMatrix() = default;
    virtual int getColumnsNumber() = 0;
    virtual int getRowsNumber() = 0;
    void fill(int currentRow, int currentColumn, U value) {
        this->matrix[currentRow][currentColumn] = value;
    }
    virtual U &operator()(int currentRow, int currentColumn) = 0;
    virtual AbstractMatrix<U> &operator=(const AbstractMatrix<U> &copyMatrix) = 0;

    friend ostream &operator<<(ostream &currentStream, const AbstractMatrix<U> &currentMatrix) {
        for (int currentRow = 0; currentRow < currentMatrix.rows; currentRow++) {
            for (int currentColumn = 0; currentColumn < currentMatrix.columns; currentColumn++)
                currentStream << currentMatrix.matrix[currentRow][currentColumn] << ' ';
            currentStream << endl;
        }
        return currentStream;
    }
};

template <typename T>
class Matrix : public AbstractMatrix<T> {
   public:
    Matrix() : AbstractMatrix<T>() {
    }
    Matrix(int inputRows, int inputColumns) : AbstractMatrix<T>(inputRows, inputColumns) {
    }
    Matrix(const Matrix<T> &sourceMatrix) : AbstractMatrix<T>(sourceMatrix) {
    }

   public:
    ~Matrix() override {
        this->numberOfMatrixes--;
        this->clean();
    }

    int getColumnsNumber() {
        return this->columns;
    }

    int getRowsNumber() {
        return this->rows;
    }

    T &operator()(int currentRow, int currentColumn) {
        return this->matrix[currentRow][currentColumn];
    }

    AbstractMatrix<T> &operator=(const AbstractMatrix<T> &copyMatrix) {
        this->clean();
        this->set(copyMatrix);
        return *this;
    }

    static int getNumberOfMatrixes() {
        return AbstractMatrix<T>::numberOfMatrixes;
    }

    Matrix<T> &operator*(const int value) {
        Matrix<T> *newMatrix = new Matrix(*this);
        for (int currentRow = 0; currentRow < this->rows; currentRow++)
            for (int currentColumn = 0; currentColumn < this->columns; currentColumn++)
                newMatrix->fill(currentRow, currentColumn, (*newMatrix)(currentRow, currentColumn) * value);
        return *newMatrix;
    }

    Matrix<T> &operator*(const Matrix<T> anotherMatrix) {
        Matrix<T> *newMatrix = new Matrix(this->rows, anotherMatrix.columns);

        for (int currentRow = 0; currentRow < this->rows; currentRow++) {
            for (int anotherColumn = 0; anotherColumn < anotherMatrix.columns; anotherColumn++) {
                for (int currentColumn = 0; currentColumn < this->columns; currentColumn++)
                    newMatrix->matrix[currentRow][anotherColumn] += this->matrix[currentRow][currentColumn] * anotherMatrix.matrix[currentColumn][anotherColumn];
            }
        }
        return *newMatrix;
    }
};

template <typename T>
int AbstractMatrix<T>::numberOfMatrixes = 0;

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