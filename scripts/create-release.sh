SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

if ! command -v git &> /dev/null; then
    echo "Error: git is not installed or not in PATH"
    exit 1
fi

if ! git diff-index --quiet HEAD --; then
    echo "Error: Working directory is not clean. Please commit or stash changes."
    exit 1
fi

echo "Fetching latest changes..."
git fetch origin

CURRENT_BRANCH=$(git symbolic-ref --short HEAD)
if [ "$CURRENT_BRANCH" != "develop" ]; then
    echo "Warning: You are not on the develop branch. Current branch: $CURRENT_BRANCH"
    read -p "Do you want to continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

VERSION_FILE="include/version.h"
if [ ! -f "$VERSION_FILE" ]; then
    mkdir -p include
    echo "#ifndef VERSION_H" > "$VERSION_FILE"
    echo "#define VERSION_H" >> "$VERSION_FILE"
    echo "" >> "$VERSION_FILE"
    echo "#define FIRMWARE_VERSION \"0.1.0\"" >> "$VERSION_FILE"
    echo "" >> "$VERSION_FILE"
    echo "#endif // VERSION_H" >> "$VERSION_FILE"
    echo "Created version.h file with default version 0.1.0"
fi

CURRENT_VERSION=$(grep -o '".*"' "$VERSION_FILE" | sed 's/"//g')
echo "Current version: $CURRENT_VERSION"

read -p "Enter new version number (format X.Y.Z): " NEW_VERSION

if ! [[ $NEW_VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "Error: Version must be in format X.Y.Z"
    exit 1
fi

RELEASE_BRANCH="release/v$NEW_VERSION"
echo "Creating release branch: $RELEASE_BRANCH"
git checkout -b "$RELEASE_BRANCH"

sed -i "s/\"$CURRENT_VERSION\"/\"$NEW_VERSION\"/" "$VERSION_FILE"
echo "Updated version.h to version $NEW_VERSION"

if [ -f "CHANGELOG.md" ]; then
    DATE=$(date +%Y-%m-%d)
    TEMP_FILE=$(mktemp)
    echo "## [$NEW_VERSION] - $DATE" > "$TEMP_FILE"
    echo "" >> "$TEMP_FILE"
    echo "### Added" >> "$TEMP_FILE"
    echo "- " >> "$TEMP_FILE"
    echo "" >> "$TEMP_FILE"
    echo "### Changed" >> "$TEMP_FILE"
    echo "- " >> "$TEMP_FILE"
    echo "" >> "$TEMP_FILE"
    echo "### Fixed" >> "$TEMP_FILE"
    echo "- " >> "$TEMP_FILE"
    echo "" >> "$TEMP_FILE"
    cat "CHANGELOG.md" >> "$TEMP_FILE"
    mv "$TEMP_FILE" "CHANGELOG.md"
    echo "Added new entry to CHANGELOG.md"
    echo "Please update the changelog with actual changes."
fi

git add "$VERSION_FILE"
if [ -f "CHANGELOG.md" ]; then
    git add "CHANGELOG.md"
fi
git commit -m "Bump version to $NEW_VERSION"

echo "Pushing release branch..."
git push -u origin "$RELEASE_BRANCH"

echo ""
echo "Next steps:"
echo "1. Edit CHANGELOG.md to document the changes in this release"
echo "2. Make any other needed adjustments to the release"
echo "3. Create a pull request from $RELEASE_BRANCH to main"
echo "4. After the PR is merged, run the following commands to tag the release:"
echo "   git checkout main"
echo "   git pull"
echo "   git tag -a v$NEW_VERSION -m \"Release v$NEW_VERSION\""
echo "   git push origin v$NEW_VERSION"
echo ""
echo "This will trigger the CI/CD to build the release assets."