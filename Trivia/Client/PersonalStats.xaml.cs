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
    /// Interaction logic for PersonalStats.xaml
    /// </summary>
    public partial class PersonalStats : Window
    {
        Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
        public PersonalStats()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;
            Thread PersonalStats = new Thread(showPersonalStats);
            PersonalStats.Start();
        }
        private void Button_Back(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            StatisticsWindow window = new StatisticsWindow();
            window.Show();
            this.Close();
        }
        private void showPersonalStats()
        {
            
            byte[] bytes = Serializer.seriallizeRequest("", 54);
            newCommunicator.SendMsg(bytes);
            (int code, string response) = newCommunicator.ReceiveCodeAndResponse();
            PersonalStatistics personalStats = JsonConvert.DeserializeObject<PersonalStatistics>(response);
            CreateStatsText(personalStats);
        }
        private void CreateStatsText(PersonalStatistics stats)
        {
            Dispatcher.Invoke(() =>
            {
                double averageAnswerTime = double.Parse(stats.Statistics[0].Split(':')[1].Trim());
                int totalAnswers = int.Parse(stats.Statistics[1].Split(':')[1].Trim());
                int correctAnswers = int.Parse(stats.Statistics[2].Split(':')[1].Trim());
                int totalGamesPlayed = int.Parse(stats.Statistics[3].Split(':')[1].Trim());
                int totalScore = int.Parse(stats.Statistics[4].Split(':')[1].Trim());
                string username = stats.Statistics[5].Split(':')[1].Trim();
                int wrongAnswers = totalAnswers - correctAnswers;

                CreateTitleText(username);
                TextBlock block;
                block = new TextBlock();

                block.Text = "Correct Answers -  " + correctAnswers + "✔️\n" +
                "Wrong Answers -  " + wrongAnswers + "❌\n" +
                "Average Time -  " + averageAnswerTime.ToString("F2")+
                "🕒\nScore -  " + totalScore + " 🎯";


                block.FontFamily = new FontFamily("Impact");
                block.TextAlignment = TextAlignment.Left;
                block.Background = Brushes.Transparent;
                block.Foreground = new SolidColorBrush(Colors.Cyan);
                block.FontSize = 35;
                block.Width = 400;
                block.Margin = new Thickness(200, 400 , 0, 0);
                canvas.Children.Add(block);
            });
        }
        private void CreateTitleText(string username)
        {
            TextBlock Title;
            Title = new TextBlock();
            Title.Text = username +"'s Personal Stats";
            Title.FontFamily = new FontFamily("Impact");
            Title.TextAlignment = TextAlignment.Left;
            Title.Background = Brushes.Transparent;
            Title.Foreground = new SolidColorBrush(Colors.Violet);
            Title.FontSize = 55;
            Title.Width = 700;
            Title.Margin = new Thickness(450, 200, 0, 0);
            canvas.Children.Add(Title);


        }
    }
}
