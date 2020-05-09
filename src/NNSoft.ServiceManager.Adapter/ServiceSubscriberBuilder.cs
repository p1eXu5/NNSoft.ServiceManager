using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace NNSoft.ServiceManager.Adapter
{
    public class ServiceSubscriberBuilder : IDisposable
    {
        private List< IServiceSubscriber > _subscribers;


        public List< IServiceSubscriber > Subscribers => 
            
            _subscribers ?? ( _subscribers = new List< IServiceSubscriber >() );

        public void Build( IServiceSubscriber subscriber )
        {
            var subscribers = Subscribers;

            subscriber.ReloadRequested += Reload;
            subscriber.StartRequested += Start;
            subscriber.StopRequested += Stop;
            subscriber.RestartRequested += Restart;
            subscriber.Disposed += Dispose;

            subscribers.Add( subscriber );
        }


        private void Dispose( IServiceSubscriber subscriber, EventArgs args )
        {
            var subscribers = Subscribers;

            if ( subscribers.Contains( subscriber ) ) {
                subscriber.ReloadRequested -= Reload;
                subscriber.StartRequested -= Start;
                subscriber.StopRequested -= Stop;
                subscriber.RestartRequested -= Restart;
                subscriber.Disposed -= Dispose;
            }
        }

        private void Reload( IServiceSubscriber subscriber, EventArgs args )
        {
            var services = Enumerable.Range( 1, 50 ).Select( n =>
                new Service( n, $"Test {n}", $"Description #{n}", Statuses.Running, $"Group {n}", $"Image path #{n}")
            );

            subscriber.OnReload( services );
        }

        private void Start( IServiceSubscriber subscriber, ServiceEventArgs args )
        {
            var service = args.Service;
            subscriber.OnUpdated( new Service( service.Pid, service.Name, service.Description, Statuses.Running, service.Group, service.ImagePath ) );
        }

        private void Stop( IServiceSubscriber subscriber, ServiceEventArgs args )
        {
            var service = args.Service;
            subscriber.OnUpdated( new Service( service.Pid, service.Name, service.Description, Statuses.Stopped, service.Group, service.ImagePath ) );
        }

        private void Restart( IServiceSubscriber subscriber, ServiceEventArgs args )
        {
            var service = args.Service;
            subscriber.OnUpdated( new Service( service.Pid, service.Name, service.Description, Statuses.Running, service.Group, service.ImagePath ) );
        }

        public void Dispose()
        {
            var subscribers = Subscribers;

            foreach ( var subscriber in subscribers ) {
                Dispose( subscriber, EventArgs.Empty );
            }
        }


        public void Initialize()
        {
            var sc = IntPtr.Size == 8 ? OpenSCManager_64() : OpenSCManager_32();

            if ( IntPtr.Size == 8 ) {
                CloseServiceHandle_64( sc );
            }
            else {
                CloseServiceHandle_32( sc );
            }
        }


        [DllImport("NNSoft.ServiceManagerWin32.dll", EntryPoint = "openSCManager", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr OpenSCManager_32();

        [DllImport("NNSoft.ServiceManagerx64.dll", EntryPoint = "openSCManager", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr OpenSCManager_64();


        [DllImport("NNSoft.ServiceManagerWin32.dll", EntryPoint = "closeServiceHandle", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool CloseServiceHandle_32( IntPtr sc );

        [DllImport("NNSoft.ServiceManagerx64.dll", EntryPoint = "closeServiceHandle", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool CloseServiceHandle_64( IntPtr sc );
    }
}
