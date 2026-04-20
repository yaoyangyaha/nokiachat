using System;

namespace Nokia_Chat.Models
{
    public class MessageItem
    {
        public long MsgId { get; set; }
        public long Ts { get; set; }
        public string Sender { get; set; }
        public string MsgType { get; set; }
        public string Content { get; set; }

        public string DisplayText
        {
            get
            {
                if (string.IsNullOrEmpty(MsgType) || MsgType == "text")
                {
                    return (Sender ?? "") + ": " + (Content ?? "");
                }
                return (Sender ?? "") + ": [" + MsgType + "] " + (Content ?? "");
            }
        }

        public string TimeText
        {
            get
            {
                try
                {
                    var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
                    var dt = epoch.AddSeconds(Ts).ToLocalTime();
                    return dt.ToString("MM-dd HH:mm");
                }
                catch
                {
                    return "";
                }
            }
        }
    }
}
