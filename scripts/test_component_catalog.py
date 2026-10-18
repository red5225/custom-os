#!/usr/bin/env python3
"""Validate manifests and compile/run each independently testable catalog component."""
from __future__ import annotations
import json
import pathlib
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
COMPONENT_ROOT = ROOT / "components"
manifests = sorted(COMPONENT_ROOT.glob("*/*/module.json"))
if len(manifests) != 100:
    raise SystemExit(f"expected 100 component manifests, found {len(manifests)}")

seen_ids = set()
for manifest_path in manifests:
    item = json.loads(manifest_path.read_text(encoding="utf-8"))
    required = {"name", "display_name", "domain", "component_id", "api_version", "maturity", "build_test", "integrated_into_boot_image"}
    missing = required - item.keys()
    if missing:
        raise SystemExit(f"{manifest_path}: missing manifest fields {sorted(missing)}")
    if item["component_id"] in seen_ids:
        raise SystemExit(f"{manifest_path}: duplicate component_id {item['component_id']}")
    seen_ids.add(item["component_id"])
    if item["integrated_into_boot_image"] is not False:
        raise SystemExit(f"{manifest_path}: catalog scaffold must not claim boot integration")

    module_dir = manifest_path.parent
    header = module_dir / "include/module.h"
    source = module_dir / "src/module.c"
    test = module_dir / "tests/test_module.c"
    for required_path in (header, source, test, module_dir / "README.md"):
        if not required_path.is_file():
            raise SystemExit(f"{manifest_path}: missing {required_path.relative_to(ROOT)}")

    with tempfile.TemporaryDirectory(prefix="customos-component-") as temp:
        binary = pathlib.Path(temp) / "test-module"
        command = [
            "gcc", "-std=c11", "-Wall", "-Wextra", "-Werror",
            "-I", str(module_dir / "include"),
            str(source), str(test), "-o", str(binary),
        ]
        subprocess.run(command, check=True)
        subprocess.run([str(binary)], check=True)

print(f"PASS: validated {len(manifests)} manifests and compiled/ran {len(manifests)} component self-tests.")
