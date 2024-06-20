using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading; // Add this namespace
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Newtonsoft.Json;
using System.Windows.Threading;
using System.Diagnostics;
using System.ComponentModel;

namespace Client
{
    /// <summary>
    /// Interaction logic for GameResultsWindow.xaml
    /// </summary>
    
    public partial class GameResultsWindow : Window
    {
        private bool _isAdmin;
        public GameResultsWindow(bool isAdmin = false)
        {
            _isAdmin = isAdmin;
            InitializeComponent();
            Closing += HandleClosing;
            Thread leaderBoard = new Thread(ShowLeaderBoard);
            leaderBoard.Start();

        }
        private int count = 1;
        private bool handleClosing = true;
        Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
        private void ShowLeaderBoard()
        {
            
            byte[] bytes = Serializer.seriallizeRequest("", 63); // send game results request
            newCommunicator.SendMsg(bytes);
            (int code, string response) = newCommunicator.ReceiveCodeAndResponse();
            GameResults gameResults = JsonConvert.DeserializeObject<GameResults>(response);
            foreach (GameResult result in gameResults.Results)
            {
                CreateTextBox(result);
            }
            try
            {
                Dispatcher.Invoke(() =>
                {
                    if (gameResults.isWinner) // show video
                    {
                        WinnerVideo.Source = new Uri("clideo_editor_dfc44eb92d1d4cb6b200f59ff31969e8.mp4", UriKind.Relative);
                        video.Visibility = Visibility.Visible;
                        win1.Visibility = Visibility.Visible;
                        win2.Visibility = Visibility.Visible;
                        WinnerVideo.Play();
                    }
                });
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }


        }
        private void Button_Back(object sender, RoutedEventArgs e)
        {
            handleClosing = false;
            byte[] bytes = Serializer.seriallizeRequest("", 46);
            newCommunicator.SendMsg(bytes);
            (int code, string resp) = newCommunicator.ReceiveCodeAndResponse();
            MainMenuWindow window = new MainMenuWindow();
            window.Show();
            this.Close();
        }
        public void HandleClosing(object sender, CancelEventArgs e)
        {
            if (handleClosing)
            {
                CustomMessageBox customMessageBox = new CustomMessageBox
                {
                    Owner = (Window)sender
                };
                customMessageBox.ShowDialog();

                if (!customMessageBox.Result)
                {
                    e.Cancel = true;
                }
                else
                {
                    byte[] bytes = Serializer.seriallizeRequest("", 46); // send leave results request
                    newCommunicator.SendMsg(bytes);
                    newCommunicator.endCommunication();
                    Environment.Exit(0);
                }
            }
            handleClosing = true;
        }
        private void CreateTextBox(GameResult result)
        {
            Dispatcher.Invoke(() =>
            {
                TextBlock block;
                block = new TextBlock();

                block.Text = count + ". " + result.username + "  Correct Answers -  " + result.correctAnswerCount + 
                "✔️  Wrong Answers -  " + result.wrongAnswerCount + "❌  Average Time -  " + result.averageAnswerTime.ToString("F2") +
                "🕒  Score -  " + result.score + " 🎯"; 


                block.FontFamily = new FontFamily("Impact");
                block.TextAlignment = TextAlignment.Left;
                block.Background = Brushes.Transparent;
                block.Foreground = new SolidColorBrush(Colors.Cyan);
                block.FontSize = 22;
                block.Width = Width;
                block.Margin = new Thickness(0, count * 40,0, 0);
                namesContainer.Children.Add(block);
                count++;
            });
        }
    }
}
