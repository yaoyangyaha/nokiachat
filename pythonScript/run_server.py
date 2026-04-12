import os

import uvicorn

from nokiachat_server.api import create_app


def _load_dotenv_if_present() -> None:
    # Very small dotenv loader to avoid extra dependencies on server.
    env_path = os.path.join(os.path.dirname(__file__), ".env")
    if not os.path.exists(env_path):
        return
    with open(env_path, "r", encoding="utf-8") as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith("#") or "=" not in line:
                continue
            k, v = line.split("=", 1)
            os.environ.setdefault(k.strip(), v.strip())


def main() -> None:
    _load_dotenv_if_present()
    host = os.environ.get("NOKIACHAT_BIND_HOST", "0.0.0.0")
    port = int(os.environ.get("NOKIACHAT_BIND_PORT", "8000"))
    uvicorn.run(create_app(), host=host, port=port)


if __name__ == "__main__":
    main()

