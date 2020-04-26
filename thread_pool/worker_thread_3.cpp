#include <chrono>
#include <iostream>

int main()
{
    ThreadPool tp(5, 1000);

    for (int n = 0; n < 1000; n++) {
        auto workA = [ ] { std::cout << "WorkA"         << std::endl; };
        auto workB = [=] { std::cout << "WorkB n=" << n << std::endl; };

        while (!tp.add(workA)) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        while (!tp.add(workB)) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    return 0;
}
