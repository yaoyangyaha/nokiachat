from __future__ import annotations

import httpx


class NapcatOneBotClient:
    """
    OneBot v11 HTTP API client.

    conv_id format (for this project):
      - "user:<qq>"   private chat
      - "group:<qq>"  group chat
    """

    def __init__(self, http_base: str) -> None:
        self._http_base = http_base.rstrip("/")
        self._client = httpx.AsyncClient(timeout=10.0)

    async def aclose(self) -> None:
        await self._client.aclose()

    @staticmethod
    def _parse_conv_id(conv_id: str) -> tuple[str, int]:
        if conv_id.startswith("user:"):
            return ("user", int(conv_id.split(":", 1)[1]))
        if conv_id.startswith("group:"):
            return ("group", int(conv_id.split(":", 1)[1]))
        raise ValueError("Invalid conv_id, expected user:<qq> or group:<qq>")

    async def send_text(self, conv_id: str, text: str) -> None:
        kind, qq = self._parse_conv_id(conv_id)
        if kind == "user":
            await self._client.post(
                f"{self._http_base}/send_private_msg",
                json={"user_id": qq, "message": text},
            )
        else:
            await self._client.post(
                f"{self._http_base}/send_group_msg",
                json={"group_id": qq, "message": text},
            )

