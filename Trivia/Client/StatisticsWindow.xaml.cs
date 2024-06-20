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
    /// Interaction logic for StatisticsWindow.xaml
    /// </summary>
    public partial class StatisticsWindow : Window
    {
        public StatisticsWindow()
        {
            InitializeComponent();
            Closing += ((App)Application.Current).HandleClosing;

        }

        private void Button_Leaderboard(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            Leaderboard window = new Leaderboard();
            window.Show();
            this.Close();
        }
        private void Button_PersonalStats(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            PersonalStats window = new PersonalStats();
            window.Show();
            this.Close();
        }
        private void Button_Back(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).SetHandleClosing(false);

            MainMenuWindow window = new MainMenuWindow();
            window.Show();
            this.Close();
        }


    }
}
