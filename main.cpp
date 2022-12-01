#include <iostream>

// Economic Planning Utilities.
#include <planning.h>

int main(int aArgCount, char** aArgList) {
	//float MatrixData[] = {
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f,
	//};
	//planning::matrix Matrix(4, MatrixData);
	planning::matrix Matrix(1024);

	std::cout << "Calculating" << std::endl;
	std::cout << "det(A) = " << planning::determinant(Matrix) << std::endl;
	std::cout << "Finished" << std::endl;
	return 0;
}