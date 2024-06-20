using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    /// Interaction logic for Leaderboard.xaml
    /// </summary>
    public partial class Leaderboard : Window
    {
        Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
        public Leaderboard()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;
            Thread LeaderBoard = new Thread(ShowLeaderBoard);
            LeaderBoard.Start();

        }
        private void ShowLeaderBoard()
        {
            byte[] bytes = Serializer.seriallizeRequest("", 55);
            newCommunicator.SendMsg(bytes);
            (int code, string response) = newCommunicator.ReceiveCodeAndResponse();
            if (code == 101)
                return;
            TotalStatistics Leaderboard = JsonConvert.DeserializeObject<TotalStatistics>(response);
            CreateStatsText(Leaderboard);
        }
        private void CreateStatsText(TotalStatistics stats)
        {
            Dispatcher.Invoke(() =>
            {
                int count = 1 ;
                
                foreach (Stats stat in stats.Statistics)
                {
                    TextBlock block;
                    block = new TextBlock();

                    block.Text = "#" + count + "    " + stat.UserName + "    " + stat.Score + " 🎯";


                    block.FontFamily = new FontFamily("Impact");
                    block.TextAlignment = TextAlignment.Left;
                    block.Background = Brushes.Transparent;
                    block.Foreground = new SolidColorBrush(Colors.Cyan);
                    block.FontSize = 35;
                    block.Width = 400;
                    block.Margin = new Thickness(20, 15, 0, 0);
                    namesContainer.Children.Add(block);
                    count++;
                }
                
            });
        }
        private void Button_Back(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            StatisticsWindow window = new StatisticsWindow();
            window.Show();
            this.Close();
        }
    }

}
