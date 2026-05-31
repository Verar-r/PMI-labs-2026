// лабораторная_3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "math.h"
#include "stdlib.h"

template <typename T>
	class Vector {
	protected:
		T* v; /*Указатель на массив элементов*/
		int size; /*Количество элементов*/
	public:
		Vector() : v(nullptr), size(0) {} /*Конструктор по умолчанию*/

		Vector(int n, T value = T()) {
			size = n;
			v = new T[size];
			for (int i = 0; i < size; i++) {
				v[i] = value;
			}
		}
		
		Vector(const Vector& copy) { /*конструктор копирования*/
			size=copy.size;
			v = new T[size];
			for (int i = 0; i < size; i++) 
				v[i] = copy.v[i];
		}

		~Vector() { /*деструктор*/
			delete[] v;
			v = nullptr;
			size = 0;
		}
		
		Vector& operator = (const Vector& other) { /*оператор присваивания*/
			if (this == &other) {
				return *this;
			}
			delete[] v;
			size = other.size;
			v = new T[other.size];
			for (int i = 0; i < size; i++) {
				v[i] = other.v[i];
			}
			return *this;
		}

		void swap(int i, int j) {
			T t = v[i];
			v[i] = v[j];
			v[j] = t;
		}

		T& operator [] (int i) {
			if (i < size && i >= 0) {
				return v[i];
			}
			else {
				throw "Error: Incorrect index";
			}
		}

		const T& operator[](int i) const {
			if (i < size && i >= 0) {
				return v[i];
			}
			else {
				throw "Error: Incorrect index";
			}
		}
		
		void PrintVector() {
			for (int i = 0; i < size; i++) {
				std::cout << v[i] << "\t";
			}
			std::cout << "\n";
		}

		void FillVector(){
			for (int i = 0; i < size; i++) {
				std::cin >> v[i];
			}
		}
	};

	template <typename T>
	class SquareMatrix : public Vector<Vector<T>> {
	public:
		SquareMatrix(int n) : Vector<Vector<T>>(n, Vector<T>(n, T())) { 
			this->size = n; 
		}

		SquareMatrix(int n, T a) : Vector<Vector<T>>(n, Vector<T>(n, a)) { 
			this->size = n; 
		}

		void PrintMatrix() {
			for (int i = 0; i < this->size; i++) {
				this->v[i].PrintVector();
				std::cout << "\n";
			}
		}

		Vector<T>& operator [] (int i) {
			if (i < this->size && i >= 0) {
				return this->v[i];
			}
			else {
				throw "Error: Incorrect index";
			}
		}

		const Vector<T>& operator [] (int i) const {
			if (i < this->size && i >= 0) {
				return this->v[i];
			}
			else {
				throw "Error: Incorrect index";
			}
		}

		void FillMatrix() {
			for (int i = 0; i < this->size; i++) {
				this->v[i].FillVector();
			}
		}

		void SwapRow(int i, int j) {
			Vector<T> t = this->v[i];
			this->v[i] = this->v[j];
			this->v[j] = t;
		}
	};

	template <typename T>
	class SLAU : public SquareMatrix<T> {
	public:
		SLAU(int n) : SquareMatrix<T>(n) {};
		SLAU(int n, T a) : SquareMatrix<T>(n, a) {}

		Vector<T> Gauss(Vector<T> b) {
			SLAU<T> copy = *this; /*копия исходной матрицы коэффициентов*/
			int n = copy.size;
			if (n == 0) {
				throw "Error: size = 0";
			}

			Vector<T> ans = b; /*копия правой части*/
			Vector<T> x(n, 0); /*вектор-результат*/
			Vector<int> pivot(n, -1);  /*вектор, хранит номер столбца ведущего элемента i-ой строки*/

			/*прямой ход, приведение к ступечатому виду*/
			int row = 0; /*текущая строка (индекс)*/
			for (int column = 0; column < n && row < n; column++) {
				int max_i = row; /*максимальный по модулю элемент в столбце (индекс)*/
				T max_el = copy.v[row][column]; /*максимальный по модулю элемент в столбце*/
				for (int i = row + 1; i < n; i++) {
					if (abs(copy.v[i][column]) > abs(copy.v[max_i][column])) {
						max_i = i;
						max_el = copy.v[i][column];
					}
				}
				if (abs(copy.v[max_i][column]) > 0.0000000000000001) {/*если ведущий элемент не ноль*/
					if (max_i != row) {
						copy.SwapRow(row, max_i);
						ans.swap(row, max_i);
					}

					pivot[row] = column;
					
					for (int i = row + 1; i < n; i++) {/*исключаем элементы снизу*/
						T k = copy.v[i][column] / copy.v[row][column];
						for (int j = column; j < n; j++) {
							copy.v[i][j] = copy.v[i][j] - k * copy.v[row][j];
						}
						ans[i] = ans[i] - k * ans[row];
					}
					row++;
				}/*иначе переходим к следующему столбцу*/
			}

			/*совместность системы: если в левой части нулевая строка, а в правой не ноль, то решений нет*/
			for (int j = row; j < n; j++) {
				if (abs(ans[j]) > 0.0000000000000001) {
					throw "No solutions";
				}
			}

			if (row < n) {
				std::cout << "Infinitely many solutions, one of the private solutions:\n";
			}

			/*обратный ход*/
			for (int i = row - 1; i >= 0; i--) {
				int c = pivot[i]; /*номер столбца ведущего элемента i-ой строки*/
				T sum = 0;
				for (int j = c + 1; j < n; j++) {
					sum = sum + copy.v[i][j] * x[j];
				}
				x[c] = (ans[i] - sum) / copy.v[i][c];
			}
			return x;
		}
	};

	void menu() {
		std::cout << "\nDo you want to enter another vector b?: "<<"\n";
		std::cout << "1 - yes"<<"\n";
		std::cout << "2 - no"<<"\n";
		std::cout << "\n";
	}

	int main() {
		try {
			int n;
			std::cout << "Enter size of system: ";
			std::cin >> n;
			std::cout << "\n";

			SLAU<double> A(n);
			std::cout << "Enter matrix A elements: ";
			A.FillMatrix();

			std::cout << "\nMatrix A:\n";
			A.PrintMatrix();

			int point = 1;
			while (point == 1) {
				std::cout << "\nEnter vector b elements: ";
				Vector<double> b(n);
				b.FillVector();

				std::cout << "\nVector b: \n";
				b.PrintVector();
				std::cout << "\n";

				try {
					Vector<double> x = A.Gauss(b);

					std::cout << "\nSolution x:\n";
					x.PrintVector();
				}
				catch (const char* mes) {
					std::cerr << "Error: " << mes << "\n";
				}

				menu();
				std::cin >> point;
			}
		}
		catch (const char* mes) {
			std::cerr << "\nError: " << mes << "\n";
		}
		return 0;
	}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
