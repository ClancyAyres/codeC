#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <chrono>
#include <iomanip>
#include <string>
#include <numeric>

using Clock = std::chrono::high_resolution_clock;

void printResult(const std::string& name, long long ms, const std::string& note = "") {
    std::cout << std::left << std::setw(10) << name
              << std::setw(8) << ms << "ms";
    if (!note.empty()) std::cout << "  " << note;
    std::cout << '\n';
}

void benchPushBack(size_t N) {
    std::cout << "\n=== push_back (N=" << N << ") ===\n";

    {
        std::vector<int> v;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) v.push_back(static_cast<int>(i));
        auto end = Clock::now();
        printResult("vector",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1) amortized");
    }

    {
        std::list<int> l;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) l.push_back(static_cast<int>(i));
        auto end = Clock::now();
        printResult("list",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1)");
    }

    {
        std::deque<int> d;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));
        auto end = Clock::now();
        printResult("deque",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1)");
    }
}

void benchPushFront(size_t N) {
    std::cout << "\n=== push_front (N=" << N << ") ===\n";

    {
        std::vector<int> v;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) v.insert(v.begin(), static_cast<int>(i));
        auto end = Clock::now();
        printResult("vector",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(N) — very slow!");
    }

    {
        std::list<int> l;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) l.push_front(static_cast<int>(i));
        auto end = Clock::now();
        printResult("list",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1) — fast");
    }

    {
        std::deque<int> d;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) d.push_front(static_cast<int>(i));
        auto end = Clock::now();
        printResult("deque",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1) — fast");
    }
}

void benchRandomAccess(size_t N) {
    std::cout << "\n=== random_access (N=" << N << ") ===\n";

    std::vector<int> v(N);
    std::iota(v.begin(), v.end(), 0);
    std::list<int> l(v.begin(), v.end());
    std::deque<int> d(v.begin(), v.end());

    {
        volatile long long sum = 0;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) sum += v[i];
        auto end = Clock::now();
        printResult("vector",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1) — fast");
    }

    {
        volatile long long sum = 0;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) {
            auto it = l.begin();
            std::advance(it, i);
            sum += *it;
        }
        auto end = Clock::now();
        printResult("list",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(N²) — very slow!");
    }

    {
        volatile long long sum = 0;
        auto start = Clock::now();
        for (size_t i = 0; i < N; ++i) sum += d[i];
        auto end = Clock::now();
        printResult("deque",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1) — slightly slower than vector");
    }
}

void benchIteration(size_t N) {
    std::cout << "\n=== iteration (N=" << N << ") ===\n";

    std::vector<int> v(N);
    std::iota(v.begin(), v.end(), 0);
    std::list<int> l(v.begin(), v.end());
    std::deque<int> d(v.begin(), v.end());

    {
        volatile long long sum = 0;
        auto start = Clock::now();
        for (const auto& x : v) sum += x;
        auto end = Clock::now();
        printResult("vector",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "cache-friendly, fastest");
    }

    {
        volatile long long sum = 0;
        auto start = Clock::now();
        for (const auto& x : l) sum += x;
        auto end = Clock::now();
        printResult("list",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "pointer chasing, slowest");
    }

    {
        volatile long long sum = 0;
        auto start = Clock::now();
        for (const auto& x : d) sum += x;
        auto end = Clock::now();
        printResult("deque",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "chunked, medium speed");
    }
}

void benchInsertMiddle(size_t N) {
    const size_t insertCount = 1000;
    std::cout << "\n=== insert_middle (N=" << N << ", insert " << insertCount << " at middle) ===\n";

    {
        std::vector<int> v(N);
        std::iota(v.begin(), v.end(), 0);
        auto mid = v.begin() + v.size() / 2;
        auto start = Clock::now();
        for (size_t i = 0; i < insertCount; ++i) {
            mid = v.insert(mid, 0);
        }
        auto end = Clock::now();
        printResult("vector",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(N) per insert — slow");
    }

    {
        std::list<int> l(N);
        std::iota(l.begin(), l.end(), 0);
        auto mid = l.begin();
        std::advance(mid, l.size() / 2);
        auto start = Clock::now();
        for (size_t i = 0; i < insertCount; ++i) {
            l.insert(mid, 0);
        }
        auto end = Clock::now();
        printResult("list",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(1) per insert — fast");
    }

    {
        std::deque<int> d(N);
        std::iota(d.begin(), d.end(), 0);
        auto mid = d.begin() + d.size() / 2;
        auto start = Clock::now();
        for (size_t i = 0; i < insertCount; ++i) {
            mid = d.insert(mid, 0);
        }
        auto end = Clock::now();
        printResult("deque",
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
            "O(N) per insert — slow");
    }
}

int main() {
    const size_t N = 100000;

    benchPushBack(N);
    benchPushFront(N);
    benchRandomAccess(N);
    benchIteration(N);
    benchInsertMiddle(N);

    std::cout << "\n=== Summary ===\n";
    std::cout << "vector: best for random access, tail ops, iteration\n";
    std::cout << "deque:  best for front+back ops, good random access\n";
    std::cout << "list:   best for middle insert/delete, no random access\n";

    return 0;
}
