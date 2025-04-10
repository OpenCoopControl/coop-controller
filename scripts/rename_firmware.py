import os
Import("env")

def post_firmware_bin(source, target, env):
    firmware_path = str(target[0])
    firmware_dir = os.path.dirname(firmware_path)
    standard_firmware = os.path.join(firmware_dir, "firmware.bin")
    print(f"Copying {firmware_path} to {standard_firmware}")
    if os.path.exists(firmware_path) and os.path.getsize(firmware_path) > 0:
        with open(firmware_path, "rb") as src_file:
            with open(standard_firmware, "wb") as dest_file:
                dest_file.write(src_file.read())
    else:
        print(f"Error: {firmware_path} does not exist or is empty!")
        env.Execute("ls -la " + firmware_dir)
        raise Exception("Firmware file missing or empty")

env.AddPostAction("$BUILD_DIR/firmware.bin", post_firmware_bin)
env.AddPostAction("$BUILD_DIR/spiffs.bin", post_buildfs)