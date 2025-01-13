#include "txt.h"

// Функция для преобразования текста в верхний регистр и записи в новый файл
void processFile(const std::string& inputFile, const std::string& outputFile) {

	std::ifstream inFile(inputFile);
	std::ofstream outFile(outputFile);

	if (!inFile.is_open() || !outFile.is_open())
	{
		std::cerr << "Error opening file" << inputFile << " or " << outputFile << std::endl;
		return;
	}
	std::string line;
	while (std::getline(inFile, line))
	{
		std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) {
			return std::toupper(c); }
		);
		outFile << line << "\n";
	}

}

// Функция для измерения времени выполнения
std::string formatDuration(const std::chrono::nanoseconds& duration) {
	std::chrono::seconds s = duration_cast<std::chrono::seconds>(duration);
	std::chrono::milliseconds ms = duration_cast<std::chrono::milliseconds>(duration % std::chrono::seconds(1));
	std::chrono::microseconds us = duration_cast<std::chrono::microseconds>(duration % std::chrono::microseconds(1));
	return std::to_string(s.count()) + "s::" +
		   std::to_string(ms.count()) + "ms::" +
		   std::to_string(us.count()) + "mks";

}