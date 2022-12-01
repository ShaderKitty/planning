#pragma once
#ifndef PLANNING_MATRIX_H
#define PLANNING_MATRIX_H

#include <vector>

namespace planning {

	class matrix {
	public:

		matrix();
		matrix(int aDimension);
		matrix(int aDimension, float* aData);
		matrix(const matrix& aInput);
		matrix(matrix&& aInput) noexcept;
		~matrix();

		// Access Operations
		float& operator()(int aI, int aJ);
		float operator()(int aI, int aJ) const;

		matrix& operator=(const matrix& aRhs);
		matrix& operator=(matrix&& aRhs) noexcept;

		// Will crash is dimensions don't match.
		matrix operator+(const matrix& aRhs) const;
		matrix operator-(const matrix& aRhs) const;

		// Scalar Multiplication.
		matrix operator*(float aRhs) const;
		matrix operator*(const matrix& aRhs) const;

		std::vector<float> operator*(std::vector<float> aRhs) const;

		matrix minor(int aI, int aJ) const;

		void resize(int aDimension);

		int size() const;

		void clear();

	private:

		int n;
		float* ptr;

		void zero_out();

	};

	float determinant(const matrix& aInput);
	float trace(const matrix& aInput);

	matrix transpose(const matrix& aInput);
	matrix invert(const matrix& aInput);

}

#endif // !PLANNING_MATRIX_H
