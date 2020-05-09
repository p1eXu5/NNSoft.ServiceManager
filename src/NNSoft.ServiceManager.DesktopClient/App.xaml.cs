using System.Windows;
using NNSoft.ServiceManager.Adapter;
using NNSoft.ServiceManager.DesktopClient.ViewModels;

namespace NNSoft.ServiceManager.DesktopClient
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup( StartupEventArgs e )
        {
            base.OnStartup( e );

            var mvm = new MainViewModel();

            var builder = new ServiceSubscriberBuilder();
            builder.Initialize();

            builder.Build( mvm );
            
            var wnd = new MainWindow {
                DataContext = mvm
            };
            wnd.Closed += (s, args) => builder.Dispose();
            wnd.Activated += (s, args) => mvm.ReloadCommand.Execute( null );

            wnd.Show();
        }
    }
}
