# CRC generation
import json

process_imports = True


#
# Plugin entry point
#
def run(output_dir, input_filename, s):
    j = {}
    major = 0
    minor = 0
    patch = 0
    if "version" in s["Option"]:
        v = s["Option"]["version"]
        (major, minor, patch) = v.split(".")
    j["_version"] = {"major": major, "minor": minor, "patch": patch}
    for t in s["Define"]:
        j[t.name] = {"crc": f"{t.crc:#08x}", "version": major, "options": t.options}
    return json.dumps(j, indent=4, separators=(",", ": "))
