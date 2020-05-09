using System;
using NNSoft.ServiceManager.Adapter;
using p1eXu5.Wpf.MvvmBaseLibrary;

namespace NNSoft.ServiceManager.DesktopClient.ViewModels
{
    public class ServiceViewModel : ViewModelBase
    {
        private Service _service;

        public ServiceViewModel( Service service )
        {
            _service = service;
        }

        public Service Service => _service;
        public int Pid => _service.Pid;
        public string Name => _service.Name;
        public string Description => _service.Description;
        public string Status => _service.Status == Statuses.Running ? Enum.GetName( typeof( Statuses ), _service.Status ) : "";
        public string Group => _service.Group;
        public string ImagePath => _service.ImagePath;

        public void Update( Service service )
        {
            _service = service;
            OnPropertyChanged( nameof( Pid ) );
            OnPropertyChanged( nameof( Name ) );
            OnPropertyChanged( nameof( Description ) );
            OnPropertyChanged( nameof( Status ) );
            OnPropertyChanged( nameof( Group ) );
            OnPropertyChanged( nameof( ImagePath ) );
        }
    }
}
