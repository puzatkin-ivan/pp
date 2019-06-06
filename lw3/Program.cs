using System;
using System.Diagnostics;
using lw3.CriticalSection;
using lw3.IntergalCalculator;

namespace lw3
{
    class Program
    {
        public static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Insufficient argument count.");
                Console.WriteLine("Usage: dotnet run <iterationCount> <timeout> <count>");
                return;
            }

            int iterationCount = Convert.ToInt32(args[0]);
            int timeout = Convert.ToInt32(args[1]);
            int count = Convert.ToInt32(args[2]);

            ExecuteCalculatePiAndPrintCalculateResult(iterationCount, timeout, count, CriticalSectionEnum.Enter);
            ExecuteCalculatePiAndPrintCalculateResult(iterationCount, timeout, count, CriticalSectionEnum.TryEnter);
        }

        private static void ExecuteCalculatePiAndPrintCalculateResult(int iterationCount, int timeOut, int count, CriticalSectionEnum type)
        {
            Stopwatch stopwatch = Stopwatch.StartNew();
            double pi = CalculatePiValue(iterationCount, timeOut, count, type);
            stopwatch.Stop();
            
            PrintCalculateResult(pi, stopwatch.ElapsedMilliseconds, type);
        }

        private static double CalculatePiValue(int iterationCount, int timeOut, int count, CriticalSectionEnum type)
        {
            ThreadIntegralCalculator calculator = new ThreadIntegralCalculator(iterationCount, timeOut, count, type);
            return calculator.Calculate();;
        }

        private static void PrintCalculateResult(double pi, long workingTime, CriticalSectionEnum type)
        {
            Console.WriteLine("-------Begin-------");
            Console.WriteLine("PI: "  + pi);
            Console.WriteLine("Time: "  + workingTime);
            Console.WriteLine("Type: "  + type);
            Console.WriteLine("-------End-------");
        }
    }
}
