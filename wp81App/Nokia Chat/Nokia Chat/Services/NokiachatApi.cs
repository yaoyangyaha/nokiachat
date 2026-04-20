using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Threading.Tasks;
using Nokia_Chat.Models;

namespace Nokia_Chat.Services
{
    public class NokiachatApi
    {
        private readonly string _baseUrl;
        private readonly string _apiKey;

        public NokiachatApi(string serverHost, int port, string apiKey)
        {
            if (string.IsNullOrWhiteSpace(serverHost))
                throw new ArgumentException("serverHost is empty");

            serverHost = serverHost.Trim();
            if (serverHost.StartsWith("http://", StringComparison.OrdinalIgnoreCase) ||
                serverHost.StartsWith("https://", StringComparison.OrdinalIgnoreCase))
            {
                _baseUrl = serverHost.TrimEnd('/');
            }
            else
            {
                _baseUrl = "http://" + serverHost + ":" + port;
            }

            _apiKey = apiKey ?? "";
        }

        private HttpClient CreateClient()
        {
            var client = new HttpClient();
            if (!string.IsNullOrEmpty(_apiKey))
            {
                client.DefaultRequestHeaders.Add("X-Api-Key", _apiKey);
            }
            return client;
        }

        public async Task<string> PingAsync()
        {
            using (var client = CreateClient())
            {
                var resp = await client.GetAsync(_baseUrl + "/v1/ping");
                resp.EnsureSuccessStatusCode();
                return await resp.Content.ReadAsStringAsync();
            }
        }

        public async Task<List<ConversationItem>> GetConversationsAsync()
        {
            using (var client = CreateClient())
            {
                var resp = await client.GetAsync(_baseUrl + "/v1/qq/conversations");
                resp.EnsureSuccessStatusCode();
                var body = await resp.Content.ReadAsStringAsync();
                return ParseConversations(body);
            }
        }

        public async Task<List<MessageItem>> GetMessagesAsync(string convId, long afterId = 0, int limit = 50)
        {
            using (var client = CreateClient())
            {
                var url = _baseUrl + "/v1/qq/messages?conv_id=" + Uri.EscapeDataString(convId ?? "") +
                          "&after_id=" + afterId + "&limit=" + limit;
                var resp = await client.GetAsync(url);
                resp.EnsureSuccessStatusCode();
                var body = await resp.Content.ReadAsStringAsync();
                return ParseMessages(body);
            }
        }

        public async Task SendTextAsync(string convId, string text)
        {
            using (var client = CreateClient())
            {
                var form = new FormUrlEncodedContent(new Dictionary<string, string>
                {
                    {"conv_id", convId ?? ""},
                    {"text", text ?? ""}
                });
                var resp = await client.PostAsync(_baseUrl + "/v1/qq/send_text", form);
                resp.EnsureSuccessStatusCode();
            }
        }

        private static List<ConversationItem> ParseConversations(string tsv)
        {
            var list = new List<ConversationItem>();
            if (string.IsNullOrEmpty(tsv)) return list;

            var lines = tsv.Split(new[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (var raw in lines)
            {
                var line = raw.TrimEnd('\r');
                var fields = line.Split('\t');
                if (fields.Length < 5) continue;

                long lastTs;
                int unread;
                long.TryParse(fields[2], out lastTs);
                int.TryParse(fields[4], out unread);

                list.Add(new ConversationItem
                {
                    ConvId = fields[0],
                    Name = fields[1],
                    LastTs = lastTs,
                    LastPreview = fields[3],
                    Unread = unread
                });
            }
            return list;
        }

        private static List<MessageItem> ParseMessages(string tsv)
        {
            var list = new List<MessageItem>();
            if (string.IsNullOrEmpty(tsv)) return list;

            var lines = tsv.Split(new[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (var raw in lines)
            {
                var line = raw.TrimEnd('\r');
                var fields = line.Split('\t');
                if (fields.Length < 5) continue;

                long msgId;
                long ts;
                long.TryParse(fields[0], out msgId);
                long.TryParse(fields[1], out ts);

                list.Add(new MessageItem
                {
                    MsgId = msgId,
                    Ts = ts,
                    Sender = fields[2],
                    MsgType = fields[3],
                    Content = fields[4]
                });
            }
            return list;
        }
    }
}

