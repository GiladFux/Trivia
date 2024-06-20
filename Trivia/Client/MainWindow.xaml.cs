using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;

        }
        private void Button_Login(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            LoginWindow window = new LoginWindow();
            window.Show();
            this.Close();
        }

        private void Button_SignUp(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);
            SignupWindow window = new SignupWindow();
            window.Show();
            this.Close();
        }

        private void Button_Quit(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
