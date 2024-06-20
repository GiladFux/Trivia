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
    /// Interaction logic for SignupWindow.xaml
    /// </summary>
    public partial class SignupWindow : Window
    {
        public SignupWindow()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;

        }
        

        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            var textBox = sender as TextBox;
            if (textBox != null && (textBox.Text == "Username" || textBox.Text == "Password" || textBox.Text == "Email"))
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
                else if (textBox.Name == "PasswordTextBox")
                    textBox.Text = "Password";
                else if (textBox.Name == "EmailTextBox")
                    textBox.Text = "Email";

                textBox.Foreground = System.Windows.Media.Brushes.Gray;
            }
        }
        private void Button_Back(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            MainWindow window = new MainWindow();
            window.Show();
            this.Close();
        }
        private void Button_Signup(object sender, RoutedEventArgs e)
        {
            string username = UsernameTextBox.Text;
            string password = PasswordTextBox.Text;
            string email = EmailTextBox.Text;
            if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password) || string.IsNullOrWhiteSpace(email) ||
        username == "Username" || password == "Password" || email == "Email")
            {
                // If either field is empty, display a message to prompt the user to enter both
                MessageBox.Show("Please enter username, password and email.", "Signup Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                // show it until send to server
                Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
                //TODO : seriallize msg and add checks if signup succeeded
                Requests.SignupMessage signupMsg = new Requests.SignupMessage(username, password, email);
                string jsonsignup = JsonConvert.SerializeObject(signupMsg);
                byte[] bytes = Serializer.seriallizeRequest(jsonsignup, 201);
                newCommunicator.SendMsg(bytes);
                (int code, string content) = newCommunicator.ReceiveCodeAndResponse();


                if (code == 100 || code == 101)
                {
                    ErrorResponse resp = JsonConvert.DeserializeObject<ErrorResponse>(content);
                    MessageBox.Show(resp.ERROR, "Signup error", MessageBoxButton.OK, MessageBoxImage.Error);
                    //show error msg
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
            EmailTextBox.Foreground = System.Windows.Media.Brushes.Gray;

            UsernameTextBox.Text = "Username";
            PasswordTextBox.Text = "Password";
            EmailTextBox.Text = "Email";

        }

    }
}
