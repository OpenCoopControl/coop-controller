# Versioning Policy for OpenCoopControl

OpenCoopControl follows [Semantic Versioning](https://semver.org/) (SemVer) for its releases.

## Version Format

Versions are numbered in the format of `X.Y.Z` where:

- `X` is the major version (incremented for incompatible API changes)
- `Y` is the minor version (incremented for backwards-compatible new features)
- `Z` is the patch version (incremented for backwards-compatible bug fixes)

Additional labels for pre-release and build metadata may be appended as extensions:
- Alpha releases: `X.Y.Z-alpha.N`
- Beta releases: `X.Y.Z-beta.N`
- Release candidates: `X.Y.Z-rc.N`

## Version Increments

### Major Version (X)

Increment the major version when:
- Making incompatible hardware changes
- Breaking changes to the web API
- Restructuring the settings file format
- Changing pin assignments that require rewiring

### Minor Version (Y)

Increment the minor version when:
- Adding new features in a backwards-compatible manner
- Deprecating functionality (but not removing it)
- Adding new configuration options
- Adding support for new sensors or modules

### Patch Version (Z)

Increment the patch version when:
- Making backwards-compatible bug fixes
- Performance improvements
- Documentation updates
- Small tweaks that don't add new features

## Release Cycle

1. Development occurs on feature branches
2. Features are merged into the `develop` branch
3. When ready for release, a `release/vX.Y.Z` branch is created
4. After testing, the release branch is merged to `main`
5. The release is tagged as `vX.Y.Z`

## Release Assets

Each tagged release will automatically generate:
- Firmware binary (`opencoopcontrol_vX.Y.Z_firmware.bin`)
- SPIFFS filesystem binary (`opencoopcontrol_vX.Y.Z_spiffs.bin`)
- Flashing scripts for Windows and Linux/Mac

## Version Compatibility

The firmware will check for version compatibility with the stored configuration during initialization. If the configuration format has changed in a breaking way, the firmware will reset to defaults.

## Updating Between Versions

### Minor and Patch Updates

For minor and patch updates, users can simply flash the new binaries. Settings and configurations should be preserved.

### Major Updates

For major version updates, users should:
1. Backup any custom configurations
2. Perform a full flash (both firmware and filesystem)
3. Reconfigure the device as needed

## Release Notes

Release notes should include:
- Summary of changes
- New features
- Bug fixes
- Known issues
- Required hardware/wiring changes (if any)
- Special update instructions (if necessary)

This versioning policy helps ensure that users can confidently update their systems while understanding the potential impact of each update.