using System.Windows;

namespace Client
{
    public partial class CustomMessageBox : Window
    {
        public bool Result { get; private set; }

        public CustomMessageBox()
        {
            InitializeComponent();
        }

        private void YesButton_Click(object sender, RoutedEventArgs e)
        {
            Result = true;
            this.Close();
        }

        private void NoButton_Click(object sender, RoutedEventArgs e)
        {
            Result = false;
            this.Close();
        }
    }
}
