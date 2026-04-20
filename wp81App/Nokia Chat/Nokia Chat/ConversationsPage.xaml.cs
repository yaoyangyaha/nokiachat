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
    public sealed partial class ConversationsPage : Page
    {
        private readonly ObservableCollection<ConversationItem> _items = new ObservableCollection<ConversationItem>();

        public ConversationsPage()
        {
            this.InitializeComponent();
            this.NavigationCacheMode = NavigationCacheMode.Required;
            ConversationsListView.ItemsSource = _items;
        }

        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {
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
            try
            {
                var api = CreateApi();
                var list = await api.GetConversationsAsync();
                _items.Clear();
                foreach (var c in list) _items.Add(c);
            }
            catch (Exception ex)
            {
                await new MessageDialog("加载会话失败: " + ex.Message).ShowAsync();
            }
        }

        private async void RefreshButton_Click(object sender, RoutedEventArgs e)
        {
            await LoadAsync();
        }

        private void ConversationsListView_ItemClick(object sender, ItemClickEventArgs e)
        {
            var item = e.ClickedItem as ConversationItem;
            if (item == null) return;
            Frame.Navigate(typeof(MessagesPage), item);
        }
    }
}

