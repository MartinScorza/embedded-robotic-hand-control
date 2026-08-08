#!/usr/bin/env python3
"""Fail CI if tracked files contain known portfolio-publication hazards.

This is a repository hygiene check, not a legal review. The rules intentionally
reflect the current publication decisions for this academic project and can be
relaxed later if provenance/redistribution questions are resolved.
"""

from __future__ import annotations

import subprocess
from pathlib import Path


SELF_PATH = Path("scripts/check_public_tree.py")

FORBIDDEN_PATH_PARTS = {
    "Debug",
    "Release",
    ".launches",
}

FORBIDDEN_SUFFIXES = {
    ".out",
    ".obj",
    ".map",
    ".d",
    ".7z",
    ".rar",
    ".zip",
}

# Board-specific/internal material intentionally excluded pending provenance and
# redistribution review.
FORBIDDEN_BASENAMES = {
    "Hand_Defines.h",
    "Hand_SPI_IT.c",
    "Hand_SPI_IT.h",
    "Hand_LowLevel.c",
    "Hand_LowLevel.h",
    "utc_logo.png",
}

FORBIDDEN_CONTENT_MARKERS = {
    "C:/Users/": "absolute Windows user path",
    "C:\\Users\\": "absolute Windows user path",
    "OneDrive": "local OneDrive path/reference",
}


def tracked_files() -> list[Path]:
    result = subprocess.run(
        ["git", "ls-files", "-z"],
        check=True,
        stdout=subprocess.PIPE,
    )
    return [Path(p.decode()) for p in result.stdout.split(b"\0") if p]


def check_path(path: Path) -> list[str]:
    problems: list[str] = []

    if any(part in FORBIDDEN_PATH_PARTS for part in path.parts):
        problems.append(f"generated/local directory tracked: {path}")

    if path.suffix.lower() in FORBIDDEN_SUFFIXES:
        problems.append(f"generated/archive file tracked: {path}")

    if path.name in FORBIDDEN_BASENAMES:
        problems.append(f"currently non-public UTC/internal asset tracked: {path}")

    return problems


def check_content(path: Path) -> list[str]:
    problems: list[str] = []

    # The guard source necessarily contains its own marker strings.
    if path == SELF_PATH:
        return problems

    try:
        data = path.read_bytes()
    except OSError as exc:
        return [f"could not read tracked file {path}: {exc}"]

    # Skip large or clearly binary content. Path-level rules still apply.
    if len(data) > 1_000_000 or b"\0" in data:
        return problems

    text = data.decode("utf-8", errors="ignore")
    for marker, description in FORBIDDEN_CONTENT_MARKERS.items():
        if marker in text:
            problems.append(f"{description} found in {path}: {marker!r}")

    return problems


def main() -> int:
    problems: list[str] = []

    for path in tracked_files():
        problems.extend(check_path(path))
        problems.extend(check_content(path))

    if problems:
        print("Public-release guard FAILED:\n")
        for problem in problems:
            print(f"- {problem}")
        print(
            "\nRemove the tracked material or update the guard only after the "
            "publication/provenance decision has been documented."
        )
        return 1

    print("Public-release guard PASS: no configured publication hazards found.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
