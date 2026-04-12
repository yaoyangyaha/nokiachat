import os


def db_path() -> str:
    return os.environ.get("NOKIACHAT_DB_PATH", "./nokiachat.sqlite3")


def api_key() -> str:
    return os.environ.get("NOKIACHAT_API_KEY", "change-me")


def napcat_http_base() -> str | None:
    v = os.environ.get("NAPCAT_HTTP_BASE", "").strip()
    return v or None


def napcat_ws_events() -> str | None:
    v = os.environ.get("NAPCAT_WS_EVENTS", "").strip()
    return v or None

