#include <planning/matrix.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Levi Civita Garbage

namespace planning {

	matrix::matrix() {
		this->zero_out();
	}

	matrix::~matrix() {
		this->clear();
	}

	matrix::matrix(int aDimension) {
		this->zero_out();
		this->resize(aDimension);
		for (int i = 0; i < this->n * this->n; i++) {
			this->ptr[i] = 0.0f;
		}
	}

	matrix::matrix(const matrix& aInput) {
		this->zero_out();
		this->resize(aInput.n);
		memcpy(this->ptr, aInput.ptr, this->n * this->n * sizeof(float));
	}

	matrix::matrix(matrix&& aInput) noexcept {
		this->n = aInput.n;
		this->ptr = aInput.ptr;
		aInput.zero_out();
	}

	float& matrix::operator()(int aI, int aJ) {
		return this->ptr[(aJ - 1) + this->n * (aI - 1)];
	}

	float matrix::operator()(int aI, int aJ) const {
		return this->ptr[(aJ - 1) + this->n * (aI - 1)];
	}

	matrix& matrix::operator=(const matrix& aRhs) {
		if (this == &aRhs) return *this;
		this->resize(aRhs.n);
		memcpy(this->ptr, aRhs.ptr, this->n * this->n * sizeof(float));
		return *this;
	}

	matrix& matrix::operator=(matrix&& aRhs) noexcept {
		this->clear();
		this->n = aRhs.n;
		this->ptr = aRhs.ptr;
		aRhs.zero_out();
		return *this;
	}

	matrix matrix::operator+(const matrix& aRhs) const {
		assert(this->n != aRhs.n);
		matrix A = *this;
		matrix B = aRhs;
		matrix C(this->n);
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				C(i, j) = A(i, j) + B(i, j);
			}
		}
		return C;
	}

	matrix matrix::operator-(const matrix& aRhs) const {
		assert(this->n != aRhs.n);
		matrix A = *this;
		matrix B = aRhs;
		matrix C(this->n);
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				C(i, j) = A(i, j) - B(i, j);
			}
		}
		return C;
	}

	matrix matrix::operator*(float aRhs) const {
		//assert(this->n != aRhs.n);
		matrix A = *this;
		//matrix B = aRhs;
		matrix C(this->n);
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				C(i, j) = A(i, j) * aRhs;
			}
		}
		return C;
	}

	matrix matrix::operator*(const matrix& aRhs) const {
		assert(this->n != aRhs.n);
		matrix A = *this;
		matrix B = aRhs;
		matrix C(this->n);
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				C(i, j) = 0.0f;
				for (int k = 1; k <= n; k++) {
					C(i, j) += A(i, k) * B(k, j);
				}
			}
		}
		return C;
	}

	void matrix::resize(int aDimension) {
		if (aDimension > 0) {
			// Crash if allocation failure.
			void* nptr = malloc(aDimension * aDimension * sizeof(float));
			assert(nptr);

			// Carry over values.
			for (int i = 0; i < aDimension; i++) {
				for (int j = 0; j < aDimension; j++) {
					float* temp = (float*)nptr;
					if ((i < this->n) && (j < this->n)) {
						temp[j + aDimension * i] = this->ptr[j + this->n * i];
					}
					else {
						temp[j + aDimension * i] = 0.0f;
					}
				}
			}

			this->clear();
			this->n = aDimension;
			this->ptr = (float*)nptr;
		}
		else {
			this->clear();
		}
	}

	void matrix::clear() {
		free(this->ptr);
		this->zero_out();
	}

	void matrix::zero_out() {
		this->n = 0;
		this->ptr = NULL;
	}

}

