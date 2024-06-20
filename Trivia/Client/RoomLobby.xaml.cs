using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Threading;
using System.Net.Sockets;
using System.ComponentModel;
using System.Windows.Documents;

namespace Client
{
    public partial class RoomLobby : Window
    {
        bool finishLoop = false;
        private Communicator newCommunicator;
        private Thread getStateThread;
        private Thread listeningThread;
        private uint questionTimeLimit;
        private uint amountOfQuestions;
        private bool _isAdmin;
        private bool _handleClosing = true;
        public RoomLobby(uint id, bool isAdmin = false)
        {
            _isAdmin = isAdmin;
            InitializeComponent();
            if (isAdmin)
            {
                AddAdminButtons();
            }
            else
            {
                AddLeaveButton();
            }
            Closing += HandleClosing;

            getStateThread = new Thread(GetState);
            getStateThread.Start();
            listeningThread = new Thread(HandleServerResponse);
            listeningThread.Start();
        }

        public void SetHandleClosing(bool enable)
        {
            _handleClosing = enable;
        }
        public void HandleClosing(object sender, CancelEventArgs e)
        {
            if (_handleClosing)
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
                    Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
                    if (_isAdmin)
                    {
                        byte[] bytes = Serializer.seriallizeRequest("", 40);
                        newCommunicator.SendMsg(bytes);
                    }
                    else
                    {
                        byte[] bytes = Serializer.seriallizeRequest("", 44);
                        newCommunicator.SendMsg(bytes);
                    }
                    newCommunicator.endCommunication();
                    Environment.Exit(0);
                }
            }
            _handleClosing = true;


        }
        private void GetState()
        {
            while (!finishLoop)
            {
                newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
                byte[] bytes = Serializer.seriallizeRequest("", 42);
                newCommunicator.SendMsg(bytes);
                Thread.Sleep(3000); // Sleep for 3 seconds
            }
        }

        private void HandleServerResponse()
        {
            while (!finishLoop)
            {
                newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
                {
                    try
                    {
                        (int code, string response) = newCommunicator.ReceiveCodeAndResponse();
                        switch (code)
                        {
                            
                            case 41:
                                Dispatcher.Invoke(() =>
                                { 
                                SetHandleClosing(false);
                                getStateThread.Abort();
                                GameWindow window = new GameWindow(questionTimeLimit,amountOfQuestions,_isAdmin);
                                window.Show();
                                finishLoop = true;
                                this.Close();
                                });
                                break;
                            case 42:
                                Dispatcher.Invoke(() =>
                                {
                                    RoomState state = JsonConvert.DeserializeObject<RoomState>(response);
                                    questionTimeLimit = state.AnswerTimeOut;
                                    amountOfQuestions = state.QuestionCount;
                                    resetText(state);
                                });
                                break;
                            case 40:
                                Dispatcher.Invoke(() =>
                                {
                                    SetHandleClosing(false);
                                    finishLoop = true;
                                    leaveRoom();

                                });
                                break;
                            case 45:
                                Dispatcher.Invoke(() =>
                                {
                                    SetHandleClosing(false);
                                    finishLoop = true;
                                    leaveRoom();
                                });
                                break;
                            case 43: 
                            default:
                                break;
                        }

                    }
                    catch (SocketException ex)
                    {
                        MessageBox.Show($"Socket error occurred: {ex.Message}");
                        System.Environment.Exit(0);
                    }
                    catch (ThreadAbortException ex)
                    {
                        //When ever thread is getting abort its throwing an exception
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"error: {ex.Message}");
                        System.Environment.Exit(0);
                    }
                    
                }
            }
            
        }
        

        private void resetText(RoomState state)
        {
            bool first = true;
            textContainer.Children.Clear(); // Clear previous text
            foreach (string p in state.Players)
            {
                if (first)
                    AddPlayerText(p, 30, true);
                else
                    AddPlayerText(p, 15);
                first = false;
            }
        }
        private void leaveRoom()
        {
            ((App)Application.Current).SetHandleClosing(false);
            MainMenuWindow window = new MainMenuWindow();
            window.Show();
            getStateThread.Abort();
            this.Close();

        }
        private void AddPlayerText(string name, int margin, bool admin = false)
        {
            TextBlock textBlock = new TextBlock();
            if (admin)
            {
                textBlock.Text = name + "👑";
                textBlock.FontWeight = FontWeights.Bold;
                textBlock.Foreground = Brushes.Yellow;
            }
            else
            {
                textBlock.Text = name;
                textBlock.Foreground = Brushes.White;
            }
            textBlock.TextAlignment = TextAlignment.Left;
            textBlock.Background = Brushes.Transparent;
            textBlock.FontSize = 20;
            textBlock.FontFamily = new FontFamily("Roboto");
            textBlock.Margin = new Thickness(0, margin, 0, 0);
            textBlock.Background = Brushes.Transparent;

            textContainer.Children.Add(textBlock);
        }

        private void AddAdminButtons()
        {
            Button closeGameButton = new Button
            {
                Content = "Close Game",
                Margin = new Thickness(5, 0, 0, 15),
                HorizontalAlignment = HorizontalAlignment.Center,
                Background = new SolidColorBrush(Colors.Red),
                Style = (Style)FindResource("MainMenuButtonStyle"),
                Cursor = Cursors.Hand

            };
            Button startGameButton = new Button
            {
                Content = "Start Game",
                Margin = new Thickness(0, 0, 5, 15),
                HorizontalAlignment = HorizontalAlignment.Center,
                Background = new SolidColorBrush(Colors.LightGreen),
                Style = (Style)FindResource("MainMenuButtonStyle"),
                Cursor = Cursors.Hand

            };

            closeGameButton.Click += CloseGameButton_Click;
            startGameButton.Click += StartGameButton_Click;
            adminButtonPanel.Children.Add(startGameButton);
            adminButtonPanel.Children.Add(closeGameButton);
        }

        private void AddLeaveButton()
        {

            Button leaveGameButton = new Button
            {
                Content = "Leave Game",
                Margin = new Thickness(0,0, 0, 15),
                HorizontalAlignment = HorizontalAlignment.Right,
                Background = new SolidColorBrush(Colors.Red),
                Style = (Style)FindResource("MainMenuButtonStyle"),
                Cursor = Cursors.Hand

            };

            leaveButtonPanel.Children.Add(leaveGameButton);
            leaveGameButton.Click += LeaveGameButton_Click;
        }

        private void CloseGameButton_Click(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);
            byte[] bytes = Serializer.seriallizeRequest("", 40);
            newCommunicator.SendMsg(bytes);
        }

        private void LeaveGameButton_Click(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);
            byte[] bytes = Serializer.seriallizeRequest("", 43);
            newCommunicator.SendMsg(bytes);
        }
        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] bytes = Serializer.seriallizeRequest("", 41);
            newCommunicator.SendMsg(bytes);
        }
    }
}
