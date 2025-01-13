#include "search.h"

void searchInRange(const std::vector<int>& vec, size_t start, size_t end, int target, std::atomic<bool>& found) {
	for (size_t i = start; i < end&&!found; ++i)
	{
		if (vec[i]==target)
		{
			//Устанавливаем флаг
			found = true;
			std::cout << "Found " << target << " at index: " << i << " by thread: " << std::this_thread::get_id() << "\n";
			return;
		}
	}
}