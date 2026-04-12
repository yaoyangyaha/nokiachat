from __future__ import annotations

import sqlite3
import threading
import time
from dataclasses import dataclass


@dataclass(frozen=True)
class ConversationRow:
    conv_id: str
    name: str
    last_ts: int
    last_preview: str
    unread: int


@dataclass(frozen=True)
class MessageRow:
    msg_id: int
    ts: int
    sender: str
    msg_type: str
    content: str


class NokiachatDb:
    def __init__(self, path: str) -> None:
        self._path = path
        self._lock = threading.Lock()
        self._conn = sqlite3.connect(self._path, check_same_thread=False)
        self._conn.row_factory = sqlite3.Row
        self._init_db()

    def _init_db(self) -> None:
        with self._conn:
            self._conn.execute(
                """
                CREATE TABLE IF NOT EXISTS conversations (
                  conv_id TEXT PRIMARY KEY,
                  name TEXT NOT NULL,
                  last_ts INTEGER NOT NULL DEFAULT 0,
                  last_preview TEXT NOT NULL DEFAULT '',
                  unread INTEGER NOT NULL DEFAULT 0
                )
                """
            )
            self._conn.execute(
                """
                CREATE TABLE IF NOT EXISTS messages (
                  msg_id INTEGER PRIMARY KEY AUTOINCREMENT,
                  conv_id TEXT NOT NULL,
                  ts INTEGER NOT NULL,
                  sender TEXT NOT NULL,
                  msg_type TEXT NOT NULL,
                  content TEXT NOT NULL,
                  FOREIGN KEY (conv_id) REFERENCES conversations(conv_id)
                )
                """
            )
            self._conn.execute(
                "CREATE INDEX IF NOT EXISTS idx_messages_conv_id_msg_id ON messages(conv_id, msg_id)"
            )

    def upsert_conversation(
        self,
        conv_id: str,
        name: str,
        *,
        last_ts: int | None = None,
        last_preview: str | None = None,
        unread_delta: int = 0,
    ) -> None:
        now_ts = int(time.time())
        last_ts_val = last_ts if last_ts is not None else now_ts
        last_preview_val = last_preview if last_preview is not None else ""

        with self._lock, self._conn:
            cur = self._conn.execute(
                "SELECT conv_id, unread FROM conversations WHERE conv_id=?",
                (conv_id,),
            )
            row = cur.fetchone()
            if row is None:
                self._conn.execute(
                    """
                    INSERT INTO conversations (conv_id, name, last_ts, last_preview, unread)
                    VALUES (?, ?, ?, ?, ?)
                    """,
                    (conv_id, name, last_ts_val, last_preview_val, max(0, unread_delta)),
                )
            else:
                self._conn.execute(
                    """
                    UPDATE conversations
                    SET name=?,
                        last_ts=?,
                        last_preview=?,
                        unread=MAX(0, unread + ?)
                    WHERE conv_id=?
                    """,
                    (name, last_ts_val, last_preview_val, unread_delta, conv_id),
                )

    def add_message(
        self,
        conv_id: str,
        *,
        ts: int,
        sender: str,
        msg_type: str,
        content: str,
        unread_delta: int = 1,
        conv_name: str | None = None,
        preview: str | None = None,
    ) -> int:
        conv_title = conv_name or conv_id
        preview_val = preview if preview is not None else (content[:80] if content else msg_type)
        self.upsert_conversation(
            conv_id,
            conv_title,
            last_ts=ts,
            last_preview=preview_val,
            unread_delta=unread_delta,
        )

        with self._lock, self._conn:
            cur = self._conn.execute(
                """
                INSERT INTO messages (conv_id, ts, sender, msg_type, content)
                VALUES (?, ?, ?, ?, ?)
                """,
                (conv_id, ts, sender, msg_type, content),
            )
            return int(cur.lastrowid)

    def list_conversations(self, limit: int = 100) -> list[ConversationRow]:
        with self._lock:
            cur = self._conn.execute(
                """
                SELECT conv_id, name, last_ts, last_preview, unread
                FROM conversations
                ORDER BY last_ts DESC
                LIMIT ?
                """,
                (limit,),
            )
            return [
                ConversationRow(
                    conv_id=r["conv_id"],
                    name=r["name"],
                    last_ts=int(r["last_ts"]),
                    last_preview=r["last_preview"],
                    unread=int(r["unread"]),
                )
                for r in cur.fetchall()
            ]

    def list_messages(
        self, conv_id: str, *, after_id: int = 0, limit: int = 50
    ) -> list[MessageRow]:
        with self._lock:
            cur = self._conn.execute(
                """
                SELECT msg_id, ts, sender, msg_type, content
                FROM messages
                WHERE conv_id=? AND msg_id > ?
                ORDER BY msg_id ASC
                LIMIT ?
                """,
                (conv_id, after_id, limit),
            )
            return [
                MessageRow(
                    msg_id=int(r["msg_id"]),
                    ts=int(r["ts"]),
                    sender=r["sender"],
                    msg_type=r["msg_type"],
                    content=r["content"],
                )
                for r in cur.fetchall()
            ]

    def mark_read(self, conv_id: str) -> None:
        with self._lock, self._conn:
            self._conn.execute(
                "UPDATE conversations SET unread=0 WHERE conv_id=?",
                (conv_id,),
            )

