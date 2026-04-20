using System;
using System.Collections.ObjectModel;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Nokia_Chat.Models;
using Nokia_Chat.Services;

namespace Nokia_Chat
{
    public sealed partial class MessagesPage : Page
    {
        private readonly ObservableCollection<MessageItem> _items = new ObservableCollection<MessageItem>();
        private ConversationItem _conv;

        public MessagesPage()
        {
            this.InitializeComponent();
            MessagesListView.ItemsSource = _items;
        }

        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {
            _conv = e.Parameter as ConversationItem;
            if (_conv != null)
            {
                ConversationTitleTextBlock.Text = _conv.Name ?? "";
            }
            await LoadAsync();
        }

        private NokiachatApi CreateApi()
        {
            var host = AppSettings.GetServerHost();
            var port = AppSettings.GetServerPort();
            var apiKey = AppSettings.GetApiKey();
            return new NokiachatApi(host, port, apiKey);
        }

        private async System.Threading.Tasks.Task LoadAsync()
        {
            if (_conv == null) return;
            try
            {
                var api = CreateApi();
                var list = await api.GetMessagesAsync(_conv.ConvId, 0, 50);
                _items.Clear();
                foreach (var m in list) _items.Add(m);
            }
            catch (Exception ex)
            {
                await new MessageDialog("加载消息失败: " + ex.Message).ShowAsync();
            }
        }

        private async void RefreshButton_Click(object sender, RoutedEventArgs e)
        {
            await LoadAsync();
        }

        private async void SendButton_Click(object sender, RoutedEventArgs e)
        {
            if (_conv == null) return;
            var text = (SendTextBox.Text ?? "").Trim();
            if (text.Length == 0) return;

            try
            {
                SendTextBox.IsEnabled = false;
                var api = CreateApi();
                await api.SendTextAsync(_conv.ConvId, text);
                SendTextBox.Text = "";
                await LoadAsync();
            }
            catch (Exception ex)
            {
                await new MessageDialog("发送失败: " + ex.Message).ShowAsync();
            }
            finally
            {
                SendTextBox.IsEnabled = true;
            }
        }
    }
}
