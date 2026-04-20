# Windows Phone 8.1 客户端（Nokia Chat）

工程位置：
- `F:\code\S60V3FP1\nokiachat\wp81App\Nokia Chat\Nokia Chat.sln`

## 功能

- 首页：微信消息（占位）/ QQ消息
- QQ消息：
  - 会话列表（拉取 `/v1/qq/conversations`）
  - 会话消息列表（拉取 `/v1/qq/messages`）
  - 发送文字（`/v1/qq/send_text`）

## 运行/部署

1. 用 Visual Studio（带 Windows Phone 8.1 开发组件）打开：
   - `F:\code\S60V3FP1\nokiachat\wp81App\Nokia Chat\Nokia Chat.sln`
2. 确保 `Package.appxmanifest` 已有联网能力（当前为 `internetClientServer`）。
3. 选择目标（真机/模拟器）运行。

## 服务器设置（App 内输入）

打开 App 首页，填写并保存：

- 服务器：公网 IP 或域名（例如 `1.2.3.4`）
- 端口：例如 `8000`
- API Key：必须与服务端 `.env` 的 `NOKIACHAT_API_KEY` 一致

点“测试连接”会调用 `/v1/ping`，成功显示 `ok`。

## 注意

- 当前客户端默认按 `http://{服务器}:{端口}` 拼接（如果你输入的是完整 `http://...` / `https://...`，则直接使用该 URL）。
- 图片收发暂未实现；后续可以在消息列表遇到 `msg_type=image` 时增加下载与展示逻辑。

