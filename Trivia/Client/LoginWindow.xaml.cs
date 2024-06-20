using Newtonsoft.Json;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Client
{

    public partial class LoginWindow : Window
    {
        public LoginWindow()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;

        }

        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            var textBox = sender as TextBox;
            if (textBox != null && (textBox.Text == "Username"))
            {
                textBox.Text = "";
                textBox.Foreground = System.Windows.Media.Brushes.White;
            }
        }

        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            var textBox = sender as TextBox;
            if (textBox != null && string.IsNullOrWhiteSpace(textBox.Text))
            {
                if (textBox.Name == "UsernameTextBox")
                    textBox.Text = "Username";
                textBox.Foreground = System.Windows.Media.Brushes.Gray;
            }
        }

        private void PasswordTextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            var passwordBox = sender as PasswordBox;
            PasswordPlaceholderTextBox.Visibility = Visibility.Collapsed;
            passwordBox.Foreground = System.Windows.Media.Brushes.White;
        }

        private void PasswordTextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            var passwordBox = sender as PasswordBox;
            if (passwordBox != null && string.IsNullOrWhiteSpace(passwordBox.Password))
            {
                PasswordPlaceholderTextBox.Visibility = Visibility.Visible;
                passwordBox.Foreground = System.Windows.Media.Brushes.Gray;
            }
        }


        private void Button_Login(object sender, RoutedEventArgs e)
        {
            string username = UsernameTextBox.Text;
            string password = PasswordTextBox.Password;
            if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password) || UsernameTextBox.Text == "Username" || PasswordTextBox.Password == "Password")
            {
                // If either field is empty, display a message to prompt the user to enter both
                MessageBox.Show("Please enter both username and password.", "Login Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                // show it until send to server
                Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
                //need to serialize msg 
                Requests.LoginMessage loginMsg = new Requests.LoginMessage(username, password);
                string jsonLogin = JsonConvert.SerializeObject(loginMsg);

                byte[] bytes = Serializer.seriallizeRequest(jsonLogin, 200);
                newCommunicator.SendMsg(bytes);
                (int code, string content) = newCommunicator.ReceiveCodeAndResponse();

                if (code == 100 || code == 101)
                {
                    ErrorResponse resp = JsonConvert.DeserializeObject<ErrorResponse>(content);
                    MessageBox.Show(resp.ERROR, "Login error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                else
                {
                    ((App)Application.Current).SetHandleClosing(false);

                    MainMenuWindow window = new MainMenuWindow();
                    window.Show();
                    this.Close();
                }
            }

            // clear textboxes
            UsernameTextBox.Foreground = System.Windows.Media.Brushes.Gray;
            PasswordTextBox.Foreground = System.Windows.Media.Brushes.Gray;
            UsernameTextBox.Text = "Username";
            PasswordTextBox.Password = "";
        }

        private void Button_Back(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            MainWindow window = new MainWindow();
            window.Show();
            this.Close();
        }
    }
}
