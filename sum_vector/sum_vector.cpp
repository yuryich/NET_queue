#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

void vector_addition(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end) {
	for (int i = start; i < end; ++i) {
		result[i] = a[i] + b[i];
	}
}

double run_test(int num_threads, int size) {
	std::vector<int> a(size, 1);
	std::vector<int> b(size, 2);
	std::vector<int> result(size);

	std::vector<std::thread> threads;
	int chunk_size = size / num_threads;

	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < num_threads; ++i) {
		int start = i * chunk_size;
		int end = (i == num_threads - 1) ? size : start + chunk_size;
		threads.emplace_back(vector_addition, std::cref(a), std::cref(b), std::ref(result), start, end);
	}

	for (auto& t : threads) {
		t.join();
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end_time - start_time;
	return elapsed.count();
}

int main() {
	setlocale(LC_ALL, "RUS");
	std::cout << "Количество аппаратных ядер - " << std::thread::hardware_concurrency() << "\n\n";

	std::vector<int> sizes = { 1000, 10000, 100000, 1000000 };
	std::vector<int> thread_counts = { 1, 2, 4, 8, 16 };

	std::cout << std::setw(12) << " ";
	for (int size : sizes) {
		std::cout << std::setw(10) << size;
	}
	std::cout << "\n";

	for (int threads : thread_counts) {
		std::cout << std::setw(2) << threads << " потоков ";
		for (int size : sizes) {
			double time = run_test(threads, size);
			std::cout << std::setw(10) << std::fixed << std::setprecision(6) << time << "s";
		}
		std::cout << "\n";
	}

	return 0;
}