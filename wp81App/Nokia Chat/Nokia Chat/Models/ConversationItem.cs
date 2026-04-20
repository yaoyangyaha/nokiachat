namespace Nokia_Chat.Models
{
    public class ConversationItem
    {
        public string ConvId { get; set; }
        public string Name { get; set; }
        public long LastTs { get; set; }
        public string LastPreview { get; set; }
        public int Unread { get; set; }

        public string TitleText
        {
            get
            {
                if (Unread > 0) return Name + " (" + Unread + ")";
                return Name;
            }
        }

        public string PreviewText
        {
            get { return LastPreview ?? ""; }
        }
    }
}

