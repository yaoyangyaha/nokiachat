# Nokiachat (Server)

本目录是公网服务器端：接入 Napcat(OneBot v11)，把 QQ 消息落库并提供给塞班客户端拉取，同时接收塞班端的发送请求并转发到 QQ。

## 运行

1. 安装依赖：
   - `python -m venv .venv`
   - `.\.venv\Scripts\pip install -r requirements.txt`
2. 配置环境变量：
   - 复制 `.env.example` 为 `.env` 并修改
3. 启动：
   - `.\.venv\Scripts\python run_server.py`

## 协议（塞班端友好：纯文本 TSV）

- `GET /v1/qq/conversations`
  - 返回多行：`conv_id\tname\tlast_ts\tlast_preview\tunread`
- `GET /v1/qq/messages?conv_id=...&after_id=0&limit=50`
  - 返回多行：`msg_id\tts\tsender\tmsg_type\tcontent`
  - `msg_type`: `text` / `image` / `other`
- `POST /v1/qq/send_text`（`application/x-www-form-urlencoded`）
  - 参数：`conv_id`、`text`

认证：
- 所有 `/v1/*` 需要请求头 `X-Api-Key: <NOKIACHAT_API_KEY>`

