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
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        public CreateRoomWindow()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;

        }
        private void isNumber(object sender, TextCompositionEventArgs e)
        {
            foreach (char c in e.Text)
            {
                if (!char.IsDigit(c))
                {
                    e.Handled = true;
                    break;
                }
            }
        }
        private void TextBox_GotFocus(object sender, RoutedEventArgs e) // check if should focus the text box
        {
            var textBox = sender as TextBox;
            if (textBox != null && (textBox.Text == "Room name" || textBox.Text == "Players amount (max 10)" || textBox.Text == "Time for question (5 - 30 seconds)" || textBox.Text == "Questions amount (max 50)"))
            {
                textBox.Text = "";
                textBox.Foreground = System.Windows.Media.Brushes.White;
            }
        }
        private void TextBox_LostFocus(object sender, RoutedEventArgs e) // bring back the place holder text
        {
            var textBox = sender as TextBox;
            if (textBox != null && string.IsNullOrWhiteSpace(textBox.Text))
            {
                if (textBox.Name == "RoomNameTextBox")
                    textBox.Text = "Room name";
                else if (textBox.Name == "PlayersAmountTextBox")
                    textBox.Text = "Players amount (max 10)";
                else if (textBox.Name == "QuestionTimeTextBox")
                    textBox.Text = "Time for question (5 - 30 seconds)";
                else if (textBox.Name == "QuestionCountTextBox")
                    textBox.Text = "Questions amount (max 50)";

                textBox.Foreground = System.Windows.Media.Brushes.Gray;
            }
        }
        private void Button_Back(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            MainMenuWindow window = new MainMenuWindow();
            window.Show();
            this.Close();
        }
        private void Button_Create(object sender, RoutedEventArgs e)
        {
            string roomName = RoomNameTextBox.Text;
            string questionTimeText = QuestionTimeTextBox.Text;
            string playersAmountText = PlayersAmountTextBox.Text;
            string questionCountText = QuestionCountTextBox.Text;

            if (string.IsNullOrWhiteSpace(RoomNameTextBox.Text) || string.IsNullOrWhiteSpace(QuestionTimeTextBox.Text) ||
                string.IsNullOrWhiteSpace(PlayersAmountTextBox.Text) || RoomNameTextBox.Text == "Room name" || QuestionTimeTextBox.Text == "Time for question (5 - 30 seconds)"
                || PlayersAmountTextBox.Text == "Players amount (max 10)" || QuestionCountTextBox.Text == "Questions amount(max 50)")
            {
                MessageBox.Show("Please provide all fields", "Create room error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;

            }
            int questionTime = int.Parse(questionTimeText);
            int playersAmount = int.Parse(playersAmountText);
            int questionCount = int.Parse(questionCountText);
            // should send request to server
            Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
            Requests.CreateRoomRequest createRoomRequest = new Requests.CreateRoomRequest(roomName, playersAmount, questionCount, questionTime);
            string jsoncreateRoom = JsonConvert.SerializeObject(createRoomRequest);
            byte[] bytes = Serializer.seriallizeRequest(jsoncreateRoom, 50);
            newCommunicator.SendMsg(bytes);
            (int code, string content) = newCommunicator.ReceiveCodeAndResponse();



            if (code == 100 || code == 101)
            {
                ErrorResponse resp = JsonConvert.DeserializeObject<ErrorResponse>(content);
                MessageBox.Show(resp.ERROR, "Create room error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                ((App)Application.Current).SetHandleClosing(false);
                createRoomResponse resp = JsonConvert.DeserializeObject<createRoomResponse>(content);

                RoomLobby window = new RoomLobby(resp.ID, true); // change to room window
                window.Show();
                this.Close();

            }
        }
    }
}
