#!/bin/bash

# Variables
PROJECT_NAME="afk"
EXECUTABLE_PATH="/usr/local/bin/$PROJECT_NAME"
PLIST_PATH="$HOME/Library/LaunchAgents/com.user.$PROJECT_NAME.plist"

# Step 1: Build the program using CMake
echo "Building the program..."
mkdir -p build
cd build
cmake ..
make
cd ..

# Step 2: Move the executable to /usr/local/bin
echo "Installing the executable to $EXECUTABLE_PATH..."
sudo mv "build/$PROJECT_NAME" "$EXECUTABLE_PATH"
sudo chmod +x "$EXECUTABLE_PATH"

# Step 3: Create LaunchAgent plist for auto-launch at login
echo "Creating LaunchAgent plist at $PLIST_PATH..."
cat << EOF > "$PLIST_PATH"
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>com.user.$PROJECT_NAME</string>
    <key>ProgramArguments</key>
    <array>
        <string>$EXECUTABLE_PATH</string>
    </array>
    <key>RunAtLoad</key>
    <true/>
    <key>KeepAlive</key>
    <true/>
</dict>
</plist>
EOF

# Step 4: Load the LaunchAgent to start at login
echo "Loading LaunchAgent..."
launchctl load "$PLIST_PATH"

echo "Installation complete. $PROJECT_NAME will now run on startup."
