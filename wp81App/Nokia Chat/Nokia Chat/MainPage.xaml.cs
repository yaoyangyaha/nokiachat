using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Popups;
using Nokia_Chat.Services;

// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=391641 上有介绍

namespace Nokia_Chat
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            this.NavigationCacheMode = NavigationCacheMode.Required;
        }

        /// <summary>
        /// 在此页将要在 Frame 中显示时进行调用。
        /// </summary>
        /// <param name="e">描述如何访问此页的事件数据。
        /// 此参数通常用于配置页。</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            ServerTextBox.Text = AppSettings.GetServerHost();
            PortTextBox.Text = AppSettings.GetServerPort().ToString();
            ApiKeyTextBox.Text = AppSettings.GetApiKey();
        }

        private void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            var host = (ServerTextBox.Text ?? "").Trim();
            int port;
            if (!int.TryParse((PortTextBox.Text ?? "").Trim(), out port)) port = 8000;
            var apiKey = (ApiKeyTextBox.Text ?? "").Trim();

            AppSettings.SetServerHost(host);
            AppSettings.SetServerPort(port);
            AppSettings.SetApiKey(apiKey);

            StatusTextBlock.Text = "已保存: " + host + ":" + port;
        }

        private async void TestButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                SaveButton_Click(null, null);
                var api = new NokiachatApi(AppSettings.GetServerHost(), AppSettings.GetServerPort(), AppSettings.GetApiKey());
                var result = await api.PingAsync();
                StatusTextBlock.Text = "连接成功: " + (result ?? "").Trim();
            }
            catch (Exception ex)
            {
                await new MessageDialog("连接失败: " + ex.Message).ShowAsync();
            }
        }

        private async void WeChatButton_Click(object sender, RoutedEventArgs e)
        {
            await new MessageDialog("微信消息暂未实现").ShowAsync();
        }

        private void QqButton_Click(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(ConversationsPage));
        }
    }
}
