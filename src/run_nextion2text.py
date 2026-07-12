#!/usr/bin/env python3
import codecs
import runpy
import sys


def _ansi_codec_alias(encoding):
    if encoding.lower().replace("_", "-") == "ansi":
        return codecs.lookup("latin-1")
    return None


def main():
    if len(sys.argv) < 2:
        print("Usage: run_nextion2text.py PATH_TO_NEXTION2TEXT [ARGS...]", file=sys.stderr)
        return 1

    codecs.register(_ansi_codec_alias)

    script_path = sys.argv[1]
    sys.argv = [script_path, *sys.argv[2:]]
    runpy.run_path(script_path, run_name="__main__")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
