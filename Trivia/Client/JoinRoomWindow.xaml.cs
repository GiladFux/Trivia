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
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        private Thread updateThread;

        public JoinRoomWindow()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;
            updateThread = new Thread(UpdateRooms);
            updateThread.Start();
           
        }
        private void Button_Back(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            MainMenuWindow window = new MainMenuWindow();
            updateThread.Abort();
            window.Show();
            this.Close();
        }
        private void UpdateRooms()
        {
            Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
            byte[] bytes = Serializer.seriallizeRequest("", 51); 
            while (true)
            {
                newCommunicator.SendMsg(bytes); // send get rooms request
                (int code, string content) = newCommunicator.ReceiveCodeAndResponse();
                if (code == 202)
                {
                    Dispatcher.Invoke(() => // allow access to the new thread
                    {
                        buttonContainer.Children.Clear(); // clear previous buttons
                        bool first = true;
                        int margin = 30;
                        Rooms Rooms = JsonConvert.DeserializeObject<Rooms>(content);
                        if (Rooms.rooms.Length == 0)
                        {
                            RoomsBorder.Visibility = Visibility.Collapsed;
                            NoRoomsTextBox.Visibility = Visibility.Visible;
                        }
                        else
                        {
                            RoomsBorder.Visibility = Visibility.Visible;
                            NoRoomsTextBox.Visibility = Visibility.Collapsed;
                        }
                        

                        foreach (Room r in Rooms.rooms)
                        {
                            if (!first)
                                margin = 15;
                            AddRoomButton(r.name, r.id, r.maxPlayers, r.numOfQuestionsInGame, r.timePerQuestion, margin);
                            first = false;
                        }
                    });
                }
                Thread.Sleep(3000); // Sleep for 3 seconds
            }

        }
        private void AddRoomButton(string roomName, uint id, uint maxPlayers, uint numOfQuestions, uint timePerQuestion, int margin)
        {
            Color color = (Color)ColorConverter.ConvertFromString("#937ADD");

            Button room = new Button();
            room.Background = new SolidColorBrush(color);
            TextBlock textBlock = new TextBlock();
            textBlock.Text = $"  {roomName}    {maxPlayers}🧍‍       {numOfQuestions}❓";
            textBlock.TextAlignment = TextAlignment.Left;
            room.Content = textBlock;

            room.HorizontalContentAlignment = HorizontalAlignment.Center; 
            room.VerticalAlignment = VerticalAlignment.Top; 

            room.Width = 250;
            room.Height = 50;
            room.HorizontalAlignment = HorizontalAlignment.Center; 
            room.Margin = new Thickness(0, margin, 0, 0); 
            room.Click += (sender, e) => joinRoom(sender, e, id);
            buttonContainer.Children.Add(room);
        }

        private void joinRoom(object sender, RoutedEventArgs e, uint id)
        {
            Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
            Requests.JoinRoomRequest req = new Requests.JoinRoomRequest(id);
            string jsonJoinRoom = JsonConvert.SerializeObject(req);

            byte[] bytes = Serializer.seriallizeRequest(jsonJoinRoom,53);
            updateThread.Abort();
            newCommunicator.SendMsg(bytes);
            (int code, string content) = newCommunicator.ReceiveCodeAndResponse();

            if (code == 202)
            {
                ((App)Application.Current).SetHandleClosing(false);
                RoomLobby joinRoom = new RoomLobby(id);
                joinRoom.Show();
                this.Close();      
            }
            else
            {
                ErrorResponse resp = JsonConvert.DeserializeObject<ErrorResponse>(content);
                MessageBox.Show(resp.ERROR, "Join room error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            
        }
    }
}
