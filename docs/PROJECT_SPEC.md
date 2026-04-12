# Nokiachat 项目具体方案（QQ 转接到 S60v3 FP1）

## 目标

- 公网服务器用 Napcat(OneBot v11)拿到 QQ 会话消息（文字/图片等），落库并提供给塞班端拉取。
- 诺基亚 N95（S60v3 FP1）上展示：
  - 首页：微信消息 / QQ消息（当前只实现 QQ）
  - QQ：会话列表 → 会话详情（历史消息）→ 可发送文字（图片后续扩展）

## 总体架构

1) Napcat 通过 WebSocket 推送 OneBot 事件到 `pythonScript` 服务端  
2) 服务端把事件转成“会话 + 消息”写入 SQLite  
3) 塞班端通过 HTTP 拉取会话/消息（TSV 文本，便于 S60 解析）  
4) 塞班端发送文字 → 服务端 `/send_text` → Napcat HTTP API → QQ

## 会话标识（conv_id）

- 私聊：`user:<qq>`，例如 `user:123456`
- 群聊：`group:<qq>`，例如 `group:987654`

## 服务器端（pythonScript）

代码位置：`F:\\code\\S60V3FP1\\nokiachat\\pythonScript`

### 配置

- 复制 `pythonScript\\.env.example` 为 `pythonScript\\.env`
  - `NOKIACHAT_API_KEY`：塞班端请求头 `X-Api-Key` 的值
  - `NAPCAT_HTTP_BASE`：Napcat OneBot v11 HTTP API 基地址
  - `NAPCAT_WS_EVENTS`：Napcat OneBot v11 事件 WebSocket 地址
  - `NAPCAT_ACCESS_TOKEN`：如果 Napcat 网络配置里设置了 Token，这里填同一个（服务端会用 `Authorization: Bearer ...` 调用 Napcat）

### 启动

- `pythonScript\\run_server.py`（uvicorn 启动 FastAPI）

### 接口（TSV）

都需要请求头：`X-Api-Key: <key>`

- `GET /v1/qq/conversations`
  - 每行：`conv_id\tname\tlast_ts\tlast_preview\tunread`
- `GET /v1/qq/messages?conv_id=...&after_id=0&limit=50`
  - 每行：`msg_id\tts\tsender\tmsg_type\tcontent`
- `POST /v1/qq/send_text`（`application/x-www-form-urlencoded`）
  - `conv_id=...&text=...`

## 塞班端（sybianApp）

代码位置：`F:\\code\\S60V3FP1\\nokiachat\\sybianApp`

### 关键文件

- `sybianApp\\inc\\NokiachatProto.h`：配置服务器地址和 API Key
- `sybianApp\\src\\NokiachatHttpClient.cpp`：HTTP GET/POST（RHTTPTransaction）
- `sybianApp\\src\\NokiachatContainer.cpp`：UI + 拉取解析（会话/消息）

### 当前操作说明（Demo）

- 打开应用 → 选择“QQ消息”会拉取会话列表
- 进入会话 → 拉取消息列表
- 在消息列表按数字键 `5` → 输入文字 → 发送（调用 `/v1/qq/send_text`）
- 在会话列表/消息列表按退格键返回上一层

## 下一步扩展（按优先级）

1) 图片收发：服务端把图片落到可下载 URL（或转码为小图），塞班端下载并用 `CFbsBitmap` 展示/保存
2) 增量更新：塞班端保存 `last_msg_id`，使用 `after_id` 拉新消息；定时刷新未读数
3) 更好的 UI：多 view（主菜单/会话列表/会话详情），增加 Options 菜单（刷新/发送/保存图）
4) 安全：把 HTTP 改成 HTTPS（注意 S60 TLS 兼容性），或走自建隧道/反代
