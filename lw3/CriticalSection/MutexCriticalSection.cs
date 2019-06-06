using System;
using System.Threading;

namespace lw3.CriticalSection
{
    public class MutexCriticalSection : ICriticalSection, IDisposable
    {
        private int _spinCount = 1;
        private readonly Mutex _waitHandler = new Mutex();

        public void Enter()
        {
            bool success = false;
            while (!success)
            {
                for (int i = 0; i < _spinCount; ++i)
                {
                    if (_waitHandler.WaitOne(10))
                    {
                        success = true;
                        break;
                    }
                }

                if (!success)
                {
                    Thread.Sleep(10);
                }
            }
        }

        public bool TryEnter(int timeout)
        {
            var startTime = DateTime.UtcNow;

            while (startTime.AddMilliseconds(timeout) > DateTime.UtcNow)
            {
                for (int i = 0; i < _spinCount; ++i)
                {
                    if (_waitHandler.WaitOne(10))
                    {
                        return true;
                    }

                    if (startTime.AddMilliseconds(timeout) <= DateTime.UtcNow)
                    {
                        return false;
                    }
                }
                Thread.Sleep(10);
            }

            return false;
        }

        public void SetSpinCount(int count)
        {
            _spinCount = count;
        }

        public void Leave()
        {
            _waitHandler.ReleaseMutex();
        }

        public void Dispose()
        {
            _waitHandler.Dispose();
        }
    }
}