using System;

namespace lw3.IntergalCalculator
{
    public struct CalculateIntegralParams
    {
        public long left;
        public long right;
        public double step;
        public Action enter;
        public Action leave;

        public CalculateIntegralParams(long left, long right, double step, Action enter, Action leave)
        {
            this.left = left;
            this.right = right;
            this.step = step;
            this.enter = enter;
            this.leave = leave;
        }
    }
}