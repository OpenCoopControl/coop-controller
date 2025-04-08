# Contributing to OpenCoopControl

Thank you for your interest in contributing to OpenCoopControl! This document outlines the process for contributing to the project and explains our branch structure and development workflow.

## Branch Structure

We use the following branch structure:

- `main` - The production branch containing stable code
- `develop` - Integration branch for features before they go to main
- `feature/*` - Feature branches for new functionality
- `bugfix/*` - Branches for bug fixes
- `release/*` - Release candidate branches
- `hotfix/*` - Emergency fixes for production issues

## Development Workflow

### Starting a New Feature

1. Create a new branch from `develop`:
   ```bash
   git checkout develop
   git pull
   git checkout -b feature/your-feature-name
   ```

2. Make your changes, commit them with descriptive messages:
   ```bash
   git add .
   git commit -m "Add feature X that does Y"
   ```

3. Push your branch:
   ```bash
   git push -u origin feature/your-feature-name
   ```

4. Create a Pull Request to merge your changes into `develop`

### Creating a Bug Fix

1. Create a new branch from `develop` (or `main` for critical fixes):
   ```bash
   git checkout develop
   git pull
   git checkout -b bugfix/issue-description
   ```

2. Fix the bug, commit your changes:
   ```bash
   git add .
   git commit -m "Fix issue with Z"
   ```

3. Push your branch and create a Pull Request

### Creating a Release

1. When `develop` is ready for release, create a release branch:
   ```bash
   git checkout develop
   git pull
   git checkout -b release/v1.0.0
   ```

2. Make any final adjustments, version number updates, etc.

3. Create a Pull Request to merge into `main`

4. After merging to `main`, tag the release:
   ```bash
   git checkout main
   git pull
   git tag -a v1.0.0 -m "Release v1.0.0"
   git push origin v1.0.0
   ```

## Coding Standards

- Use consistent indentation (spaces, not tabs)
- Include comments for complex logic
- Follow the existing code style
- Keep functions focused on a single responsibility
- Add descriptive comments for pin configurations and hardware connections

## Testing

Before submitting a pull request:

1. Test your changes on actual ESP32 hardware if possible
2. Ensure the build succeeds with PlatformIO
3. Verify that your changes don't break existing functionality

## Commit Messages

Write clear, descriptive commit messages that explain what changes were made and why:

- Start with a short summary (50 chars or less)
- Use the imperative mood ("Add feature" not "Added feature")
- Reference issue numbers when applicable ("Fix #123: Resolve door timing issue")

## Pull Requests

When submitting a pull request:

1. Describe the changes you've made
2. Explain why these changes are necessary
3. Mention any breaking changes
4. Include screenshots for UI changes
5. Reference related issues

## Documentation

Update the relevant documentation when adding or changing features:

- Update the README.md if necessary
- Document new settings or configuration options
- Add comments to explain complex code sections

Thank you for contributing to OpenCoopControl!