import os
Import("env")

# This script ensures PlatformIO uses consistent names for firmware and SPIFFS

def post_firmware_bin(source, target, env):
    # Get firmware path and ensure it's named consistently
    firmware_path = str(target[0])
    firmware_dir = os.path.dirname(firmware_path)
    
    # Create standardized firmware filenames
    standard_firmware = os.path.join(firmware_dir, "firmware.bin")
    
    # Copy the files to standardized names
    print(f"Copying {firmware_path} to {standard_firmware}")
    if os.path.exists(firmware_path):
        with open(firmware_path, "rb") as src_file:
            with open(standard_firmware, "wb") as dest_file:
                dest_file.write(src_file.read())
    else:
        print(f"Warning: {firmware_path} does not exist")

def post_buildfs(source, target, env):
    # Get SPIFFS path and ensure it's named consistently
    spiffs_path = str(target[0])
    spiffs_dir = os.path.dirname(spiffs_path)
    
    # Create standardized SPIFFS filename
    standard_spiffs = os.path.join(spiffs_dir, "spiffs.bin")
    
    # Copy the file to standardized name
    print(f"Copying {spiffs_path} to {standard_spiffs}")
    if os.path.exists(spiffs_path):
        with open(spiffs_path, "rb") as src_file:
            with open(standard_spiffs, "wb") as dest_file:
                dest_file.write(src_file.read())
    else:
        print(f"Warning: {spiffs_path} does not exist")
    
# Register callbacks
env.AddPostAction("$BUILD_DIR/firmware.bin", post_firmware_bin)
env.AddPostAction("$BUILD_DIR/spiffs.bin", post_buildfs)