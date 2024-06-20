using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private bool handleClosing = true;


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
                    Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);
                    newCommunicator.endCommunication();
                    Environment.Exit(0);
                }
            }
            handleClosing = true;
        }
        public void SetHandleClosing(bool enable)
        {
            handleClosing = enable;
        }

    }
}
