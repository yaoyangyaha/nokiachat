from __future__ import annotations

import asyncio
import json
import time
from typing import Any

import websockets

from .db import NokiachatDb


def _extract_text(message: Any) -> str:
    # OneBot v11 "message" field can be string or CQ segment list.
    if isinstance(message, str):
        return message
    if isinstance(message, list):
        parts: list[str] = []
        for seg in message:
            if not isinstance(seg, dict):
                continue
            if seg.get("type") == "text":
                parts.append(str(seg.get("data", {}).get("text", "")))
            elif seg.get("type") == "image":
                parts.append("[image]")
            else:
                parts.append(f"[{seg.get('type','other')}]")
        return "".join(parts).strip()
    return ""


async def run_napcat_event_listener(db: NokiachatDb, ws_url: str) -> None:
    while True:
        try:
            async with websockets.connect(ws_url, ping_interval=20, ping_timeout=20) as ws:
                async for raw in ws:
                    try:
                        evt = json.loads(raw)
                    except Exception:
                        continue

                    if not isinstance(evt, dict):
                        continue
                    post_type = evt.get("post_type")
                    if post_type != "message":
                        continue

                    message_type = evt.get("message_type")
                    ts = int(evt.get("time") or time.time())
                    sender = (evt.get("sender") or {}).get("nickname") or "QQ"
                    content = _extract_text(evt.get("message"))

                    if message_type == "private":
                        user_id = evt.get("user_id")
                        if user_id is None:
                            continue
                        conv_id = f"user:{int(user_id)}"
                        conv_name = f"{sender}({user_id})"
                    elif message_type == "group":
                        group_id = evt.get("group_id")
                        if group_id is None:
                            continue
                        conv_id = f"group:{int(group_id)}"
                        conv_name = f"群 {group_id}"
                    else:
                        continue

                    db.add_message(
                        conv_id,
                        ts=ts,
                        sender=sender,
                        msg_type="text",
                        content=content or "[non-text]",
                        unread_delta=1,
                        conv_name=conv_name,
                        preview=content or "[non-text]",
                    )
        except Exception:
            await asyncio.sleep(3)

