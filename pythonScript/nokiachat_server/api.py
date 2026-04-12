from __future__ import annotations

import asyncio
from contextlib import asynccontextmanager

from fastapi import FastAPI, Form, HTTPException, Request, Response

from . import config
from .db import NokiachatDb
from .napcat_listener import run_napcat_event_listener
from .napcat_onebot import NapcatOneBotClient


def _tsv_escape(s: str) -> str:
    # Keep protocol easy: avoid tabs/newlines.
    return s.replace("\t", " ").replace("\r", " ").replace("\n", " ")


def create_app() -> FastAPI:
    db = NokiachatDb(config.db_path())
    napcat_http = config.napcat_http_base()
    napcat_ws = config.napcat_ws_events()
    napcat_client = NapcatOneBotClient(napcat_http) if napcat_http else None

    @asynccontextmanager
    async def lifespan(app: FastAPI):
        task = None
        if napcat_ws:
            task = asyncio.create_task(run_napcat_event_listener(db, napcat_ws))
        try:
            yield
        finally:
            if task:
                task.cancel()
            if napcat_client:
                await napcat_client.aclose()

    app = FastAPI(title="nokiachat", lifespan=lifespan)

    @app.middleware("http")
    async def auth_middleware(request: Request, call_next):
        if request.url.path.startswith("/v1/"):
            key = request.headers.get("x-api-key", "")
            if key != config.api_key():
                return Response("unauthorized\n", status_code=401, media_type="text/plain")
        return await call_next(request)

    @app.get("/v1/ping")
    async def ping():
        return Response("ok\n", media_type="text/plain")

    @app.get("/v1/qq/conversations")
    async def list_conversations(limit: int = 100):
        rows = db.list_conversations(limit=limit)
        body = "\n".join(
            "\t".join(
                [
                    _tsv_escape(r.conv_id),
                    _tsv_escape(r.name),
                    str(r.last_ts),
                    _tsv_escape(r.last_preview),
                    str(r.unread),
                ]
            )
            for r in rows
        )
        if body:
            body += "\n"
        return Response(body, media_type="text/plain; charset=utf-8")

    @app.get("/v1/qq/messages")
    async def list_messages(conv_id: str, after_id: int = 0, limit: int = 50):
        rows = db.list_messages(conv_id, after_id=after_id, limit=limit)
        body = "\n".join(
            "\t".join(
                [
                    str(r.msg_id),
                    str(r.ts),
                    _tsv_escape(r.sender),
                    _tsv_escape(r.msg_type),
                    _tsv_escape(r.content),
                ]
            )
            for r in rows
        )
        if body:
            body += "\n"
        return Response(body, media_type="text/plain; charset=utf-8")

    @app.post("/v1/qq/mark_read")
    async def mark_read(conv_id: str = Form(...)):
        db.mark_read(conv_id)
        return Response("ok\n", media_type="text/plain")

    @app.post("/v1/qq/send_text")
    async def send_text(conv_id: str = Form(...), text: str = Form(...)):
        if napcat_client is None:
            raise HTTPException(status_code=503, detail="NAPCAT_HTTP_BASE not configured")
        await napcat_client.send_text(conv_id, text)
        db.add_message(
            conv_id,
            ts=int(__import__("time").time()),
            sender="me",
            msg_type="text",
            content=text,
            unread_delta=0,
            conv_name=conv_id,
            preview=text,
        )
        return Response("ok\n", media_type="text/plain")

    return app

