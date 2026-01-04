#include "test/test_isseq.hpp"
#include "test/test_seq.hpp"
#include "utils/log.hpp"

static auto evaluate() -> int
{
    bool failed = false;

    for (const auto& [testFn, fnName] : SeqTest::CASES)
    {
        try
        {
            testFn();

            Log::printBdColor("✓ ", AnsiColor::GREEN);
            Log::println("{}", fnName);
        }
        catch (const Failure& f)
        {
            Log::printBdColor("✗ ", AnsiColor::RED);
            Log::println("{}", fnName);
            Log::println("├─ {}", f.what());
            Log::println("└─ {}", f.where());

            failed = true;
        }
    }

    return failed ? 1 : 0;
}

auto main() -> int { return evaluate(); }
