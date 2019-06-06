using System;
using System.Collections.Generic;
using System.Threading;
using lw3.CriticalSection;

namespace lw3.IntergalCalculator
{
    public class ThreadIntegralCalculator
    {
        private const int THREAD_COUNT = 4;
        private int _iterationCount;
        private int _timeOut;
        private int _count;
        private CriticalSectionEnum _type;
        private ICriticalSection _cs;
        private static double _pi;

        public ThreadIntegralCalculator(int iterationCount, int timeOut, int count, CriticalSectionEnum type)
        {
            _iterationCount = iterationCount;
            _timeOut = timeOut;
            _count = count;
            _type = type;
            _cs = new MutexCriticalSection();
            _cs.SetSpinCount(_count);
        }
        
        public double Calculate()
        {
            _pi = 0;
            Action enterToCs = GetEnterCs();
            void LeaveCs() => _cs.Leave();
            List<Thread> threads = new List<Thread>();
            
            double step = 1.0 / _iterationCount;

            for (int i = 0; i < THREAD_COUNT; i++)
            {
                var newThread = new Thread(IntegratePi);
                newThread.Start(GetCalculateParams(i, step, enterToCs, LeaveCs));

                threads.Add(newThread);
            }

            foreach (Thread worker in threads)
            {
                worker.Join();
            }

            return _pi;
        }

        private CalculateIntegralParams GetCalculateParams(int threadNumber, double step, Action enterToCs, Action leaveCs)
        {
            int iterationNumberPerThread = _iterationCount / THREAD_COUNT;
            return new CalculateIntegralParams(threadNumber * iterationNumberPerThread,
                (threadNumber + 1) * iterationNumberPerThread, step, enterToCs, leaveCs);
        }

        private static void IntegratePi(object integrationRules)
        {
            var integrationRulesObj = (CalculateIntegralParams)integrationRules;
            for (long i = integrationRulesObj.left; i < integrationRulesObj.right; i++)
            {
                double x = (i + 0.5) * integrationRulesObj.step;
                double sum = 4.0 / (1.0 + x * x);

                integrationRulesObj.enter();
                _pi += sum * integrationRulesObj.step;
                integrationRulesObj.leave();
            }
        }

        private Action GetEnterCs()
        {
            Action enterToCs = () => _cs.Enter();
            if (_type == CriticalSectionEnum.TryEnter)
            {
                enterToCs = () => { while (!_cs.TryEnter(_timeOut)) { } };
            }

            return enterToCs;
        }
    }
}