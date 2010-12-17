/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#pragma once

#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace rsc {
namespace math {

/**
 * @author jwienke
 * @todo allow working with matrices of different types, e.g. double and float
 *       are possible
 */
template<unsigned int Rows, unsigned int Cols, class DataType = double>
class Matrix {
public:

    /**
     * An explicitly implicit conversion constructor for being able to use
     * matrices of different but comparable data types like float and double.
     *
     * @param otherMatrix matrix with comparable data type. Dimensions must
     *                    agree
     * @tparam OtherDataType data type of the other matrix
     * @note when converting from a floating point type to an integer type, no
     *       rounding is applied. Instead the fractional part is set to 0.
     */
    template<class OtherDataType>
    Matrix(const Matrix<Rows, Cols, OtherDataType> &otherMatrix) {
        fillFrom(otherMatrix);
    }

    /**
     * Creates an empty matrix with all elements being zero.
     */
    Matrix() {
        set(0);
    }

    virtual ~Matrix() {
    }

    /**
     * Sets all elements of this matrix to the specified value.
     *
     * @param value value to set
     * @return a reference to this matrix itself
     */
    Matrix<Rows, Cols, DataType> &set(const DataType &value) {
        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Cols; ++col) {
                data[row][col] = value;
            }
        }
        return *this;
    }

    /**
     * Fills this matrix with values from another matrix. Thereby a data type
     * conversion is possible.
     *
     * @param otherMatrix other matrix
     * @tparam OtherDataType data type of the other matrix
     * @note when converting from a floating point type to an integer type, no
     *       rounding is applied. Instead the fractional part is set to 0.
     */
    template<class OtherDataType>
    void fillFrom(const Matrix<Rows, Cols, OtherDataType> &otherMatrix) {
        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Cols; ++col) {
                data[row][col] = otherMatrix(row, col);
            }
        }
    }

    /**
     * Access operator that returns a mutable reference to the matrix element.
     * Hence, it is intended to assign new values.
     *
     * @param row row to access < Rows
     * @param col column to access < Cols
     * @return reference to the matrix element
     */
    DataType &operator()(const unsigned int &row, const unsigned int &col) {
        // using this const cast wisely prevents copying the code in safeAcces
        // to both operator functions
        return const_cast<DataType&> (safeAccess(row, col));
    }

    /**
     * Read-only access to matrix elements.
     *
     * @param row row to access < Rows
     * @param col column to access < Cols
     * @return the desired matrix element
     */
    DataType operator()(const unsigned int &row, const unsigned int &col) const {
        return safeAccess(row, col);
    }

    /**
     * Multiplies this matrix with a given other one using this * other.
     *
     * @param other other matrix to multiply with
     * @return multiplication result as a new matrix. Other matrices are not
     *                        changed
     */
    template<unsigned int OtherCols>
    Matrix<Rows, OtherCols, DataType> mult(const Matrix<Cols, OtherCols,
            DataType> &other) const {

        Matrix<Rows, OtherCols> result;

        for (unsigned int otherCol = 0; otherCol < OtherCols; ++otherCol) {
            for (unsigned int row = 0; row < Rows; ++row) {
                DataType res = 0;
                for (unsigned int col = 0; col < Cols; ++col) {
                    res += (data[row][col] * other(col, otherCol));
                }
                result(row, otherCol) = res;
            }
        }

        return result;
    }

    /**
     * Performs a multiplication of this matrix with a scalar.
     *
     * @param scalar scalar to multiply with
     * @return a reference to this matrix itself
     */
    Matrix<Rows, Cols, DataType> &mult(const DataType &scalar) {
        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Cols; ++col) {
                data[row][col] *= scalar;
            }
        }
        return *this;
    }

    /**
     * Multiplies this matrix with a given other one using this * other.
     *
     * @param other other matrix to multiply with
     * @return multiplication result
     */
    template<unsigned int OtherCols>
    Matrix<Rows, OtherCols, DataType> operator*(const Matrix<Cols, OtherCols,
            DataType> &other) const {
        return mult(other);
    }

    /**
     * Adds a scalar value to all elements of this matrix.
     *
     * @param scalar scalar to add.
     */
    Matrix<Rows, Cols, DataType> add(const DataType &scalar) {
        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Cols; ++col) {
                data[row][col] += scalar;
            }
        }
        return *this;
    }

    /**
     * Adds the contents of the other, equally-sized matrix to the elements of
     * this matrix.
     *
     * @param other other matrix
     * @return this matrix which is changed
     */
    Matrix<Rows, Cols, DataType> add(const Matrix<Rows, Cols, DataType> &other) {
        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Cols; ++col) {
                data[row][col] += other.data[row][col];
            }
        }
        return *this;
    }

    /**
     * Comparison operator using the specified comparator.
     *
     * @param other other matrix to compare with
     * @return @c true if matrices are equal according to comparison operation
     *         else @c false
     */
    bool operator==(const Matrix<Rows, Cols, DataType> &other) const {

        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Cols; ++col) {
                if (data[row][col] != other.data[row][col]) {
                    return false;
                }
            }
        }

        return true;

    }

    /**
     * Sets a diagonal matrix with all elements = 0 except the diagonal
     * starting from the upper left element to the first bottom right element
     * that touches the border of the matrix with the given scaler as diagonal
     * value.
     *
     * @param scalar value of the diagonal (defaut 1)
     * @return this matrix
     */
    Matrix<Rows, Cols, DataType> setDiagonal(const DataType &scalar = 1) {
        for (unsigned int row = 0; row < Rows; ++row) {
            for (unsigned int col = 0; col < Cols; ++col) {
                if (row == col) {
                    data[row][col] = scalar;
                } else {
                    data[row][col] = 0;
                }
            }
        }
        return *this;
    }

    /**
     * Comparison operator using the specified comparator. Inverse of ==.
     *
     * @param other other matrix to compare with
     * @return @c false if matrices are equal according to comparison operation
     *         else @c true
     */
    bool operator!=(const Matrix<Rows, Cols> &other) const {
        return !this->operator ==(other);
    }

private:

    const DataType &safeAccess(unsigned int row, unsigned int col) const {
        if (row >= Rows) {
            std::stringstream s;
            s << "Row argument must be less than " << Rows << " but received "
                    << row;
            throw std::domain_error(s.str());
        }
        if (col >= Cols) {
            std::stringstream s;
            s << "Col argument must be less than " << Cols << " but received "
                    << col;
            throw std::domain_error(s.str());
        }
        return data[row][col];
    }

    DataType data[Rows][Cols];

};

/**
 * @todo do I need to implement this in header or can it also be implemented
 *       in the source file
 */
template<unsigned int Rows, unsigned int Cols, class DataType>
std::ostream &operator<<(std::ostream &stream, const Matrix<Rows, Cols,
        DataType> &matrix) {

    stream << "Matrix[rows = " << Rows << ", cols = " << Cols << "\n";

    for (unsigned int row = 0; row < Rows; ++row) {

        if (row == 0) {
            stream << "/";
        } else if (row == Rows - 1) {
            stream << "\\";
        } else {
            stream << "|";
        }

        stream << " ";

        // TODO automatically compute required precision
        for (unsigned int col = 0; col < Cols; ++col) {
            stream << std::setw(8) << matrix(row, col) << " ";
        }

        if (row == 0) {
            stream << "\\";
        } else if (row == Rows - 1) {
            stream << "/";
        } else {
            stream << "|";
        }

        stream << "\n";

    }

    return stream << "\n]";

}

typedef Matrix<3, 1, double> Vector3d;
typedef Matrix<3, 1, double> Vector3d;

}
}

