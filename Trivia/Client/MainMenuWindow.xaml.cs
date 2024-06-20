using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for MainMenuWindow.xaml
    /// </summary>
    public partial class MainMenuWindow : Window
    {
        public MainMenuWindow()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;

        }

        private void Button_CreateRoom(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            CreateRoomWindow window = new CreateRoomWindow();
            window.Show();
            this.Close();
        }
        private void Button_JoinRoom(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            JoinRoomWindow window = new JoinRoomWindow();
            window.Show();
            this.Close();
        }
        private void Button_Statistics(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            StatisticsWindow window = new StatisticsWindow();
            window.Show();
            this.Close();
        }
        private void Button_Logout(object sender, RoutedEventArgs e)
        {
            Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
            byte[] bytes = Serializer.seriallizeRequest("", 203);
            newCommunicator.SendMsg(bytes);
            (int code, string response) = newCommunicator.ReceiveCodeAndResponse();
            ((App)Application.Current).SetHandleClosing(false);
            MainWindow window = new MainWindow();
            window.Show();
            this.Close();
        }
    }
}
