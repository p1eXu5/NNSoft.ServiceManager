using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NNSoft.ServiceManager.Adapter
{
    public static class ServiceConstants
    {
        public const int SERVICE_STOPPED = 1;
        public const int SERVICE_START_PENDING = 2;
        public const int SERVICE_STOP_PENDING = 3;
        public const int SERVICE_RUNNING = 4;
        public const int SERVICE_CONTINUE_PENDING = 5;
        public const int SERVICE_PAUSE_PENDING = 6;
        public const int SERVICE_PAUSED = 7;
    }

    public enum Statuses
    {
        Unknown = 0,
        Stopped = ServiceConstants.SERVICE_STOPPED,
        Starting = ServiceConstants.SERVICE_START_PENDING,
        Stopping = ServiceConstants.SERVICE_STOP_PENDING,
        Running = ServiceConstants.SERVICE_RUNNING,
        Continuing = ServiceConstants.SERVICE_CONTINUE_PENDING,
        Pausing = ServiceConstants.SERVICE_PAUSE_PENDING,
        Paused = ServiceConstants.SERVICE_PAUSED
    }
}
