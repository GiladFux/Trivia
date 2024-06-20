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
using System.Security.Cryptography;

namespace Client
{
    /// <summary>
    /// Interaction logic for GameWindow.xaml
    /// </summary>
    public partial class GameWindow : Window
    {
        
        private Stopwatch _stopwatch = new Stopwatch();
        private Button ans1;
        private Button ans2;
        private Button ans3;
        private Button ans4;
        private TextBlock correctAmountText;
        private TextBlock questionText;
        private TextBlock questionAmountText;
        private DispatcherTimer timer;
        private float timeLeft;
        private uint correctAmount;
        private uint _timePerQuestion;
        private uint _amountOfQuestions;
        private bool isSubmit = false;
        private bool _isAdmin = false;
        public bool isNewQuestion = true;
        private Communicator newCommunicator = Communicator.GetInstance("127.0.0.1", 6969);

        public GameWindow(uint timePerQuestion, uint amountOfQuestions, bool isAdmin = false)
        {
            InitializeComponent();
            _isAdmin = isAdmin;
            Closing += ((App)Application.Current).HandleClosing;
            _timePerQuestion = timePerQuestion;
            timeLeft = timePerQuestion;
            _amountOfQuestions = amountOfQuestions; 
            if(!newCommunicator.isSocketEmpty())
            {
                newCommunicator.ReceiveCodeAndResponse(); // clear buffer
            }
            InitializeComponent();
            Thread HandleThread = new Thread(HandleQuestions);
            HandleThread.Start();
        }
        public void HandleClosing(object sender, CancelEventArgs e)
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
                byte[] bytes = Serializer.seriallizeRequest("", 60); // send leave game request
                newCommunicator.SendMsg(bytes);
                newCommunicator.endCommunication();
                Environment.Exit(0);
            }
        
        }
        private void HandleQuestions()
        {
            bool endLoop = false;
            while (!endLoop)
            {
                if (isNewQuestion)
                {
                    isNewQuestion = false;
                    int code;
                    Question newQuestion;
                    (code, newQuestion) = getQuestion();
                    if (code == 1) // there is a question available
                    {
                        AddAnswersButtonsAndQuestionText(newQuestion.Answers, newQuestion.QuestionString);
                        Dispatcher.Invoke(() =>
                        {
                            StartTimer(_timePerQuestion);
                        });
                    }
                    else
                    {
                        Dispatcher.Invoke(() =>
                        {

                            endLoop = true;
                            GameResultsWindow results = new GameResultsWindow(_isAdmin);
                            results.Show();
                            ((App)Application.Current).SetHandleClosing(false);
                            this.Close();
                        });


                    }
                }
            }

        }

        private void StartTimer(uint seconds)
        {
            
            timeLeft = seconds;
            TimerTextBlock.Text = timeLeft.ToString() + " ⌛";
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += TimerTick;
            timer.Start();
            _stopwatch.Start();
        }
        private void TimerTick(object sender, EventArgs e)
        {
            if (timeLeft > 0)
            {
                timeLeft--;
                TimerTextBlock.Text = timeLeft.ToString() + " ⌛";
            }
            else
            {
                _stopwatch.Stop();
                timer.Stop();
                TimerTextBlock.Text = "Time's up!";
                ans1.IsEnabled = false;
                ans2.IsEnabled = false;
                ans3.IsEnabled = false;
                ans4.IsEnabled = false;
                if(!isSubmit)
                {
                    SubmitAnswerRequest req;
                    req.answerId = 5;//set invalid id to ensure wrong answer
                    req.time = _timePerQuestion;
                    string jsonSubmit = JsonConvert.SerializeObject(req);
                    sendMsg(62, jsonSubmit);
                    newCommunicator.ReceiveCodeAndResponse();
                }
                isSubmit = false;
                timeLeft = _timePerQuestion;
                Task.Delay(1000).ContinueWith(_ => ResetAnswerButtons());
                Thread.Sleep(1000);
                isNewQuestion = true;
                _amountOfQuestions--;
                _stopwatch.Reset();
            }
        }
        private void AddAnswersButtonsAndQuestionText(Dictionary<uint, string> answers, string question)
        {
            Dispatcher.Invoke(() =>
            {
            correctAmountText =  CreateTextBlock(questionAmountText, correctAmount.ToString() + " ✔️", new SolidColorBrush(Colors.LimeGreen), 30, new Thickness(1150,0, 0, 200), 60, new FontFamily("Garamond"), false);
            questionAmountText = CreateTextBlock(questionAmountText, _amountOfQuestions.ToString() + " ❔", new SolidColorBrush(Colors.LightCyan), 30, new Thickness(1150, 30, 0, 200), 60, new FontFamily("Garamond"), false);
            questionText = CreateTextBlock(questionText, question, new SolidColorBrush(Colors.Violet), 40, new Thickness(140, 120, 0, 0), 1050 , new FontFamily("Impact"));
            int screenWidth = (int)SystemParameters.PrimaryScreenWidth;
            int screenHeight = (int)SystemParameters.PrimaryScreenHeight;
            ans1 = CreateAnswerButton(0, answers[0], screenWidth * 0.12, screenHeight * 0.54);
            ans2 = CreateAnswerButton(1, answers[1], screenWidth * 0.55, screenHeight * 0.54);
            ans3 = CreateAnswerButton(2, answers[2], screenWidth * 0.12, screenHeight * 0.75);
            ans4 = CreateAnswerButton(3, answers[3], screenWidth * 0.55, screenHeight * 0.75);
            });
        }
        private TextBlock CreateTextBlock(TextBlock block ,string content, SolidColorBrush color , int fontSize , Thickness Place , int Width ,FontFamily family, bool wrap = true)
        {
            Dispatcher.Invoke(() =>
            {
            block = new TextBlock();
            block.Text = content;
            block.FontFamily = new FontFamily("Impact");
            block.TextAlignment = TextAlignment.Left;
            block.Background = Brushes.Transparent;
            block.Foreground = color;
            block.FontSize = fontSize;
            if(wrap) 
                block.TextWrapping = TextWrapping.Wrap;
            block.Width = Width;
            block.Margin = Place;
            canvas.Children.Add(block);
            });

            return block;
        }
        private Button CreateAnswerButton(uint id, string content, double left, double top)
        {
            Button button = new Button
            {
                Width = 420,
                Height = 60,
                Content = content,
                FontFamily = new FontFamily("Impact"),
                Foreground = new SolidColorBrush(Colors.White),
                HorizontalContentAlignment = HorizontalAlignment.Center,
                VerticalContentAlignment = VerticalAlignment.Center,
                Background = Brushes.Transparent,
                BorderBrush = null, 
                FontSize = 25,
            };
            Canvas.SetLeft(button, left);
            Canvas.SetTop(button, top);
            button.Style = (Style)FindResource("CustomButtonStyle");
            button.Click += (sender, e) => SubmitAnswer(button,id);
            canvas.Children.Add(button);

            return button;
        }
        private (int, Question) getQuestion()
        {
            if (!newCommunicator.isSocketEmpty())
            {
                newCommunicator.ReceiveCodeAndResponse();
            }
            sendMsg(61, "");
            (int code, string response) = newCommunicator.ReceiveCodeAndResponse();
            Question newQuestion = JsonConvert.DeserializeObject<Question>(response);
            return (code, newQuestion);
            
        }
        private void ResetAnswerButtons()
        {
            Dispatcher.Invoke(() =>
            {
                ResetButton(ans1);
                ResetButton(ans2);
                ResetButton(ans3);
                ResetButton(ans4);
                canvas.Children.Remove(questionText);
                canvas.Children.Remove(questionAmountText);
                canvas.Children.Remove(correctAmountText);
            });
        }

        private void ResetButton(Button button)
        {
            if (button != null)
            {
                button.Foreground = new SolidColorBrush(Colors.White);
                button.IsEnabled = true;
                button.Background = Brushes.Transparent; 
                button.Content = string.Empty;
            }
        }
        private void SubmitAnswer(Button button , uint id)
        {
            _stopwatch.Stop();
            isSubmit = true;
            SubmitAnswerRequest req;
            req.answerId = id;
            req.time = (float)_stopwatch.Elapsed.TotalSeconds;
            _stopwatch.Reset();
            string jsonSubmit = JsonConvert.SerializeObject(req);
            if (!newCommunicator.isSocketEmpty())
            {
                newCommunicator.ReceiveCodeAndResponse();
            }
            sendMsg(62, jsonSubmit);
            (int code, string response) = newCommunicator.ReceiveCodeAndResponse();
            SubmitAnswerResponse ans = JsonConvert.DeserializeObject<SubmitAnswerResponse>(response);
            if(id == ans.CorrectID)
            {
                button.Foreground = new SolidColorBrush(Colors.Lime);
                correctAmount++;
            }
            else
            {
                button.Foreground = new SolidColorBrush(Colors.Red);
                switch(ans.CorrectID)
                {
                    case 0:
                        ans1.Foreground = new SolidColorBrush(Colors.Lime);
                        break;
                    case 1:
                        ans2.Foreground = new SolidColorBrush(Colors.Lime);
                        break;
                    case 2:
                        ans3.Foreground = new SolidColorBrush(Colors.Lime);
                        break;
                    case 3:
                        ans4.Foreground = new SolidColorBrush(Colors.Lime);
                        break;
                }
                
            }
            
            ans1.IsEnabled = false;
            ans2.IsEnabled = false;
            ans3.IsEnabled = false;
            ans4.IsEnabled = false;

        }
        private void sendMsg(int id , string content)
        {
            byte[] bytes = Serializer.seriallizeRequest(content, id);
            newCommunicator.SendMsg(bytes);

        }
    }
}
