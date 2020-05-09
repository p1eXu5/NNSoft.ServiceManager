using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Input;
using System.Windows.Threading;
using NNSoft.ServiceManager.Adapter;
using p1eXu5.Wpf.MvvmBaseLibrary;

namespace NNSoft.ServiceManager.DesktopClient.ViewModels
{
    public class MainViewModel : ViewModelBase, IServiceSubscriber
    {
        private ServiceViewModel _selectedService;
        private readonly ObservableCollection< ServiceViewModel > _serviceVms;

        public MainViewModel()
        {
            _serviceVms = new ObservableCollection< ServiceViewModel >();
            ServiceVms = new ReadOnlyObservableCollection< ServiceViewModel >( _serviceVms );
        }

        public event ServiceEventHandler< EventArgs > ReloadRequested;
        public event ServiceEventHandler< ServiceEventArgs > StartRequested;
        public event ServiceEventHandler< ServiceEventArgs > StopRequested;
        public event ServiceEventHandler< ServiceEventArgs > RestartRequested;
        public event ServiceEventHandler< EventArgs > Disposed;


        public ReadOnlyObservableCollection< ServiceViewModel > ServiceVms { get; }

        public ServiceViewModel SelectedService
        {
            get => _selectedService; 
            set {
                _selectedService = value;
                OnPropertyChanged( nameof(SelectedService) );
            }
        }

        public ICommand ReloadCommand => new MvvmCommand( Reload );
        public ICommand StartCommand => new MvvmCommand( Start );
        public ICommand StopCommand => new MvvmCommand( Stop );
        public ICommand RestartCommand => new MvvmCommand( Restart );


        public void Dispose()
        {
            Disposed?.Invoke( this, EventArgs.Empty );
        }




        public void OnReload( IEnumerable< Service > services )
        {
            Dispatcher.CurrentDispatcher.BeginInvoke( new Action( () => {
                _serviceVms.Clear();
                foreach ( var service in services ) {
                    _serviceVms.Add( new ServiceViewModel( service ) );
                }
            } ) );
        }

        public void OnUpdated( Service service )
        {
            Dispatcher.CurrentDispatcher.BeginInvoke( new Action( () => {
                var serviceVm = _serviceVms.FirstOrDefault( s => s.Pid == service.Pid );
                if ( null == serviceVm ) {
                    _serviceVms.Add( new ServiceViewModel( service ) );
                }
                else {
                    serviceVm.Update( service );
                }
            } ) );
        }


        private void Reload( object obj )
        {
            ReloadRequested?.Invoke( this, EventArgs.Empty );
        }

        private void Start( object obj )
        {
            if ( obj is ServiceViewModel serviceVm )
                StartRequested?.Invoke( this, new ServiceEventArgs( serviceVm.Service ) );
        }

        private void Stop( object obj )
        {
            if ( obj is ServiceViewModel serviceVm )
                StopRequested?.Invoke( this, new ServiceEventArgs( serviceVm.Service ) );
        }

        private void Restart( object obj )
        {
            if ( obj is ServiceViewModel serviceVm )
                RestartRequested?.Invoke( this, new ServiceEventArgs( serviceVm.Service ) );
        }
    }
}
