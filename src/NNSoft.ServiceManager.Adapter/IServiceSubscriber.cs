using System;
using System.Collections.Generic;

namespace NNSoft.ServiceManager.Adapter
{
    public class ServiceEventArgs : EventArgs
    {
        public ServiceEventArgs( Service service )
        {
            Service = service;
        }

        public Service Service { get; }
    }

    public delegate void ServiceEventHandler< in TEventArgs >( IServiceSubscriber subscriber, TEventArgs args ) where TEventArgs : EventArgs;

    public interface IServiceSubscriber : IDisposable
    {
        event ServiceEventHandler< EventArgs > ReloadRequested;
        event ServiceEventHandler< ServiceEventArgs > StartRequested;
        event ServiceEventHandler< ServiceEventArgs > StopRequested;
        event ServiceEventHandler< ServiceEventArgs > RestartRequested;
        event ServiceEventHandler< EventArgs > Disposed;

        void OnReload( IEnumerable< Service > services );
        void OnUpdated( Service service );
    }
}
