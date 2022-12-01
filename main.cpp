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
	float MatrixData[] = {
		12.0f, 0.0f, 10.0f, -4.0f,
		0.0f, 6.0f, 29.0f, 0.0f,
		2.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 2.0f
	};
	planning::matrix Matrix(4, MatrixData);
	//planning::matrix Matrix(1024);

	std::cout << "Calculating" << std::endl;
	std::cout << "det(A) = " << planning::determinant(Matrix) << std::endl;
	std::cout << "Finished" << std::endl;

	std::cout << Matrix.str().ptr() << std::endl;

	planning::matrix Invert = invert(Matrix);
	std::cout << Invert.str().ptr() << std::endl;

	planning::matrix Result = Matrix * Invert;
	std::cout << Result.str().ptr() << std::endl;

	return 0;
}