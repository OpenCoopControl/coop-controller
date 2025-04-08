# GitHub Workflows for OpenCoopControl

This directory contains GitHub Actions workflows that automate the building, testing, and releasing of the OpenCoopControl firmware.

## Workflows

### Build and Release (`build.yml`)

This workflow is triggered:
- On pushes to `main`, `develop`, and `release/*` branches
- On any tag push that starts with 'v' (e.g., v1.0.0)
- On pull requests targeting `main` or `develop`

The workflow:

1. Sets up the build environment with Python and PlatformIO
2. Builds the firmware
3. Uploads the build as an artifact
4. For tag pushes:
   - Creates a GitHub Release
   - Uploads the firmware, SPIFFS image, and flashing scripts as release assets

## Secrets

No additional secrets are required for the basic workflow. However, if you add deployment to external servers or services, you might need to add secrets to the repository settings.

## Custom Workflow Development

If you need to modify these workflows:

1. Clone this repository
2. Create a feature branch
3. Make your changes to the workflow files
4. Test them by pushing to your feature branch
5. Create a pull request to merge your changes

## Additional Information

- The main script (`build.yml`) is designed to work with PlatformIO's build system
- For ESP32 firmware, it builds both the application firmware and the SPIFFS file system
- When a new tag is pushed, the workflow automatically packages everything needed for end-users to flash their devices

## Future Enhancements

Potential improvements for the CI/CD pipeline:

- Automated testing with unit tests
- Static code analysis
- Automated version bumping
- Deployment to a release server or CDN
- Integration with OTA update mechanisms