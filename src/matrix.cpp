#include <planning/matrix.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include <iostream>

// Levi Civita Garbage
static int sift_down(int* A, int Length, int i) {
	if ((i < 0) && (i >= Length)) return 0;
	static int SwapCount = 0;
	static int StackDepth = 0;
	StackDepth += 1;

	int L = 2 * i + 1; // Left Child
	int R = 2 * i + 2; // Right Child

	int SwapContainer;
	if ((L < Length) && (R < Length)) {
		if (A[L] > A[R]) {
			if (A[L] > A[i]) {
				SwapContainer = A[L];
				A[L] = A[i];
				A[i] = SwapContainer;
				SwapCount += 1;
				sift_down(A, Length, L);
			}
		}
		else {
			if (A[R] > A[i]) {
				SwapContainer = A[R];
				A[R] = A[i];
				A[i] = SwapContainer;
				SwapCount += 1;
				sift_down(A, Length, R);
			}
		}
	}
	else if (L < Length) {
		if (A[L] > A[i]) {
			SwapContainer = A[L];
			A[L] = A[i];
			A[i] = SwapContainer;
			SwapCount += 1;
		}
	}

	StackDepth -= 1;
	if (StackDepth == 0) {
		int SwapTotal = SwapCount;
		SwapCount = 0;
		return SwapTotal;
	}
	else {
		return 0;
	}
}

static int build_heap(int* A, int Length) {
	int SwapCount = 0;
	for (int i = Length - 1; i >= 0; i--) {
		if (2 * i + 1 >= Length) continue;
		SwapCount += sift_down(A, Length, i);
	}
	return SwapCount;
}

static int heap_sort(int* A, int Length) {
	int SwapCount = 0;
	SwapCount += build_heap(A, Length);
	int SwapContainer;
	int HeapSize = Length;
	while (HeapSize > 1) {
		SwapContainer = A[0];
		A[0] = A[HeapSize - 1];
		A[HeapSize - 1] = SwapContainer;
		HeapSize -= 1;
		SwapCount += 1;
		SwapCount += sift_down(A, HeapSize, 0);
	}
	return SwapCount;
}

// Probably Delete
int kronecker_delta(int I, int J) {
	return (I == J) ? 1 : 0;
}

//tex:
// The Levi Civita Symbol is known for its skew-symmetric nature.
// The property being that if any two of the indices are swapped,
// it is the negative of itself. If any two indices repeat, just
// swapping those two particular indices lead to the same entry,
// therefore leaving only zero to be the possible answer. The rank
// of the tensor is then $n$, with $n$ indices. The larger the matrix
// gets, the less feasible this is to calculate. There must be other
// methods.
// $$ i_{1}, i_{2}, i_{3}, ... i_{n} \in [1, n] $$
// $$ \varepsilon_{i_{1}, i_{2}, ... i_{n}} \in \{ -1, 0, +1 \} $$
int levi_civita(int aRank, const int* aIndex) {
	//tex:
	// All values must be $ i_{1}, i_{2}, i_{3},...i_{n} \in [1, n] $
	// $$ \varepsilon_{i_{i} i_{2} ... i_{n}} \in \{ -1, 0, +1 \}$$
	// If any indices repeat, then $\varepsilon_{i_{i} i_{2} ... i_{n}} = 0$
	if ((aRank <= 0) || (aIndex == NULL)) return 0;
	//tex:
	// Checks for index values in $ i_{j} \in [1, n] $.
	for (int i = 0; i < aRank; i++) {
		if ((aIndex[i] < 1) || (aIndex[i] >= aRank)) return 0;
	}
	//tex:
	// Checks for duplicate indices, returns 0 if there are duplicates.
	for (int i = 0; i < aRank; i++) {
		for (int j = i + 1; j < aRank; j++) {
			if (aIndex[i] == aIndex[j]) return 0;
		}
	}
	// Indices are valid, and unique. Sort to find signature.
	int* Index = (int*)malloc(aRank * sizeof(int));
	if (Index == NULL) return 0;
	memcpy(Index, aIndex, aRank * sizeof(int));
	int IndexSwapCount = heap_sort(Index, aRank);
	free(Index);
	return (((IndexSwapCount % 2) > 0) ? -1 : +1);
}

// This is the same function, but for syntactical sugar.
int levi_civita(int aRank, ...) {

	//int i = 0;
	//int j = 0;

	int* Index = NULL;
	Index = (int*)malloc(aRank * sizeof(int));
	if (Index == NULL) return 0;

	va_list ArgList;
	va_start(ArgList, aRank);
	for (int i = 0; i < aRank; i++) {
		// Assumes user is not retarded.
		Index[i] = va_arg(ArgList, int);
	}
	va_end(ArgList);

	//// Checks for illegal values.
	//for (i = 0; i < aRank; i++) {
	//	if ((Index[i] < 0) && (Index[i] >= aRank)) {
	//		free(Index); Index = NULL;
	//		return 0;
	//	}
	//}

	//// Checks for duplicate values.
	//for (i = 0; i < aRank; i++) {
	//	for (j = i + 1; j < aRank; j++) {
	//		if (Index[i] == Index[j]) {
	//			free(Index); Index = NULL;
	//			return 0;
	//		}
	//	}
	//}

	int temp = levi_civita(aRank, Index);

	free(Index); Index = NULL;

	return temp;
}


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
	}

	matrix::matrix(int aDimension, float* aData) {
		this->zero_out();
		this->resize(aDimension);
		memcpy(this->ptr, aData, this->n * this->n * sizeof(float));
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
		return this->ptr[aJ + this->n * aI];
	}

	float matrix::operator()(int aI, int aJ) const {
		return this->ptr[aJ + this->n * aI];
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
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
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
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
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
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				C(i, j) = A(i, j) * aRhs;
			}
		}
		return C;
	}

	matrix matrix::operator/(float aRhs) const {
		//assert(this->n != aRhs.n);
		matrix A = *this;
		//matrix B = aRhs;
		matrix C(this->n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				C(i, j) = A(i, j) / aRhs;
			}
		}
		return C;
	}

	matrix matrix::operator*(const matrix& aRhs) const {
		assert(this->n == aRhs.n);
		matrix A = *this;
		matrix B = aRhs;
		matrix C(this->n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				C(i, j) = 0.0f;
				for (int k = 0; k < n; k++) {
					C(i, j) += A(i, k) * B(k, j);
				}
			}
		}
		return C;
	}

	std::vector<float> matrix::operator*(std::vector<float> aRhs) const {
		std::vector<float> Return(aRhs.size());
		for (int i = 0; i < n; i++) {
			Return[i] = 0.0f;
			for (int j = 0; j < n; j++) {
				Return[i] += (*this)(i, j) * aRhs[j];
			}
		}
		return Return;
	}

	string matrix::str() const {
		string Str;
		for (int j = 0; j < n; j++) {
			Str += "[";
			for (int i = 0; i < n; i++) {
				float Number = (*this)(i, j);
				char NumberString[256];
				memset(NumberString, 0x00, sizeof(NumberString));
				sprintf(NumberString, "%.4f", Number);

				if (Number >= 0.0f) {
					Str += " ";
				}				

				Str += NumberString;
				if (i != n - 1) {
					Str += ",\t";
				}
			}
			Str += "]\n";
		}

		return Str;
	}

	matrix matrix::minor(int aI, int aJ) const {
		matrix Minor(this->n - 1);
		for (int i = 0; i < Minor.size(); i++) {
			for (int j = 0; j < Minor.size(); j++) {
				int Decision = 0x00000000 | (((i < aI) << 1) | (j < aJ));
				switch (Decision) {
				default:
					Minor(i, j) = 0.0f;
					break;
				case 0b00:
					Minor(i, j) = (*this)(i + 1, j + 1);
					break;
				case 0b01:
					Minor(i, j) = (*this)(i + 1, j);
					break;
				case 0b10:
					Minor(i, j) = (*this)(i, j + 1);
					break;
				case 0b11:
					Minor(i, j) = (*this)(i, j);
					break;
				}
			}
		}
		std::cout << Minor.str().ptr() << std::endl;
		return Minor;
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

	int matrix::size() const {
		return this->n;
	}

	void matrix::clear() {
		free(this->ptr);
		this->zero_out();
	}

	void matrix::zero_out() {
		this->n = 0;
		this->ptr = NULL;
	}

	// Computationally infeasible for matrices larger than 10.
	/*
	float determinant(const matrix& aInput) {
		float Determinant = 0.0f;
		matrix A = aInput;
		int n = aInput.size();
		int* i = (int*)malloc(n * sizeof(int));

		// Set Starting Index Vector
		for (int k = 0; k < n; k++) {
			i[k] = 0;
		}

		while (true) {
			int k = 0;
			int Sign = 0;
			float AggregateProduct = 1.0f;
			bool CarryOver = true;
			bool BreakOut = true;

			// Print Index Vector.
			std::cout << "(";
			for (k = 0; k < n; k++) {
				std::cout << i[k] << ",";
			}
			std::cout << ")" << std::endl;

			// Check for exit condition.
			for (k = 0; k < n; k++) {
				BreakOut &= (i[k] == n - 1);
			}
			if (BreakOut) break;

			// Determine Sign of contributing factor.
			Sign = levi_civita(n, i);

			// No contribution if repeating indices.
			if (Sign != 0) {
				// Calculate Determinant Factor
				for (k = 0; k < n; k++) {
					AggregateProduct *= A(k, i[k]);
				}
				Determinant += Sign * AggregateProduct;
			}

			// Update Indexing Vector.
			k = n - 1;
			while (CarryOver) {
				if (i[k] < n) {
					i[k] += 1;
					CarryOver = false;
				}
				else {
					i[k] = 0;
					CarryOver = true;
					k -= 1;
				}
			}
		}

		free(i);

		return Determinant;
	}
	*/

	// Uses recursion to calculate.
	float determinant(const matrix& aInput) {
		int k = 0;
		int n = aInput.size();
		if (n > 1) {
			float Total = 0.0f;
			for (k = 0; k < n; k++) {
				int Sign = (((k % 2) > 0) ? +1 : -1);
				float a = aInput(1, k);
				if (a != 0.0f) {
					matrix M = aInput.minor(1, k);
					Total += Sign * a * determinant(M);
				}
			}
			return Total;
		}
		else {
			return aInput(0, 0);
		}
	}

	float trace(const matrix& aInput) {
		float Total = 0.0f;
		for (int k = 0; k < aInput.size(); k++) {
			Total += aInput(k, k);
		}
		return Total;
	}

	matrix transpose(const matrix& aInput) {
		matrix T(aInput.size());
		for (int i = 0; i < T.size(); i++) {
			for (int j = 0; j < T.size(); j++) {
				T(i, j) = aInput(j, i);
			}
		}
		return T;
	}

	matrix adjugate(const matrix& aInput) {
		matrix C(aInput.size());
		int n = C.size();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int Sign = (((i + j) % 2) > 0) ? -1 : +1;
				C(i, j) = Sign * determinant(aInput.minor(i, j));
			}
		}
		return C;
	}

	matrix invert(const matrix& aInput) {
		matrix Inversion(aInput.size());
		Inversion = (transpose(adjugate(aInput))) / determinant(aInput);
		return Inversion;
	}

}

