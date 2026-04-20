using Windows.Storage;

namespace Nokia_Chat.Services
{
    public static class AppSettings
    {
        private const string ServerHostKey = "server_host";
        private const string ServerPortKey = "server_port";
        private const string ApiKeyKey = "api_key";

        public static string GetServerHost()
        {
            object v;
            if (ApplicationData.Current.LocalSettings.Values.TryGetValue(ServerHostKey, out v))
            {
                return v as string ?? "";
            }
            return "";
        }

        public static int GetServerPort(int defaultPort = 8000)
        {
            object v;
            if (ApplicationData.Current.LocalSettings.Values.TryGetValue(ServerPortKey, out v))
            {
                if (v is int) return (int)v;
                int parsed;
                if (int.TryParse(v as string, out parsed)) return parsed;
            }
            return defaultPort;
        }

        public static string GetApiKey()
        {
            object v;
            if (ApplicationData.Current.LocalSettings.Values.TryGetValue(ApiKeyKey, out v))
            {
                return v as string ?? "";
            }
            return "";
        }

        public static void SetServerHost(string host)
        {
            ApplicationData.Current.LocalSettings.Values[ServerHostKey] = host ?? "";
        }

        public static void SetServerPort(int port)
        {
            ApplicationData.Current.LocalSettings.Values[ServerPortKey] = port;
        }

        public static void SetApiKey(string apiKey)
        {
            ApplicationData.Current.LocalSettings.Values[ApiKeyKey] = apiKey ?? "";
        }
    }
}

