#include <iostream>

// Economic Planning Utilities.
#include <planning.h>

int main(int aArgCount, char** aArgList) {
	float MatrixData[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	planning::matrix Matrix(4, MatrixData);

	float Output = planning::determinant(Matrix);

	return 0;
}